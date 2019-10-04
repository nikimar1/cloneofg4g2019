// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "assn3GameMode.h"
#include "assn3Ball.h"
#include "assn3pawn.h"

Aassn3GameMode::Aassn3GameMode()
{
	// set default pawn class to my pawn
	DefaultPawnClass = Aassn3pawn::StaticClass();
}
