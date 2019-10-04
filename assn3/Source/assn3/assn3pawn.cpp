// Fill out your copyright notice in the Description page of Project Settings.


#include "assn3pawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
//adding this include for sphere component
#include "Components/SphereComponent.h"


// Sets default values
Aassn3pawn::Aassn3pawn()
{

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BallMesh(TEXT("/Game/Rolling/Meshes/BallMesh.BallMesh"));

	// Create mesh component for the pawn
	Pawn = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyPawn"));
	Pawn->SetStaticMesh(BallMesh.Object);
	Pawn->BodyInstance.SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	Pawn->SetSimulatePhysics(true);
	Pawn->SetAngularDamping(0.1f);
	Pawn->SetLinearDamping(0.1f);
	Pawn->BodyInstance.MassScale = 3.5f;
	Pawn->BodyInstance.MaxAngularVelocity = 800.0f;
	Pawn->SetNotifyRigidBodyCollision(true);
	this->RootComponent = Pawn;

	//create sphere for detecting neighbors
	NeighborSphere = CreateDefaultSubobject<USphereComponent>(TEXT("NeighborSphere"));
	NeighborSphere->SetSphereRadius(750);
	NeighborSphere->SetupAttachment(RootComponent); 

	//maybe don't need these. not sure

	// Set up forces
	/*
	RollTorque = 50000000.0f;
	JumpImpulse = 350000.0f;
	bCanJump = true; // Start being able to jump
	*/

 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void Aassn3pawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void Aassn3pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void Aassn3pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

