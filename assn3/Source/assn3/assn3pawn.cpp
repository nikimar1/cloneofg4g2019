// Fill out your copyright notice in the Description page of Project Settings.

//#ifdef __clang__
//#pragma clang optimize off
//#else
//#pragma optimize("", off)
//#endif

#include "assn3pawn.h"

//includes for camera, springarm boom, collisions, player input, static mesh, etc...
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"

//adding this include for sphere component
#include "Components/SphereComponent.h"

//adding this include for GameplayStatistics.h in order to use GetActorArrayAverageLocation
#include "Kismet/GameplayStatics.h"

//included for power function
#include "GenericPlatform/GenericPlatformMath.h"

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
	Pawn->BodyInstance.MaxAngularVelocity = 400.0f;
	Pawn->SetNotifyRigidBodyCollision(true);
	RootComponent = Pawn;

	//create sphere for detecting neighbors
	NeighborSphere = CreateDefaultSubobject<USphereComponent>(TEXT("NeighborSphere"));
	NeighborSphere->SetSphereRadius(750);
	NeighborSphere->SetupAttachment(RootComponent); 
	//add overlap end and overlap begin events via AddDynamic 
	NeighborSphere->OnComponentEndOverlap.AddDynamic(this, &Aassn3pawn::OnOverlapEnd);
	NeighborSphere->OnComponentBeginOverlap.AddDynamic(this, &Aassn3pawn::OnOverlapBegin);

	// Create a camera boom attached to the root (pawn)
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bAbsoluteRotation = true; // Rotation of the ball should not affect rotation of boom
	SpringArm->RelativeRotation = FRotator(-45.f, 0.f, 0.f);
	SpringArm->TargetArmLength = 1700.f;
	SpringArm->bEnableCameraLag = false;
	SpringArm->CameraLagSpeed = 3.f;

	// Create a camera and attach to boom
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Set up scaling factors for cohesion, avoidance, and alignment as well as torque of player controlled pawn
	RollTorque = 15000000.0f;

	cohesionFactor = 60.0f;

	avoidanceFactor = 100000000.0f;


	//JumpImpulse = 350000.0f;
	//bCanJump = true; // Start being able to jump

 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void Aassn3pawn::BeginPlay()
{
	Super::BeginPlay();

	//initialize with overlapping actors
	NeighborSphere->GetOverlappingActors(neighbourList);
	
}

//Called every frame
void Aassn3pawn::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

	//get average location of all actors in the set
	FVector averageLoc = UGameplayStatics::GetActorArrayAverageLocation(neighbourList.Array());

	//if more than itself as a neighbor, calculate forces
	if(neighbourList.Num()>1)
	{

		//UE_LOG(LogTemp, Warning, TEXT( "This is the beginning of a loop adding impulses \n\n\n"));

	//for each actor in the set, apply the correct forces
		for(AActor* Actor : neighbourList)
		{

			//get static mesh of current actor in order to apply force to said mesh
			UStaticMeshComponent* tempMesh = Actor->FindComponentByClass<UStaticMeshComponent>();

			//get current location of chosen actor
			FVector myLoc =  Actor->GetActorLocation();

			//get cohesion vector from actor to average location
			FVector directionOfCohesion = (averageLoc - myLoc);

			//normalize it to length of one
			directionOfCohesion.Normalize();

			//multiply it by scaling factor for cohesion force
			directionOfCohesion *= cohesionFactor;

			//zero the z of the vector just in case
			directionOfCohesion.Z= 0; 

			//UE_LOG(LogTemp, Warning, TEXT( "directionOfCohesion added with values %s"), *directionOfCohesion.ToString());

			//add cohesion force as an impulse to the actor as long as it is not too low
			if(!(directionOfCohesion.Equals(FVector(0,0,0),1)))
				tempMesh->AddImpulse(directionOfCohesion);

			//add forces of repulsion/avoidance to current actor by adding impulse for each of its neighbors. 
			//This impulse is scaled by distance as well as a constant avoidance factor
			for(AActor* Actor2 : neighbourList)
			{
				//get location of some actor to compare to main actor loc
				FVector tempLoc = Actor2->GetActorLocation();

				//ignore the vector from an actor to itself
				if(tempLoc.Equals(myLoc, 0))
					continue;
				else
				{
					//find direction of vector originating from some actor in the set and pushing towards the main actor
					FVector directionOfAvoidance = (myLoc - tempLoc);

					//UE_LOG(LogTemp, Warning, TEXT( "directionOfAvoidance initially has values %s"), *directionOfAvoidance.ToString());

					//find the distance which will later be used as a factor on the force
					float distanceFactor = directionOfAvoidance.Size();

					//UE_LOG(LogTemp, Warning, TEXT( "distance was %f"), distanceFactor);

					//now I can normalize the vector to a unit vector of 1
					directionOfAvoidance.Normalize();

					//UE_LOG(LogTemp, Warning, TEXT( "directionOfAvoidance added was normalized to values %s"), *directionOfAvoidance.ToString());

					//float powerResult = FGenericPlatformMath::Pow(distanceFactor,3);

					//UE_LOG(LogTemp, Warning, TEXT( "Result of cubed distance was %f"), powerResult);

					//float powInverse = (1/powerResult);

					//UE_LOG(LogTemp, Warning, TEXT( "Result of 1/cubed distance distance was %f"), powInverse);

					//now I can multiply it by avoidanceFactor and 1/distance cubed
					directionOfAvoidance =(directionOfAvoidance * avoidanceFactor * (1/FGenericPlatformMath::Pow(distanceFactor,3)));

					//UE_LOG(LogTemp, Warning, TEXT( "pre z=0 directionOfAvoidance added with values %s"), *directionOfAvoidance.ToString());

					//just in case zero the z of the avoidance force vector
					directionOfAvoidance.Z = 0;


					//UE_LOG(LogTemp, Warning, TEXT( "Final directionOfAvoidance added with values %s"), *directionOfAvoidance.ToString());
					
					//add avoidance force as an impulse to the actor as long as it is not too low
					if(!(directionOfCohesion.Equals(FVector(0,0,0),.5)))
						tempMesh->AddImpulse(directionOfAvoidance);



				}
			}
		}
	}
	//UE_LOG(LogTemp, Warning, TEXT( "This has been one full loop of adding impulses \n\n\n"));
}

void Aassn3pawn::OnOverlapBegin(class UPrimitiveComponent* OverlappedComponent, AActor * OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	neighbourList.Add(OtherActor);
}

//on overlap end delegate function
void Aassn3pawn::OnOverlapEnd(class UPrimitiveComponent* OverlappedComponent, AActor * OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	neighbourList.Remove(OtherActor);
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

//#ifdef __clang__
//#pragma clang optimize on
//#else
//#pragma optimize("", on)
//#endif
