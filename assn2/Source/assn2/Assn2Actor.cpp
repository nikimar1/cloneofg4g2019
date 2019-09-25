// Fill out your copyright notice in the Description page of Project Settings.
#ifdef __clang__
#pragma clang optimize off
#else
#pragma optimize("", off)
#endif

#include "Assn2Actor.h"


// Sets default values
AAssn2Actor::AAssn2Actor()
{
    
    //created USceneComponent for Actor Placement
    const bool bTransient = true;
    USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"), bTransient);
    check(SceneComponent != nullptr);
    this->RootComponent = SceneComponent;
    
    
    //created static mesh component
    StaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("MeshComponent"));
    StaticMeshComponent->SetupAttachment(RootComponent);

}

//Creating a matrix node struct for holding all coordinates of 5x5 grid
//For some reason could not place this in header. Type was not recognized
typedef struct myMatrixNode
{
    
    //rotation
    float pitch;
    float roll;
    
    //translation
    float x;
    float y;
    
}myMatrixNode;

//created array of matrix node strucs via initialization list
myMatrixNode matrixArray[60] = {{90,0,50,0},{90,0,-50,0},{90,90,0,-50},{90,90,0,50},{90,0,-150,0},{90,90,-100,-50},{90,90,-100,50},{90,0,-250,0},{90,90,-200,-50},{90,90,-200,50},{90,0,150,0},{90,90,100,-50},{90,90,100,50},{90,0,250,0},{90,90,200,-50},{90,90,200,50},{90,0,50,-100},{90,0,-50,-100},{90,90,0,-150},{90,0,-150,-100},{90,90,-100,-150},{90,0,-250,-100},{90,90,-200,-150},{90,0,150,-100},{90,90,100,-150},{90,0,250,-100},{90,90,200,-150},{90,0,50,-200},{90,0,-50,-200},{90,90,0,-250},{90,0,-150,-200},{90,90,-100,-250},{90,0,-250,-200},{90,90,-200,-250},{90,0,150,-200},{90,90,100,-250},{90,0,250,-200},{90,90,200,-250},{90,0,50,100},{90,0,-50,100},{90,90,0,150},{90,0,-150,100},{90,90,-100,150},{90,0,-250,100},{90,90,-200,150},{90,0,150,100},{90,90,100,150},{90,0,250,100},{90,90,200,150},{90,0,50,200},{90,0,-50,200},{90,90,0,250},{90,0,-150,200},{90,90,-100,250},{90,0,-250,200},{90,90,-200,250},{90,0,150,200},{90,90,100,250},{90,0,250,200},{90,90,200,250}};

//on construction overriden function
void AAssn2Actor::OnConstruction(const FTransform& transform)
{
    Super::OnConstruction(transform);
    
    //clearing instances to reset tarray of instances at start of onconstruction
    StaticMeshComponent->ClearInstances();
    
    //I am now using a for loop to create my 5x5 grid from the matrix array
    
    for(int i = 0; i <60; i++)
    {
        //rotation
        float pitch = matrixArray[i].pitch;
        float roll = matrixArray[i].roll;
        
        //translation
        float x = matrixArray[i].x;
        float y = matrixArray[i].y;
        StaticMeshComponent->AddInstance(FTransform(FRotator(pitch, roll, 0), FVector(x, y, 0)));
    }
    
    //using member variable to set mesh
    StaticMeshComponent->SetStaticMesh(ActorMesh);
    
    //using setmaterial and our member variable
    StaticMeshComponent->SetMaterial(0, ActorMaterial);
    
   
    
}

#ifdef __clang__
#pragma clang optimize on
#else
#pragma optimize("", on)
#endif
