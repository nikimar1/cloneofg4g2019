
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

    //for printing logs
    //DEFINE_LOG_CATEGORY_EXTERN(genLog, Log, All);

    // Sets default values for this actor's properties
    AAssn2Actor();
    
    //my overriden on construction function
    virtual void OnConstruction(const FTransform& transform) override;
    
    //for shuffling array of directions
    void shuffle( int array[], int size);
    
    //for swapping array order
    void swap(int *swapOne, int * swapTwo);
    
    //For checking if move is allowed
    bool allowedMove(int move, int startCoordX, int startCoordY);

    //void remove (int removed);


    //arraylist for holding every instances as it was added in order
    //keeps track of planes so they can be removed 
    //TArray<FVector> transformList;
    
    //structure to be used to find planes I can remove for maze.
    //commenting it out because it failed to work. using fvector2d to get x and y instead
    //struct matrixNode;
    
    //Structure for holding 2d array of visited nodes.
    //apparently 2d arrays don't work in ue4 though they work in c++
    //workaround is to make array of structs/
    //I will make that in the cpp file.
    //bool **visited;
    
    
    //for holding coordinates of 4 surrounding x,y coordinates of planes
    //FVector Above, Below, Left, Right;
    
    //function for finding plane to remove
    int FindPlane(FVector remove);
    
    //for building path/ maze randomly
    void BuildPath(int startCoordX, int startCoordY, FVector currentAbove, 
        FVector currentBelow, FVector currentLeft, FVector currentRight);
    
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

    /*
    //Creating 4 spheres for tracking overlap and position in maze
    UPROPERTY()
    class UStaticMeshComponent sphereAbove

    //Creating instanced static mesh component.
    UPROPERTY()
    class UStaticMeshComponent* sphereBelow;

    //Creating instanced static mesh component.
    UPROPERTY()
    class UStaticMeshComponent* sphereLeft;

    //Creating instanced static mesh component.
    UPROPERTY()
    class UStaticMeshComponent* sphereRight;

    */
    
    //for forcing rerun of on construction via boolean check mark
    UPROPERTY(EditAnywhere) bool Update;
    
    //for random seed generation
    UPROPERTY(EditAnywhere) int RandomSeed;
    
    
};

#ifdef __clang__
#pragma clang optimize on
#else
#pragma optimize("", on)
#endif