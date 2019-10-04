// Fill out your copyright notice in the Description page of Project Settings.


#include "assn3pawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
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
	RootComponent = Pawn;

	//create sphere for detecting neighbors
	NeighborSphere = CreateDefaultSubobject<USphereComponent>(TEXT("NeighborSphere"));
	NeighborSphere->SetSphereRadius(750);
	NeighborSphere->SetupAttachment(RootComponent); 

	// Create a camera boom attached to the root (pawn)
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bAbsoluteRotation = true; // Rotation of the ball should not affect rotation of boom
	SpringArm->RelativeRotation = FRotator(-45.f, 0.f, 0.f);
	SpringArm->TargetArmLength = 1200.f;
	SpringArm->bEnableCameraLag = false;
	SpringArm->CameraLagSpeed = 3.f;

	// Create a camera and attach to boom
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	/*
	Later I will be using add impulse instead of addtorqueinradians. Later I have to change the move functions
	moveforward and moveright as well as picking different constants for forces. 
	*/
	// Set up forces
	RollTorque = 50000000.0f;
	//JumpImpulse = 350000.0f;
	//bCanJump = true; // Start being able to jump

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

//remember to rewrite these two to add impulse not add torque 
void Aassn3pawn::MoveRight(float Val)
{
	const FVector Torque = FVector(-1.f * Val * RollTorque, 0.f, 0.f);
	Pawn->AddTorqueInRadians(Torque);
}

void Aassn3pawn::MoveForward(float Val)
{
	const FVector Torque = FVector(0.f, Val * RollTorque, 0.f);
	Pawn->AddTorqueInRadians(Torque);	
}
//remember to rewrite two above functions

// Called to bind functionality to input
//I am using this to allow for player input
void Aassn3pawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	PlayerInputComponent->BindAxis("MoveRight", this, &Aassn3pawn::MoveRight);
	PlayerInputComponent->BindAxis("MoveForward", this, &Aassn3pawn::MoveForward);

	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &Aassn3Ball::Jump);

	// handle touch devices
	//PlayerInputComponent->BindTouch(IE_Pressed, this, &Aassn3Ball::TouchStarted);
	//PlayerInputComponent->BindTouch(IE_Released, this, &Aassn3Ball::TouchStopped);
}

/*
void Aassn3pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
*/

