// Fill out your copyright notice in the Description page of Project Settings.

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
	//was 400 then 800 now 600. back to 800
	Pawn->BodyInstance.MaxAngularVelocity = 800.0f;
	Pawn->SetNotifyRigidBodyCollision(true);
	RootComponent = Pawn;

	//create sphere for detecting neighbors
	NeighborSphere = CreateDefaultSubobject<USphereComponent>(TEXT("NeighborSphere"));
	//was 750 then 400 now trying 750 again. if bad will try something >400 <750. back to 400
	NeighborSphere->SetSphereRadius(400);
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
	RollTorque = 50000000.0f;

	cohesionFactor = 120.0f;

	avoidanceFactor = 500000000.0f;

	alignmentFactor= 15.0f;

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

	//for each actor in the set, apply the correct forces
		for(AActor* Actor : neighbourList)
		{
			//Vector for storing all avoidance vectors. I am adding them cumulatively
			//to the static mesh as a single impulse
			FVector repulsionVec(0,0,0);

			//Vector for storing average velocity of all other actors
			FVector avgVec(0,0,0);

			//count for storing number of other velocity vectors
			int32 count = 0;

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

			//add forces of repulsion/avoidance to current actor by adding impulse for each of its neighbors. 
			//This impulse is scaled by distance as well as a constant avoidance factor
			for(AActor* Actor2 : neighbourList)
			{
				//get location of some actor to compare to main actor loc
				FVector tempLoc = Actor2->GetActorLocation();

				//ignore the vector from an actor to itself
				if(tempLoc.Equals(myLoc))
					continue;
				else
				{

					//get this actor's velocity to add to average
					avgVec+=(Actor2->GetVelocity());

					//increment count;
					count++;

					//find direction of vector originating from some actor in the set and pushing towards the main actor
					FVector directionOfAvoidance = (myLoc - tempLoc);

					//find the distance which will later be used as a factor on the force
					float distanceFactor = directionOfAvoidance.Size();

					//now I can normalize the vector to a unit vector of 1
					directionOfAvoidance.Normalize();

					//now I can multiply it by avoidanceFactor and 1/distance cubed
					directionOfAvoidance =(directionOfAvoidance * avoidanceFactor * (1/FGenericPlatformMath::Pow(distanceFactor,3)));

					//just in case zero the z of the avoidance force vector
					directionOfAvoidance.Z = 0;
					
					//adding to net avoidance vector
					repulsionVec+= directionOfAvoidance;

				}
			}

			//add cohesion force as an impulse to the actor as long as mesh component is valid
            if (IsValid(tempMesh))
            {
                tempMesh->AddImpulse(directionOfCohesion);
                //UE_LOG(LogTemp, Warning, TEXT( "directionOfCohesion added with values %s"), *directionOfCohesion.ToString());
            }

            //add avoidance force as an impulse to the actor as long as mesh is valid
            if (IsValid(tempMesh))
            {
            	tempMesh->AddImpulse(repulsionVec);
            	//UE_LOG(LogTemp, Warning, TEXT( "avoidance vector added with values %s"), *repulsionVec.ToString());
            }

			//get difference of average velocity and this actor's velocity
			if(count>0)
				avgVec/=count; 
			FVector alignmentForce = (avgVec-(Actor->GetVelocity()));

			//multiply by scaled factor
			alignmentForce*=alignmentFactor;

			//remove z component
			alignmentForce.Z=0;

			//add impulse force for allignment to mesh if valid
            if (IsValid(tempMesh))
            {
                tempMesh->AddImpulse(alignmentForce);
            }
		}
	}
}


//on overlap begin delegate function
void Aassn3pawn::OnOverlapBegin(class UPrimitiveComponent* OverlappedComponent, AActor * OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	neighbourList.Add(OtherActor);
}

//on overlap end delegate function
void Aassn3pawn::OnOverlapEnd(class UPrimitiveComponent* OverlappedComponent, AActor * OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	neighbourList.Remove(OtherActor);
}

//adds torque to player controlled pawn
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

//Called to bind functionality to input
//I am using this to allow for player input rotational forces via arrow keys but not jumps or touch controls
void Aassn3pawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	PlayerInputComponent->BindAxis("MoveRight", this, &Aassn3pawn::MoveRight);
	PlayerInputComponent->BindAxis("MoveForward", this, &Aassn3pawn::MoveForward);

}