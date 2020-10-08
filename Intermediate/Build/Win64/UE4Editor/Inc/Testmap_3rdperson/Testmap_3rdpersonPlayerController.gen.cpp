// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Testmap_3rdperson/Testmap_3rdpersonPlayerController.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeTestmap_3rdpersonPlayerController() {}
// Cross Module References
	TESTMAP_3RDPERSON_API UClass* Z_Construct_UClass_ATestmap_3rdpersonPlayerController_NoRegister();
	TESTMAP_3RDPERSON_API UClass* Z_Construct_UClass_ATestmap_3rdpersonPlayerController();
	ENGINE_API UClass* Z_Construct_UClass_APlayerController();
	UPackage* Z_Construct_UPackage__Script_Testmap_3rdperson();
// End Cross Module References
	void ATestmap_3rdpersonPlayerController::StaticRegisterNativesATestmap_3rdpersonPlayerController()
	{
	}
	UClass* Z_Construct_UClass_ATestmap_3rdpersonPlayerController_NoRegister()
	{
		return ATestmap_3rdpersonPlayerController::StaticClass();
	}
	struct Z_Construct_UClass_ATestmap_3rdpersonPlayerController_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ATestmap_3rdpersonPlayerController_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_APlayerController,
		(UObject* (*)())Z_Construct_UPackage__Script_Testmap_3rdperson,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ATestmap_3rdpersonPlayerController_Statics::Class_MetaDataParams[] = {
		{ "HideCategories", "Collision Rendering Utilities|Transformation" },
		{ "IncludePath", "Testmap_3rdpersonPlayerController.h" },
		{ "ModuleRelativePath", "Testmap_3rdpersonPlayerController.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_ATestmap_3rdpersonPlayerController_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ATestmap_3rdpersonPlayerController>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_ATestmap_3rdpersonPlayerController_Statics::ClassParams = {
		&ATestmap_3rdpersonPlayerController::StaticClass,
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
		0x008002A4u,
		METADATA_PARAMS(Z_Construct_UClass_ATestmap_3rdpersonPlayerController_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_ATestmap_3rdpersonPlayerController_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_ATestmap_3rdpersonPlayerController()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_ATestmap_3rdpersonPlayerController_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(ATestmap_3rdpersonPlayerController, 3519869737);
	template<> TESTMAP_3RDPERSON_API UClass* StaticClass<ATestmap_3rdpersonPlayerController>()
	{
		return ATestmap_3rdpersonPlayerController::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_ATestmap_3rdpersonPlayerController(Z_Construct_UClass_ATestmap_3rdpersonPlayerController, &ATestmap_3rdpersonPlayerController::StaticClass, TEXT("/Script/Testmap_3rdperson"), TEXT("ATestmap_3rdpersonPlayerController"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(ATestmap_3rdpersonPlayerController);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
