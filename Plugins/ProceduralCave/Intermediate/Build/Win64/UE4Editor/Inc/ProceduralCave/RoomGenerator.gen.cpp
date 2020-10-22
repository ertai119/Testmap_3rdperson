// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "ProceduralCave/Generator/RoomGenerator.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeRoomGenerator() {}
// Cross Module References
	PROCEDURALCAVE_API UClass* Z_Construct_UClass_URoomGenerator_NoRegister();
	PROCEDURALCAVE_API UClass* Z_Construct_UClass_URoomGenerator();
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
	UPackage* Z_Construct_UPackage__Script_ProceduralCave();
// End Cross Module References
	void URoomGenerator::StaticRegisterNativesURoomGenerator()
	{
	}
	UClass* Z_Construct_UClass_URoomGenerator_NoRegister()
	{
		return URoomGenerator::StaticClass();
	}
	struct Z_Construct_UClass_URoomGenerator_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_URoomGenerator_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UObject,
		(UObject* (*)())Z_Construct_UPackage__Script_ProceduralCave,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_URoomGenerator_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "Generator/RoomGenerator.h" },
		{ "ModuleRelativePath", "Generator/RoomGenerator.h" },
		{ "ObjectInitializerConstructorDeclared", "" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_URoomGenerator_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<URoomGenerator>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_URoomGenerator_Statics::ClassParams = {
		&URoomGenerator::StaticClass,
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
		METADATA_PARAMS(Z_Construct_UClass_URoomGenerator_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_URoomGenerator_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_URoomGenerator()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_URoomGenerator_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(URoomGenerator, 1952966068);
	template<> PROCEDURALCAVE_API UClass* StaticClass<URoomGenerator>()
	{
		return URoomGenerator::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_URoomGenerator(Z_Construct_UClass_URoomGenerator, &URoomGenerator::StaticClass, TEXT("/Script/ProceduralCave"), TEXT("URoomGenerator"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(URoomGenerator);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
