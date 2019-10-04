// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "assn3GameMode.h"
#include "assn3Ball.h"

Aassn3GameMode::Aassn3GameMode()
{
	// set default pawn class to our ball
	DefaultPawnClass = Aassn3Ball::StaticClass();
}
