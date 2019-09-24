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
    
    //my overriden on construction function
    virtual void OnConstruction(const FTransform& transform) override;

    
//protected:

//public:
    
//start of my code
private:
    //Creating static mesh component. Leaving uproperty for now but later maybe remove it
    UPROPERTY()
    class UStaticMeshComponent* StaticMeshComponent;
    
    UPROPERTY(EditAnywhere)
    UStaticMesh* ActorMesh;
    
    UPROPERTY(EditAnywhere)
    UMaterial* ActorMaterial;
    
    //Scene component. I believe I am actually setting another scene component
    //to root in cpp file and do not need this code.
    //UPROPERTY()
    //USceneComponent* AssociatedComponent;

	
};

#ifdef __clang__
#pragma clang optimize on
#else
#pragma optimize("", on)
#endif
