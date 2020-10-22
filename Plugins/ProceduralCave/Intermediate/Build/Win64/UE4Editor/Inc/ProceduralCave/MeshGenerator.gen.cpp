// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "ProceduralCave/Generator/MeshGenerator.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeMeshGenerator() {}
// Cross Module References
	PROCEDURALCAVE_API UClass* Z_Construct_UClass_UMeshGenerator_NoRegister();
	PROCEDURALCAVE_API UClass* Z_Construct_UClass_UMeshGenerator();
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
	UPackage* Z_Construct_UPackage__Script_ProceduralCave();
// End Cross Module References
	void UMeshGenerator::StaticRegisterNativesUMeshGenerator()
	{
	}
	UClass* Z_Construct_UClass_UMeshGenerator_NoRegister()
	{
		return UMeshGenerator::StaticClass();
	}
	struct Z_Construct_UClass_UMeshGenerator_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UMeshGenerator_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UObject,
		(UObject* (*)())Z_Construct_UPackage__Script_ProceduralCave,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UMeshGenerator_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "Generator/MeshGenerator.h" },
		{ "ModuleRelativePath", "Generator/MeshGenerator.h" },
		{ "ObjectInitializerConstructorDeclared", "" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UMeshGenerator_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UMeshGenerator>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UMeshGenerator_Statics::ClassParams = {
		&UMeshGenerator::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x000000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UMeshGenerator_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UMeshGenerator_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UMeshGenerator()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UMeshGenerator_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UMeshGenerator, 3101896943);
	template<> PROCEDURALCAVE_API UClass* StaticClass<UMeshGenerator>()
	{
		return UMeshGenerator::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UMeshGenerator(Z_Construct_UClass_UMeshGenerator, &UMeshGenerator::StaticClass, TEXT("/Script/ProceduralCave"), TEXT("UMeshGenerator"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UMeshGenerator);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
