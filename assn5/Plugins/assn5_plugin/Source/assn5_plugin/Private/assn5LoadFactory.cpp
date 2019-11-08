// Fill out your copyright notice in the Description page of Project Settings.

#ifdef __clang__
#pragma clang optimize off
#else
#pragma optimize("", off)
#endif

#include "assn5LoadFactory.h"

#include "Engine/StaticMesh.h"
#include "Misc/FeedbackContext.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

#include "AssetRegistryModule.h"
#include "Editor/UnrealEd/Public/Editor.h"
#include "Engine/StaticMesh.h"
//#include "IMessageLogListing.h"
//#include "Interfaces/IMainFrameModule.h"
#include "Logging/LogMacros.h"
#include "Logging/TokenizedMessage.h"
#include "Materials/Material.h"
//#include "MessageLogModule.h"
#include "PackageTools.h"
#include "UObject/StrongObjectPtr.h"

//added to include volume texture
#include "Engine/VolumeTexture.h"

//filehelper will be used for loading text file to array
#include "Misc/FileHelper.h"

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

	//logging full file path
	UE_LOG(LogTemp, Warning, TEXT("Opening file at the following path: %s"), *Filepath);
	
	//array for holding contents of file
	TArray <FString> fileContents; 

	//load mha or mhd text file to array
	FFileHelper::LoadFileToStringArray(fileContents,*Filepath);

	//get size of the array of file lines so I can check if the .raw file string is last or not 
	int32 arraySize = fileContents.Num();

	//reading over and logging array to confirm if this step worked
	//also, taking actions based on contents of each file line
	int count = 0;
	for (FString myString : fileContents)
	{
		UE_LOG(LogTemp, Warning, TEXT("String at line %d is %s"),count, *myString);
		
		//First string should show that object is image. I am assuming the string has to exactly equal this format but ignores case.
		//Otherwise, what if there is an object type called an "Image Map" or something else with a space. 
		//Contains won't be sufficient to check if this is the correct object type. 
		if (count == 0)
		{
			if (!myString.Equals(TEXT("ObjectType = Image"), ESearchCase::IgnoreCase))
			{
				UE_LOG(LogTemp, Warning, TEXT("The first line of the file does not declare it is an image file"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("This is an 'Image' ObjectType"));
			}
		}

		count++;
		
	}

	//set object to my texture once I am done setting it up
	Object = myTexture;

		/*
		GEditor->GetEditorSubsystem<UImportSubsystem>()->BroadcastAssetPreImport(this, InClass, InParent, InName, Parms);

		Warn->Log(Filename);

		//TStrongObjectPtr<UGLTFImportOptions> ImporterOptions(NewObject<UGLTFImportOptions>(GetTransientPackage(), TEXT("GLTF Importer Options")));
		// show import options window
		{
			const FString Filepath = FPaths::ConvertRelativePathToFull(Filename);
			const FString PackagePath = InParent->GetPathName();

			//if (!GLTFImporterImpl::ShowOptionsWindow(Filepath, PackagePath, *ImporterOptions))
			//{
			//	bOutOperationCanceled = true;
			//	return nullptr;
			//}
		}

		//FGLTFImporterContext& Context = GLTFImporterModule->GetImporterContext();
		// setup import options
		{
			//Context.StaticMeshFactory.SetUniformScale(ImporterOptions->ImportScale);
			//Context.StaticMeshFactory.SetGenerateLightmapUVs(ImporterOptions->bGenerateLightmapUVs);
		}

		UObject* Object = nullptr;
		/*if (Context.OpenFile(Filename))
		{
			const FString AssetName = Context.Asset.Name;
			const FString NewPackageName = UPackageTools::SanitizePackageName(*(FPaths::GetPath(InParent->GetName()) / AssetName));
			UObject*      ParentPackage = NewPackageName == InParent->GetName() ? InParent : CreatePackage(nullptr, *NewPackageName);

			const TArray<UStaticMesh*>& CreatedMeshes = Context.CreateMeshes(ParentPackage, Flags, false);
			Context.CreateMaterials(ParentPackage, Flags);
			UpdateMeshes();

			if (CreatedMeshes.Num() == 1)
			{
				Object = CreatedMeshes[0];
			}
			else if (CreatedMeshes.Num() != 0)
			{
				Object = CreatedMeshes[0]->GetOutermost();
			}
		}
		*/
		/**
		GEditor->GetEditorSubsystem<UImportSubsystem>()->BroadcastAssetPostImport(this, Object);

		//GLTFImporterImpl::ShowLogMessages(Context.GetLogMessages());

		return Object;
		*/

		return Object;
}

#ifdef __clang__
#pragma clang optimize on
#else
#pragma optimize("", on)
#endif