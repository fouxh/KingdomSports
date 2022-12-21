// Copyright 2021 ReverseWinterStudios Inc. All Rights Reserved.

#pragma once

#include "Components/CheckBox.h"
#include "EngineMinimal.h"
#include "Http.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RWSUtilsDataModel.h"
#include "RWSUtilsLibrary.generated.h"

class URWSUtilsSettings;
class URWSUtilsTheme;
class URWSUtilsWidgetsLibrary;
class FJsonObject;

UCLASS()
class RWSUTILS_API URWSUtilsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	static void Internal_AddParametersToJsonObject(TSharedPtr<FJsonObject> JsonObject,
		const FRWSParameters& CustomParameters, const FString& FieldName = "");

public:
	/** Gives direct access to the utils settings. */
	UFUNCTION(BlueprintPure, Category = "RWS|Utils")
	static URWSUtilsSettings* GetUtilsSettings();

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "RWS|Utils")
	static void GetDefaultObject(TSubclassOf<UObject> ObjectClass, UObject*& DefaultObj);
	
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "RWS|Utils")
	static URWSUtilsTheme* GetCurrentTheme();
	
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "RWS|Utils")
	static URWSUtilsTheme* GetTheme(TSubclassOf<URWSUtilsTheme> ThemeClass);

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "RWS|Utils")
	static URWSUtilsWidgetsLibrary* GetCurrentWidgetsLibrary();

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "RWS|Utils")
	static URWSUtilsWidgetsLibrary* GetWidgetsLibrary(TSubclassOf<URWSUtilsWidgetsLibrary> WidgetLibraryClass);

	/** Make FDateTime structure based on a given timestamp. */
	UFUNCTION(BlueprintPure, Category = "RWS|Utils")
	static FDateTime MakeDateTimeFromTimestamp(const int64 Time);

	/** Get seconds from DateTime to UnixTimestamp. */
	UFUNCTION(BlueprintPure, Category = "RWS|Utils")
	static int64 GetSecondsFromUnixTimestamp(const FDateTime& DateTime);

	template <typename TEnum>
	static FString GetEnumValueAsString(const FString& EnumName, TEnum Value)
	{
		const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, *EnumName, true);
		if (!EnumPtr)
		{
			return FString("Invalid");
		}
		const FString ValueStr = EnumPtr->GetNameByValue(static_cast<int64>(Value)).ToString();
		return ValueStr.Replace(*FString::Printf(TEXT("%s::"), *EnumName), TEXT(""));
	}

	template <typename EnumType>
	static EnumType GetEnumValueFromString(const FString& EnumName, const FString& String)
	{
		UEnum* Enum = FindObject<UEnum>(ANY_PACKAGE, *EnumName, true);
		if (!Enum)
		{
			return EnumType(0);
		}
		return static_cast<EnumType>(Enum->GetValueByName(FName(*String)));
	}

	/** Add parameters to json object root */
	static void AddParametersToJsonObject(TSharedPtr<FJsonObject> JsonObject, const FRWSParameters& CustomParameters);

	/** Add parameters to json object by FieldName */
	static void AddParametersToJsonObjectByFieldName(TSharedPtr<FJsonObject> JsonObject, const FString& FieldName, const FRWSParameters& CustomParameters);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToRWSJsonVariant (int)", CompactNodeTitle = "->", BlueprintAutocast), Category = "RWS|Utils")
	static FRWSJsonVariant Conv_IntToRWSJsonVariant(const int Value);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToRWSJsonVariant (float)", CompactNodeTitle = "->", BlueprintAutocast), Category = "RWS|Utils")
	static FRWSJsonVariant Conv_FloatToRWSJsonVariant(const float Value);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToRWSJsonVariant (string)", CompactNodeTitle = "->", BlueprintAutocast), Category = "RWS|Utils")
	static FRWSJsonVariant Conv_StringToRWSJsonVariant(const FString& Value);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToRWSJsonVariant (bool)", CompactNodeTitle = "->", BlueprintAutocast), Category = "RWS|Utils")
	static FRWSJsonVariant Conv_BoolToRWSJsonVariant(const bool Value);

	/** Set additional information for web request. */
	UFUNCTION(BlueprintCallable, Category = "RWS|Utils")
	static void SetPartnerInfo(const FString& Referral, const FString& ReferralVersion);

	static void GetPartnerInfo(FString& Referral, FString& ReferralVersion);

	/** Encodes the request body to match x-www-form-urlencoded data format. */
    static FString EncodeFormData(TSharedPtr<FJsonObject> FormDataJson);

	static FString GetPluginName(const FName& ModuleName);
private:

	static FString XReferral;
	static FString XReferralVersion;
};
