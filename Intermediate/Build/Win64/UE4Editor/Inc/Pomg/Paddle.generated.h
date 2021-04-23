// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef POMG_Paddle_generated_h
#error "Paddle.generated.h already included, missing '#pragma once' in Paddle.h"
#endif
#define POMG_Paddle_generated_h

#define Pomg_Source_Pomg_Public_Paddle_h_15_SPARSE_DATA
#define Pomg_Source_Pomg_Public_Paddle_h_15_RPC_WRAPPERS
#define Pomg_Source_Pomg_Public_Paddle_h_15_RPC_WRAPPERS_NO_PURE_DECLS
#define Pomg_Source_Pomg_Public_Paddle_h_15_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAPaddle(); \
	friend struct Z_Construct_UClass_APaddle_Statics; \
public: \
	DECLARE_CLASS(APaddle, APawn, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/Pomg"), NO_API) \
	DECLARE_SERIALIZER(APaddle)


#define Pomg_Source_Pomg_Public_Paddle_h_15_INCLASS \
private: \
	static void StaticRegisterNativesAPaddle(); \
	friend struct Z_Construct_UClass_APaddle_Statics; \
public: \
	DECLARE_CLASS(APaddle, APawn, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/Pomg"), NO_API) \
	DECLARE_SERIALIZER(APaddle)


#define Pomg_Source_Pomg_Public_Paddle_h_15_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API APaddle(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(APaddle) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, APaddle); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(APaddle); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API APaddle(APaddle&&); \
	NO_API APaddle(const APaddle&); \
public:


#define Pomg_Source_Pomg_Public_Paddle_h_15_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API APaddle(APaddle&&); \
	NO_API APaddle(const APaddle&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, APaddle); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(APaddle); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(APaddle)


#define Pomg_Source_Pomg_Public_Paddle_h_15_PRIVATE_PROPERTY_OFFSET \
	FORCEINLINE static uint32 __PPO__CollisionComponent() { return STRUCT_OFFSET(APaddle, CollisionComponent); } \
	FORCEINLINE static uint32 __PPO__VisualComponent() { return STRUCT_OFFSET(APaddle, VisualComponent); }


#define Pomg_Source_Pomg_Public_Paddle_h_12_PROLOG
#define Pomg_Source_Pomg_Public_Paddle_h_15_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	Pomg_Source_Pomg_Public_Paddle_h_15_PRIVATE_PROPERTY_OFFSET \
	Pomg_Source_Pomg_Public_Paddle_h_15_SPARSE_DATA \
	Pomg_Source_Pomg_Public_Paddle_h_15_RPC_WRAPPERS \
	Pomg_Source_Pomg_Public_Paddle_h_15_INCLASS \
	Pomg_Source_Pomg_Public_Paddle_h_15_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define Pomg_Source_Pomg_Public_Paddle_h_15_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	Pomg_Source_Pomg_Public_Paddle_h_15_PRIVATE_PROPERTY_OFFSET \
	Pomg_Source_Pomg_Public_Paddle_h_15_SPARSE_DATA \
	Pomg_Source_Pomg_Public_Paddle_h_15_RPC_WRAPPERS_NO_PURE_DECLS \
	Pomg_Source_Pomg_Public_Paddle_h_15_INCLASS_NO_PURE_DECLS \
	Pomg_Source_Pomg_Public_Paddle_h_15_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> POMG_API UClass* StaticClass<class APaddle>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID Pomg_Source_Pomg_Public_Paddle_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
