// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

/*my includes*/
#include "Math/RandomStream.h"
#include "Math/Vector.h"
#include "Containers/Set.h"
/**/

#include "assn4PluginBPLibrary.generated.h"

//my structure for blue noise
USTRUCT(BlueprintType)
struct FmyBlueNoiseStruct
{
	GENERATED_BODY()

	//set of generated 2d locations
	TSet<FVector> twoDLocations;
	
	//FRandomStream object
	FRandomStream blueNoiseStream;

	//default constructor 
	FmyBlueNoiseStruct(){}

};

/* 
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu. 
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/
UCLASS()
class Uassn4PluginBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	/** code to initialize my structure from a seed*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Blue Noise Stream Seed", Keywords = "Set Blue Noise Stream Seed"), Category = "assn4Plugin")
	static void SetBlueNoiseStreamSeed(UPARAM(ref) FRandomStream& Stream, int32 NewSeed);

	/** code to create blue noise in some range using stream*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Blue Noise Point in Range", Keywords = "Blue Noise Point in Range"), Category = "assn4Plugin")
	static FVector BlueNoisePointInRange(float Min, float Max, const FRandomStream& Stream);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Execute Sample function", Keywords = "assn4Plugin sample test testing"), Category = "assn4PluginTesting")
	static float assn4PluginSampleFunction(float Param);

};
