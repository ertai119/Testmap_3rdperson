// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "ProceduralCave/Actor/RandomMapGenActor.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeRandomMapGenActor() {}
// Cross Module References
	PROCEDURALCAVE_API UClass* Z_Construct_UClass_ARandomMapGenActor_NoRegister();
	PROCEDURALCAVE_API UClass* Z_Construct_UClass_ARandomMapGenActor();
	ENGINE_API UClass* Z_Construct_UClass_AActor();
	UPackage* Z_Construct_UPackage__Script_ProceduralCave();
	ENGINE_API UClass* Z_Construct_UClass_UStaticMeshComponent_NoRegister();
	PROCEDURALMESHCOMPONENT_API UClass* Z_Construct_UClass_UProceduralMeshComponent_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_UMaterial_NoRegister();
// End Cross Module References
	void ARandomMapGenActor::StaticRegisterNativesARandomMapGenActor()
	{
	}
	UClass* Z_Construct_UClass_ARandomMapGenActor_NoRegister()
	{
		return ARandomMapGenActor::StaticClass();
	}
	struct Z_Construct_UClass_ARandomMapGenActor_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_spawnedStaticMeshComps__MetaData[];
#endif
		static const UE4CodeGen_Private::FMapPropertyParams NewProp_spawnedStaticMeshComps_;
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_spawnedStaticMeshComps__Key_KeyProp;
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_spawnedStaticMeshComps__ValueProp;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_proceduralMeshComp__MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_proceduralMeshComp_;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_BirthCount_MetaData[];
#endif
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_BirthCount;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_DeathCount_MetaData[];
#endif
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_DeathCount;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RoomThresholdAfterProcessed_MetaData[];
#endif
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_RoomThresholdAfterProcessed;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_WallThresholdAfterProcessed_MetaData[];
#endif
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_WallThresholdAfterProcessed;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_RoomThreshold_MetaData[];
#endif
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_RoomThreshold;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_WallThreshold_MetaData[];
#endif
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_WallThreshold;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SpawnProbability_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_SpawnProbability;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_PassageSize_MetaData[];
#endif
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_PassageSize;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_WallHeight_MetaData[];
#endif
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_WallHeight;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Height_MetaData[];
#endif
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_Height;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Width_MetaData[];
#endif
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_Width;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SquareSize_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_SquareSize;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_BorderSize_MetaData[];
#endif
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_BorderSize;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_SimulateCount_MetaData[];
#endif
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_SimulateCount;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Simulate_MetaData[];
#endif
		static void NewProp_Simulate_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_Simulate;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Generate_MetaData[];
#endif
		static void NewProp_Generate_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_Generate;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_TestMat_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_TestMat;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ARandomMapGenActor_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AActor,
		(UObject* (*)())Z_Construct_UPackage__Script_ProceduralCave,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ARandomMapGenActor_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "Actor/RandomMapGenActor.h" },
		{ "ModuleRelativePath", "Actor/RandomMapGenActor.h" },
	};
#endif
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_spawnedStaticMeshComps__MetaData[] = {
		{ "Category", "RandomMapGenActor" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Actor/RandomMapGenActor.h" },
	};
#endif
	const UE4CodeGen_Private::FMapPropertyParams Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_spawnedStaticMeshComps_ = { "spawnedStaticMeshComps_", nullptr, (EPropertyFlags)0x0020088000000009, UE4CodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ARandomMapGenActor, spawnedStaticMeshComps_), EMapPropertyFlags::None, METADATA_PARAMS(Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_spawnedStaticMeshComps__MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_spawnedStaticMeshComps__MetaData)) };
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_spawnedStaticMeshComps__Key_KeyProp = { "spawnedStaticMeshComps__Key", nullptr, (EPropertyFlags)0x0000000000080009, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_spawnedStaticMeshComps__ValueProp = { "spawnedStaticMeshComps_", nullptr, (EPropertyFlags)0x0000000000080009, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, 1, Z_Construct_UClass_UStaticMeshComponent_NoRegister, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_proceduralMeshComp__MetaData[] = {
		{ "Category", "RandomMapGenActor" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Actor/RandomMapGenActor.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_proceduralMeshComp_ = { "proceduralMeshComp_", nullptr, (EPropertyFlags)0x0020080000080009, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ARandomMapGenActor, proceduralMeshComp_), Z_Construct_UClass_UProceduralMeshComponent_NoRegister, METADATA_PARAMS(Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_proceduralMeshComp__MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_proceduralMeshComp__MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_BirthCount_MetaData[] = {
		{ "Category", "RandomMapGenActor" },
		{ "ModuleRelativePath", "Actor/RandomMapGenActor.h" },
	};
#endif
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_BirthCount = { "BirthCount", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ARandomMapGenActor, BirthCount), METADATA_PARAMS(Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_BirthCount_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_BirthCount_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_DeathCount_MetaData[] = {
		{ "Category", "RandomMapGenActor" },
		{ "ModuleRelativePath", "Actor/RandomMapGenActor.h" },
	};
#endif
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_DeathCount = { "DeathCount", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ARandomMapGenActor, DeathCount), METADATA_PARAMS(Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_DeathCount_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_DeathCount_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_RoomThresholdAfterProcessed_MetaData[] = {
		{ "Category", "RandomMapGenActor" },
		{ "ModuleRelativePath", "Actor/RandomMapGenActor.h" },
	};
#endif
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_RoomThresholdAfterProcessed = { "RoomThresholdAfterProcessed", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ARandomMapGenActor, RoomThresholdAfterProcessed), METADATA_PARAMS(Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_RoomThresholdAfterProcessed_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_RoomThresholdAfterProcessed_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_WallThresholdAfterProcessed_MetaData[] = {
		{ "Category", "RandomMapGenActor" },
		{ "ModuleRelativePath", "Actor/RandomMapGenActor.h" },
	};
#endif
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_WallThresholdAfterProcessed = { "WallThresholdAfterProcessed", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ARandomMapGenActor, WallThresholdAfterProcessed), METADATA_PARAMS(Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_WallThresholdAfterProcessed_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_WallThresholdAfterProcessed_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_RoomThreshold_MetaData[] = {
		{ "Category", "RandomMapGenActor" },
		{ "ModuleRelativePath", "Actor/RandomMapGenActor.h" },
	};
#endif
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_RoomThreshold = { "RoomThreshold", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ARandomMapGenActor, RoomThreshold), METADATA_PARAMS(Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_RoomThreshold_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_RoomThreshold_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_WallThreshold_MetaData[] = {
		{ "Category", "RandomMapGenActor" },
		{ "ModuleRelativePath", "Actor/RandomMapGenActor.h" },
	};
#endif
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_WallThreshold = { "WallThreshold", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ARandomMapGenActor, WallThreshold), METADATA_PARAMS(Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_WallThreshold_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_WallThreshold_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_SpawnProbability_MetaData[] = {
		{ "Category", "RandomMapGenActor" },
		{ "ModuleRelativePath", "Actor/RandomMapGenActor.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_SpawnProbability = { "SpawnProbability", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ARandomMapGenActor, SpawnProbability), METADATA_PARAMS(Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_SpawnProbability_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_SpawnProbability_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_PassageSize_MetaData[] = {
		{ "Category", "RandomMapGenActor" },
		{ "ModuleRelativePath", "Actor/RandomMapGenActor.h" },
	};
#endif
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_PassageSize = { "PassageSize", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ARandomMapGenActor, PassageSize), METADATA_PARAMS(Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_PassageSize_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_PassageSize_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_WallHeight_MetaData[] = {
		{ "Category", "RandomMapGenActor" },
		{ "ModuleRelativePath", "Actor/RandomMapGenActor.h" },
	};
#endif
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_WallHeight = { "WallHeight", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ARandomMapGenActor, WallHeight), METADATA_PARAMS(Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_WallHeight_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_WallHeight_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_Height_MetaData[] = {
		{ "Category", "RandomMapGenActor" },
		{ "ModuleRelativePath", "Actor/RandomMapGenActor.h" },
	};
#endif
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_Height = { "Height", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ARandomMapGenActor, Height), METADATA_PARAMS(Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_Height_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_Height_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_Width_MetaData[] = {
		{ "Category", "RandomMapGenActor" },
		{ "ModuleRelativePath", "Actor/RandomMapGenActor.h" },
	};
#endif
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_Width = { "Width", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ARandomMapGenActor, Width), METADATA_PARAMS(Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_Width_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_Width_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_SquareSize_MetaData[] = {
		{ "Category", "RandomMapGenActor" },
		{ "ModuleRelativePath", "Actor/RandomMapGenActor.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_SquareSize = { "SquareSize", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ARandomMapGenActor, SquareSize), METADATA_PARAMS(Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_SquareSize_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_SquareSize_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_BorderSize_MetaData[] = {
		{ "Category", "RandomMapGenActor" },
		{ "ModuleRelativePath", "Actor/RandomMapGenActor.h" },
	};
#endif
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_BorderSize = { "BorderSize", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ARandomMapGenActor, BorderSize), METADATA_PARAMS(Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_BorderSize_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_BorderSize_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_SimulateCount_MetaData[] = {
		{ "Category", "RandomMapGenActor" },
		{ "ModuleRelativePath", "Actor/RandomMapGenActor.h" },
	};
#endif
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_SimulateCount = { "SimulateCount", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ARandomMapGenActor, SimulateCount), METADATA_PARAMS(Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_SimulateCount_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_SimulateCount_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_Simulate_MetaData[] = {
		{ "Category", "RandomMapGenActor" },
		{ "ModuleRelativePath", "Actor/RandomMapGenActor.h" },
	};
#endif
	void Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_Simulate_SetBit(void* Obj)
	{
		((ARandomMapGenActor*)Obj)->Simulate = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_Simulate = { "Simulate", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(ARandomMapGenActor), &Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_Simulate_SetBit, METADATA_PARAMS(Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_Simulate_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_Simulate_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_Generate_MetaData[] = {
		{ "Category", "RandomMapGenActor" },
		{ "ModuleRelativePath", "Actor/RandomMapGenActor.h" },
	};
#endif
	void Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_Generate_SetBit(void* Obj)
	{
		((ARandomMapGenActor*)Obj)->Generate = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_Generate = { "Generate", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(ARandomMapGenActor), &Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_Generate_SetBit, METADATA_PARAMS(Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_Generate_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_Generate_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_TestMat_MetaData[] = {
		{ "Category", "RandomMapGenActor" },
		{ "ModuleRelativePath", "Actor/RandomMapGenActor.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_TestMat = { "TestMat", nullptr, (EPropertyFlags)0x0010000000000001, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(ARandomMapGenActor, TestMat), Z_Construct_UClass_UMaterial_NoRegister, METADATA_PARAMS(Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_TestMat_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_TestMat_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_ARandomMapGenActor_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_spawnedStaticMeshComps_,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_spawnedStaticMeshComps__Key_KeyProp,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_spawnedStaticMeshComps__ValueProp,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_proceduralMeshComp_,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_BirthCount,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_DeathCount,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_RoomThresholdAfterProcessed,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_WallThresholdAfterProcessed,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_RoomThreshold,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_WallThreshold,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_SpawnProbability,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_PassageSize,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_WallHeight,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_Height,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_Width,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_SquareSize,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_BorderSize,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_SimulateCount,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_Simulate,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_Generate,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ARandomMapGenActor_Statics::NewProp_TestMat,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_ARandomMapGenActor_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ARandomMapGenActor>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_ARandomMapGenActor_Statics::ClassParams = {
		&ARandomMapGenActor::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_ARandomMapGenActor_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_ARandomMapGenActor_Statics::PropPointers),
		0,
		0x008000A4u,
		METADATA_PARAMS(Z_Construct_UClass_ARandomMapGenActor_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_ARandomMapGenActor_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_ARandomMapGenActor()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_ARandomMapGenActor_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(ARandomMapGenActor, 3980674244);
	template<> PROCEDURALCAVE_API UClass* StaticClass<ARandomMapGenActor>()
	{
		return ARandomMapGenActor::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_ARandomMapGenActor(Z_Construct_UClass_ARandomMapGenActor, &ARandomMapGenActor::StaticClass, TEXT("/Script/ProceduralCave"), TEXT("ARandomMapGenActor"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(ARandomMapGenActor);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
