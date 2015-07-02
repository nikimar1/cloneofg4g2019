// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "EnginePrivate.h"
#include "AI/Navigation/NavAreas/NavArea.h"

UNavArea::UNavArea(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	DefaultCost = 1.f;
	FixedAreaEnteringCost = 0.f;
	DrawColor = FColor::Magenta;
	SupportedAgentsBits = 0xffffffff;
	// NOTE! AreaFlags == 0 means UNWALKABLE!
	AreaFlags = 1;  
}

void UNavArea::PostInitProperties()
{
	Super::PostInitProperties();
	
	if (HasAnyFlags(RF_ClassDefaultObject)
#if WITH_HOT_RELOAD
		&& !GIsHotReload
#endif // WITH_HOT_RELOAD
		)
	{
		UNavigationSystem::RequestAreaRegistering(GetClass());
	}
}

void UNavArea::FinishDestroy()
{
	if (HasAnyFlags(RF_ClassDefaultObject)
#if WITH_HOT_RELOAD
		&& !GIsHotReload
#endif // WITH_HOT_RELOAD
		)
	{
		UNavigationSystem::RequestAreaUnregistering(GetClass());
	}

	Super::FinishDestroy();
}

void UNavArea::PostLoad()
{
	Super::PostLoad();

	if (!SupportedAgents.IsInitialized())
	{
		SupportedAgents.bSupportsAgent0 = bSupportsAgent0;
		SupportedAgents.bSupportsAgent1 = bSupportsAgent1;
		SupportedAgents.bSupportsAgent2 = bSupportsAgent2;
		SupportedAgents.bSupportsAgent3 = bSupportsAgent3;
		SupportedAgents.bSupportsAgent4 = bSupportsAgent4;
		SupportedAgents.bSupportsAgent5 = bSupportsAgent5;
		SupportedAgents.bSupportsAgent6 = bSupportsAgent6;
		SupportedAgents.bSupportsAgent7 = bSupportsAgent7;
		SupportedAgents.bSupportsAgent8 = bSupportsAgent8;
		SupportedAgents.bSupportsAgent9 = bSupportsAgent9;
		SupportedAgents.bSupportsAgent10 = bSupportsAgent10;
		SupportedAgents.bSupportsAgent11 = bSupportsAgent11;
		SupportedAgents.bSupportsAgent12 = bSupportsAgent12;
		SupportedAgents.bSupportsAgent13 = bSupportsAgent13;
		SupportedAgents.bSupportsAgent14 = bSupportsAgent14;
		SupportedAgents.bSupportsAgent15 = bSupportsAgent15;
		SupportedAgents.MarkInitialized();
	}
}

FColor UNavArea::GetColor(UClass* AreaDefinitionClass)
{
	return AreaDefinitionClass ? AreaDefinitionClass->GetDefaultObject<UNavArea>()->DrawColor : FColor::Black;
}

void UNavArea::CopyFrom(TSubclassOf<UNavArea> AreaClass)
{
	if (AreaClass)
	{
		UNavArea* DefArea = (UNavArea*)AreaClass->GetDefaultObject();

		DefaultCost = DefArea->DefaultCost;
		FixedAreaEnteringCost = DefArea->GetFixedAreaEnteringCost();
		AreaFlags = DefArea->GetAreaFlags();
		DrawColor = DefArea->DrawColor;

		// don't copy supported agents bits
	}
}

#if WITH_EDITOR
void UNavArea::UpdateAgentConfig()
{
	// empty in base class
}
#endif