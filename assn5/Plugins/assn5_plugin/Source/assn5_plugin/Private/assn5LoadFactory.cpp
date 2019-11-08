// Fill out your copyright notice in the Description page of Project Settings.

#ifdef __clang__
#pragma clang optimize off
#else
#pragma optimize("", off)
#endif

#include "assn5LoadFactory.h"

#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

#include "Editor/UnrealEd/Public/Editor.h"

//added to include volume texture
#include "Engine/VolumeTexture.h"

#define LOCTEXT_NAMESPACE "assn5LoadFactory"

Uassn5LoadFactory::Uassn5LoadFactory(const FObjectInitializer& ObjectInitializer)
{
	bCreateNew    = false;
	bEditAfterNew = false;
	bEditorImport = true;   // binary / general file source
	bText         = false;  // text source
	
	SupportedClass = UVolumeTexture::StaticClass();
	
	Formats.Add(TEXT("mha;mha format"));
	Formats.Add(TEXT("mhd;mhd format"));
}

UObject * Uassn5LoadFactory::FactoryCreateFile(UClass * InClass, UObject * InParent, FName InName, EObjectFlags Flags, const FString & Filename, const TCHAR * Parms, FFeedbackContext * Warn, bool & bOutOperationCanceled)
{
	//create blank texture
	UVolumeTexture * myTexture = NewObject<UVolumeTexture>(InParent, InName, Flags);
	
	//create null object
	UObject* Object = nullptr;

	//get full file path
	const FString Filepath = FPaths::ConvertRelativePathToFull(Filename);

	//logging full file path for debugging but commenting out for final project
	//UE_LOG(LogTemp, Warning, TEXT("Opening file at the following path: %s"), *Filepath);
	
	//array for holding contents of file
	TArray <FString> fileContents; 

	//load mha or mhd text file to array
	FFileHelper::LoadFileToStringArray(fileContents,*Filepath);

	//get size of the array of file lines so I can check if the .raw file string is last or not 
	int32 arraySize = fileContents.Num();

	//for storing some values I am parsing from file
	int32 myNDims=-1;
	int32 dimSizeX = -1;
	int32 dimSizeY = -1;
	int32 dimSizeZ = -1;
	int32 myNumChannels = -1;

	//reading over and logging array to confirm if this step worked
	//also, taking actions based on contents of each file line
	int count = 0;
	for (FString myString : fileContents)
	{
		//for debugging but removed for final program
		//UE_LOG(LogTemp, Warning, TEXT("String at line %d is %s"),count, *myString);
		
		//First string should show that object is image. I am assuming the string has to exactly equal this format but ignores case.
		//Otherwise, what if there is an object type called an "Image Map" or something else with a space. 
		//Contains won't be sufficient to check if this is the correct object type. 
		if (count == 0)
		{
			if (!myString.Equals(TEXT("ObjectType = Image"), ESearchCase::IgnoreCase))
			{
				UE_LOG(LogTemp, Warning, TEXT("The first line of the file does not declare it is an image file"));
				return nullptr;
			}

			//for debugging but removed for final program
			//else
			//{
			//	UE_LOG(LogTemp, Warning, TEXT("This is an 'Image' ObjectType"));
			//}
		}

		else if (count == (arraySize -1))
		{
			//for debugging purposes but commenting out for final program.
			//UE_LOG(LogTemp, Warning, TEXT("The final line is %s"), *myString);
		}
		else
		{
			//check if NDims are correct. Must equal 3 since I am undergrad
			if (myString.Contains(TEXT("NDims")))
			{
				//not 3 dimensions
				if (!myString.Equals(TEXT("NDims = 3"), ESearchCase::IgnoreCase))
				{
					UE_LOG(LogTemp, Warning, TEXT("Value error for NDims"));
					return nullptr;
				}
				else
				{
					//set NDims based on file
					myNDims = 3;

					//output that ndims equals 3 for debugging. later commenting out for final project
					//UE_LOG(LogTemp, Warning, TEXT("NDims = 3"));
				}
			}
			//otherwise check if it is dim size and has an array of three different values
			//if not 3 positive values, output error
			else if (myString.Contains(TEXT("DimSize =")))
			{
				//Remove everything except dim size parameters. I am assuming it is formatted as dimsize = some dims
				//Otherwise format is incorrect.
				FString myDimSizes = myString.RightChop(10);

				//for debugging purposes print them, but commented out in final code
				//UE_LOG(LogTemp, Warning, TEXT("My dim sizes are %s"), *myDimSizes);

				//now take these dims (should be three) and make them into an array. 
				//if array is not of size 3 or if any number is <=0, then output an error

				//making array for storing them
				TArray <FString> myDimSizesArray;

				//parse into array useing whitespace as seperator
				myDimSizes.ParseIntoArrayWS(myDimSizesArray);

				//first check if array is not right size
				if (!(myDimSizesArray.Num() == 3))
				{
					UE_LOG(LogTemp, Warning, TEXT("My dim sizes have the incorrect number of dimensions"));
					return nullptr;
				}

				//loop over the results
				int countDim = 0;
				for (FString dimSize : myDimSizesArray)
				{
					int32 tempInt = FCString::Atoi(*dimSize);
					switch (countDim)
					{
						case 0:
							if (tempInt <= 0)
							{
								UE_LOG(LogTemp, Warning, TEXT("Negative or 0 dimension for X"))
								return nullptr;
							}
							else
								dimSizeX = tempInt;
							break;
						case 1:
							if (tempInt <= 0)
							{
								UE_LOG(LogTemp, Warning, TEXT("Negative or 0 dimension for Y"))
								return nullptr;
							}
							else
								dimSizeY = tempInt;
							break;

						case 2:
							if (tempInt <= 0)
							{
								UE_LOG(LogTemp, Warning, TEXT("Negative or 0 dimension for Z/Slices"))
								return nullptr;
							}
							else
								dimSizeZ = tempInt;
							break;
						default:
							UE_LOG(LogTemp, Warning, TEXT("My dim sizes have the incorrect number of dimensions"));
							return nullptr;
							break;
					}
					countDim++;
				}
				
				//for debugging purposes to see dimensions. comment out later for final program
				//UE_LOG(LogTemp, Warning, TEXT("My dimensions are %d, %d, %d"), dimSizeX, dimSizeY, dimSizeZ);
			}
			//look at number of channels. since I am not doing extra credit, one channel is all that is allowed
			else if (myString.Contains(TEXT("ElementNumberOfChannels")))
			{
				FString nChannels = myString.RightChop(25);

				int32 tempInt = FCString::Atoi(*nChannels);

				if (tempInt== 1)
				{
					myNumChannels = tempInt;
				}
				else
					UE_LOG(LogTemp, Warning, TEXT("Incorrect value for ElementNumberOfChannels"));
				
				//for debugging purposes and commented out later
				//UE_LOG(LogTemp, Warning, TEXT("My num channels is %d"), myNumChannels);
			}

			else
			{
				//do nothing since other headers are not supported
			}

		}
		count++;
		
	}

	//set object to my texture once I am done setting it up
	Object = myTexture;
	
	return Object;
}

#ifdef __clang__
#pragma clang optimize on
#else
#pragma optimize("", on)
#endif