// Fill out your copyright notice in the Description page of Project Settings.


#include "assn5LoadFactory.h"

#include "Engine/StaticMesh.h"
#include "Misc/FeedbackContext.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

#include "AssetRegistryModule.h"
#include "Editor/UnrealEd/Public/Editor.h"
#include "Engine/StaticMesh.h"
//#include "IMessageLogListing.h"
//#include "Interfaces/IMainFrameModule.h"
#include "Logging/LogMacros.h"
#include "Logging/TokenizedMessage.h"
#include "Materials/Material.h"
//#include "MessageLogModule.h"
#include "PackageTools.h"
#include "UObject/StrongObjectPtr.h"

#define LOCTEXT_NAMESPACE "assn5LoadFactory"

Uassn5LoadFactory::Uassn5LoadFactory(const FObjectInitializer& ObjectInitializer)
{
	Formats.Add(TEXT("mha;mha format"));
	Formats.Add(TEXT("mhd;mhd format"));
}

UObject * Uassn5LoadFactory::FactoryCreateFile(UClass * InClass, UObject * InParent, FName InName, EObjectFlags Flags, const FString & Filename, const TCHAR * Parms, FFeedbackContext * Warn, bool & bOutOperationCanceled)
{
	/*
	GEditor->GetEditorSubsystem<UImportSubsystem>()->BroadcastAssetPreImport(this, InClass, InParent, InName, Parms);

	Warn->Log(Filename);

	//TStrongObjectPtr<UGLTFImportOptions> ImporterOptions(NewObject<UGLTFImportOptions>(GetTransientPackage(), TEXT("GLTF Importer Options")));
	// show import options window
	{
		const FString Filepath = FPaths::ConvertRelativePathToFull(Filename);
		const FString PackagePath = InParent->GetPathName();

		//if (!GLTFImporterImpl::ShowOptionsWindow(Filepath, PackagePath, *ImporterOptions))
		//{
		//	bOutOperationCanceled = true;
		//	return nullptr;
		//}
	}

	//FGLTFImporterContext& Context = GLTFImporterModule->GetImporterContext();
	// setup import options
	{
		//Context.StaticMeshFactory.SetUniformScale(ImporterOptions->ImportScale);
		//Context.StaticMeshFactory.SetGenerateLightmapUVs(ImporterOptions->bGenerateLightmapUVs);
	}

	UObject* Object = nullptr;
	/*if (Context.OpenFile(Filename))
	{
		const FString AssetName = Context.Asset.Name;
		const FString NewPackageName = UPackageTools::SanitizePackageName(*(FPaths::GetPath(InParent->GetName()) / AssetName));
		UObject*      ParentPackage = NewPackageName == InParent->GetName() ? InParent : CreatePackage(nullptr, *NewPackageName);

		const TArray<UStaticMesh*>& CreatedMeshes = Context.CreateMeshes(ParentPackage, Flags, false);
		Context.CreateMaterials(ParentPackage, Flags);
		UpdateMeshes();

		if (CreatedMeshes.Num() == 1)
		{
			Object = CreatedMeshes[0];
		}
		else if (CreatedMeshes.Num() != 0)
		{
			Object = CreatedMeshes[0]->GetOutermost();
		}
	}
	*/
	/**
	GEditor->GetEditorSubsystem<UImportSubsystem>()->BroadcastAssetPostImport(this, Object);

	//GLTFImporterImpl::ShowLogMessages(Context.GetLogMessages());

	return Object;
	*/

	return nullptr;
}
