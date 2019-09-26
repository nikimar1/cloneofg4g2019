// Fill out your copyright notice in the Description page of Project Settings.
#ifdef __clang__
#pragma clang optimize off
#else
#pragma optimize("", off)
#endif

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//adding this include for instanced static mesh component
#include "Components/InstancedStaticMeshComponent.h"
//adding include for frandomstream
#include "Math/RandomStream.h"

#include "Assn2Actor.generated.h"



UCLASS()
class ASSN2_API AAssn2Actor : public AActor
{
	GENERATED_BODY()
    
	
public:	
	// Sets default values for this actor's properties
	AAssn2Actor();
    
    //my overriden on construction function
    virtual void OnConstruction(const FTransform& transform) override;
    
    //for shuffling array of directions
    void shuffle( int array[], int size);
    
    //for swapping array order 
    void swap(int *swapOne, int * swapTwo);
    
    //For checking if move is allowed
    bool allowedMove(int move);
    
    //for building path/ maze randomly
    void BuildPath(int startCoordX, int startCoordY);
    

private:
    
    //mesh type
    UPROPERTY(EditAnywhere)
    UStaticMesh* ActorMesh;
    
    //material type
    UPROPERTY(EditAnywhere)
    UMaterial* ActorMaterial;
    
    //Creating instanced static mesh component.
    UPROPERTY()
    class UInstancedStaticMeshComponent* StaticMeshComponent;
	
};

#ifdef __clang__
#pragma clang optimize on
#else
#pragma optimize("", on)
#endif
