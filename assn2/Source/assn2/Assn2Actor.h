// Fill out your copyright notice in the Description page of Project Settings.
#ifdef __clang__
#pragma clang optimize off
#else
#pragma optimize("", off)
#endif

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Assn2Actor.generated.h"

UCLASS()
class ASSN2_API AAssn2Actor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAssn2Actor();

protected:

public:
	
};

#ifdef __clang__
#pragma clang optimize on
#else
#pragma optimize("", on)
#endif
