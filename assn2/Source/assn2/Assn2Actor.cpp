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
    //commenting out stuff I don't need
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
    
    //my code below here
    
    //created static mesh as sphere via FObjectFinder
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshRef(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
    UStaticMesh* StaticMesh = MeshRef.Object;
    check(StaticMesh != nullptr);
    
    //created USceneComponent for Actor Placement
    const bool bTransient = true;
    USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"), bTransient);
    check(SceneComponent != nullptr);
    this->RootComponent = SceneComponent;
    
    
    //on construction overriden function
    
    
    
    //created static mesh component
    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyMesh"));
    StaticMeshComponent->SetMobility(EComponentMobility::Movable);
    StaticMeshComponent->SetupAttachment(RootComponent);
    StaticMeshComponent->SetStaticMesh(StaticMesh);
    //using setmaterial and our member variable instead
    StaticMeshComponent->SetMaterial(0, ActorMaterial);
    
    StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
    StaticMeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
    
    StaticMeshComponent->SetGenerateOverlapEvents(false);
    StaticMeshComponent->SetCanEverAffectNavigation(false);
    StaticMeshComponent->bCastDynamicShadow = false;
    StaticMeshComponent->bCastStaticShadow = false;
    StaticMeshComponent->bAffectDistanceFieldLighting = false;
    StaticMeshComponent->bAffectDynamicIndirectLighting = false;
    
    

}

//commenting out stuff I don't need
// Called when the game starts or when spawned
//void AAssn2Actor::BeginPlay()
//{
//	Super::BeginPlay();
	
//}

//// Called every frame
//void AAssn2Actor::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);

//}


#ifdef __clang__
#pragma clang optimize on
#else
#pragma optimize("", on)
#endif
