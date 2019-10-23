// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "assn4PluginBPLibrary.h"
#include "assn4Plugin.h"

//structure used for my plugin
FmyBlueNoiseStruct myBlueNoise;

Uassn4PluginBPLibrary::Uassn4PluginBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	//initialize it with empty constructor
	myBlueNoise = FmyBlueNoiseStruct();
}

//Code to return point as FVector via blue noise randomization through Mitchell's Best-candidate algorithm 
FVector Uassn4PluginBPLibrary::BlueNoisePointInRange(float Min, float Max, const FRandomStream& BlueNoiseStream)
{

	//set of temporary 2d locations as candidates
	TSet<FVector> tempLocations;

	//Vector for initial location of the instanced static mesh used as starting point for mitchell algorithm. 
	FVector initialLocation = FVector(0,0,0);

	//add this initial point to the set. later transformations away from this point will be used as candidates
	myBlueNoise.twoDLocations.Add(initialLocation); 

	//for loop iterates 5N times to get set of 5N candidates
	for(int i=0; i<(5*myBlueNoise.twoDLocations.Num()); i++)
	{

		//check for duplicate candidates being added by rng. 
		//This makes sure we add new points and look at 5n candidates with every new point
		bool duplicate = true;

		//get size of the set before attempting to add
		int32 pastSize = tempLocations.Num();

		//keep trying to add candidate until size of set increases
		while(duplicate)
		{
			//get random values for x and y position candidates. later make blue noise
			float xValue= (Min + (Max - Min) * (BlueNoiseStream.FRand()));
			float yValue= (Min + (Max - Min) * (BlueNoiseStream.FRand()));

			//construct position vector
			FVector candidatePositionVec(xValue,yValue,0);

			//if this new rng vector was not already in the visited vectors list
			if(!myBlueNoise.twoDLocations.Contains(candidatePositionVec))
			{
				//add position vector to set of candidates
				tempLocations.Add(candidatePositionVec);
			}

			//if adding position vector grew the size of the candidate set, it was not a duplicate and loop exits
			if(tempLocations.Num()>pastSize)
				duplicate = false;
		}

	}

	//Variable for storing largest total distance so far
	//Make this some negative distance so first distance will always be higher
	float bestDistance = -1;

	//for storing position of best candidate
	float xBest = -1;
	float yBest = -1;
	float zBest = -1;

	//for tracking current index of the candidate we are comparing to
	int32 count = 0; 

	for(FVector candidatePosition: tempLocations)
	{
		//store array of distances from all points for every candidate
		TArray<float> distanceSet;

		float currentDistance = 0;

		//iterate over all past locations to get distances
		for(FVector pastVector: myBlueNoise.twoDLocations)
		{
			//add all distances between current candidate and past points to the set
			distanceSet.Add(FVector::Dist(candidatePosition, pastVector));
		}

		//get distance between current candidate and its closest neighbor from the set of existing points
		currentDistance = FMath::Min(distanceSet);

		if(currentDistance > bestDistance)
		{
			bestDistance = currentDistance;
			xBest = candidatePosition.X;
			yBest = candidatePosition.Y;
			zBest = candidatePosition.Z;
		}
		count++;
	}

	//empty the temporary set 
	tempLocations.Empty();

	//create best vector from extracted elements of the best candidate
	FVector positionVec(xBest,yBest,zBest);

	//add best vector to my struct
	myBlueNoise.twoDLocations.Add(positionVec);

	//return said position vector
	return positionVec;
	
}

//code to initialize type from random seed
void Uassn4PluginBPLibrary::SetBlueNoiseStreamSeed(UPARAM(ref) FRandomStream& Stream, int32 NewSeed)
{
	//initialize seed and add it to my struct
	Stream.Initialize(NewSeed);
	myBlueNoise.blueNoiseStream=Stream;
}

float Uassn4PluginBPLibrary::assn4PluginSampleFunction(float Param)
{
	return -1;
}

