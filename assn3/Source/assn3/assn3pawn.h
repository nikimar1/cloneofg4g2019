// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "UObject/ConstructorHelpers.h"

#include "assn3pawn.generated.h"



UCLASS()
class ASSN3_API Aassn3pawn : public APawn
{
	GENERATED_BODY()

	/** Spring arm for positioning the camera above the ball */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	/** Camera to view the ball */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	/** StaticMesh used for the pawn */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Pawn;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* NeighborSphere;	

public:
	// Sets default values for this pawn's properties
	Aassn3pawn();

	//using torque force for controllable ball movement
	float RollTorque;

	//constant scaling factor for cohesion force
	float cohesionFactor;

	//constant scaling factor for avoidance force
	float avoidanceFactor;

	//This is the set for neighbouring actors
	//making this visible so I can tell as neighbours are added or removed
	//UPROPERTY(Transient, VisibleAnywhere)
	TSet<AActor*> neighbourList;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called for side to side input */
	void MoveRight(float Val);

	/** Called to move ball forwards and backwards */
	void MoveForward(float Val);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	//on overlap begin delegate function
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComponent, AActor * OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	//on overlap end delegate function
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComponent, AActor * OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Returns pawn subobject **/
	FORCEINLINE class UStaticMeshComponent* GetPawn() const { return Pawn; }
	/** Returns SpringArm subobject **/
	FORCEINLINE class USpringArmComponent* GetSpringArm() const { return SpringArm; }
	/** Returns Camera subobject **/
	FORCEINLINE class UCameraComponent* GetCamera() const { return Camera; }
	
};
