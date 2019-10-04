// Fill out your copyright notice in the Description page of Project Settings.


#include "assn3pawn.h"


// Sets default values
Aassn3pawn::Aassn3pawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

