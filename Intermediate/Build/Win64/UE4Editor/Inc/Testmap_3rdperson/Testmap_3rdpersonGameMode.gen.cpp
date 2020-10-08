// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Testmap_3rdperson/Testmap_3rdpersonGameMode.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeTestmap_3rdpersonGameMode() {}
// Cross Module References
	TESTMAP_3RDPERSON_API UClass* Z_Construct_UClass_ATestmap_3rdpersonGameMode_NoRegister();
	TESTMAP_3RDPERSON_API UClass* Z_Construct_UClass_ATestmap_3rdpersonGameMode();
	ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
	UPackage* Z_Construct_UPackage__Script_Testmap_3rdperson();
// End Cross Module References
	void ATestmap_3rdpersonGameMode::StaticRegisterNativesATestmap_3rdpersonGameMode()
	{
	}
	UClass* Z_Construct_UClass_ATestmap_3rdpersonGameMode_NoRegister()
	{
		return ATestmap_3rdpersonGameMode::StaticClass();
	}
	struct Z_Construct_UClass_ATestmap_3rdpersonGameMode_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ATestmap_3rdpersonGameMode_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AGameModeBase,
		(UObject* (*)())Z_Construct_UPackage__Script_Testmap_3rdperson,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ATestmap_3rdpersonGameMode_Statics::Class_MetaDataParams[] = {
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering Utilities|Transformation" },
		{ "IncludePath", "Testmap_3rdpersonGameMode.h" },
		{ "ModuleRelativePath", "Testmap_3rdpersonGameMode.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_ATestmap_3rdpersonGameMode_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ATestmap_3rdpersonGameMode>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_ATestmap_3rdpersonGameMode_Statics::ClassParams = {
		&ATestmap_3rdpersonGameMode::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x008802ACu,
		METADATA_PARAMS(Z_Construct_UClass_ATestmap_3rdpersonGameMode_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_ATestmap_3rdpersonGameMode_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_ATestmap_3rdpersonGameMode()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_ATestmap_3rdpersonGameMode_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(ATestmap_3rdpersonGameMode, 1830531181);
	template<> TESTMAP_3RDPERSON_API UClass* StaticClass<ATestmap_3rdpersonGameMode>()
	{
		return ATestmap_3rdpersonGameMode::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_ATestmap_3rdpersonGameMode(Z_Construct_UClass_ATestmap_3rdpersonGameMode, &ATestmap_3rdpersonGameMode::StaticClass, TEXT("/Script/Testmap_3rdperson"), TEXT("ATestmap_3rdpersonGameMode"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(ATestmap_3rdpersonGameMode);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
