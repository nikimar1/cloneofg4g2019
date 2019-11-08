// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "assn5LoadFactory.generated.h"

/**
 * 
 */
UCLASS(transient)
class Uassn5LoadFactory : public UFactory
{
	GENERATED_UCLASS_BODY()

public:

	//Uassn5LoadFactory::Uassn5LoadFactory(const FObjectInitializer& ObjectInitializer);

	virtual UObject* FactoryCreateFile(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, const FString& Filename,
		const TCHAR* Parms, FFeedbackContext* Warn, bool& bOutOperationCanceled) override;

private:
	
};
