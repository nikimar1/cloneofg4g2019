// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "assn4PluginBPLibrary.h"
#include "assn4Plugin.h"

Uassn4PluginBPLibrary::Uassn4PluginBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

//My code

//first just implementing function to work exactly like random noise. Later will act as below comment
//Code to return point as FVector
FVector Uassn4PluginBPLibrary::BlueNoisePointInRange(float Min, float Max, const FRandomStream& BlueNoiseStream)
{
	//get random values for x and y position. later add code to make this blue noise
	xValue= (Min + (Max - Min) * (BlueNoiseStream.FRand()));
	yValue= (Min + (Max - Min) * (BlueNoiseStream.FRand()));

	//construct position vector
	FVector positionVec(xValue,yValue,1)

	myBlueNoiseStruct.twoDLocations.Add(positionVec);

	return positionVec;
}

//code to initialize type from random seed
void Uassn4PluginBPLibrary::SetBlueNoiseStreamSeed(UPARAM(ref) FRandomStream& Stream, int32 NewSeed)
{
	Stream.Initialize(NewSeed);
	myBlueNoiseStruct.blueNoiseStream=Stream;
}

