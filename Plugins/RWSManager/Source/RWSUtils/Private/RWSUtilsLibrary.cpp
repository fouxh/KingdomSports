// Copyright 2021 ReverseWinterStudios Inc. All Rights Reserved.

#include "RWSUtilsLibrary.h"
#include "RWSUtilsDefines.h"
#include "RWSUtilsModule.h"
#include "RWSUtilsSettings.h"
#include "RWSUtilsTheme.h"
#include "RWSUtilsWidgetsLibrary.h"

#include "Dom/JsonObject.h"
#include "Interfaces/IPluginManager.h"

FString URWSUtilsLibrary::XReferral(TEXT(""));
FString URWSUtilsLibrary::XReferralVersion(TEXT(""));

URWSUtilsLibrary::URWSUtilsLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void URWSUtilsLibrary::Internal_AddParametersToJsonObject(TSharedPtr<FJsonObject> JsonObject,
	const FRWSParameters& CustomParameters, const FString& FieldName)
{
	if (CustomParameters.Parameters.Num() == 0)
	{
		return;
	}

	TSharedPtr<FJsonObject> JsonRequestObject;
	if (FieldName.IsEmpty())
	{
		JsonRequestObject = JsonObject;
	}
	else
	{
		JsonRequestObject = MakeShareable(new FJsonObject);
	}

	for (auto& Parameter : CustomParameters.Parameters)
	{
		const FVariant Variant = Parameter.Value.Variant;

		switch (Variant.GetType())
		{
		case EVariantTypes::Bool:
			JsonRequestObject->SetBoolField(Parameter.Key, Variant.GetValue<bool>());
			break;
		case EVariantTypes::String:
			JsonRequestObject->SetStringField(Parameter.Key, Variant.GetValue<FString>());
			break;
		case EVariantTypes::Int32:
			JsonRequestObject->SetNumberField(Parameter.Key, Variant.GetValue<int>());
			break;
		case EVariantTypes::Float:
			JsonRequestObject->SetNumberField(Parameter.Key, Variant.GetValue<float>());
			break;
		default:
			UE_LOG(LogRWSUtils, Log, TEXT("%s: parameter with type of %s was not added"),
				*VA_FUNC_LINE, *URWSUtilsLibrary::GetEnumValueAsString("ERWSVariantTypes", static_cast<ERWSVariantTypes>(Variant.GetType())));
			break;
		}
	}

	if (!FieldName.IsEmpty())
	{
		JsonObject->SetObjectField(FieldName, JsonRequestObject);
	}
}

URWSUtilsSettings* URWSUtilsLibrary::GetUtilsSettings()
{
	return FRWSUtilsModule::Get().GetSettings();
}

void URWSUtilsLibrary::GetDefaultObject(TSubclassOf<UObject> ObjectClass, UObject*& DefaultObj)
{
	if (ObjectClass)
	{
		DefaultObj = ObjectClass->GetDefaultObject();
	}
}

URWSUtilsTheme* URWSUtilsLibrary::GetCurrentTheme()
{
	URWSUtilsSettings* Settings = FRWSUtilsModule::Get().GetSettings();
	TSubclassOf<URWSUtilsTheme> CurrentThemeClass = Settings->InterfaceTheme;
	if (!CurrentThemeClass)
	{
		return nullptr;
	}
	
	UObject* CurrentTheme;
	GetDefaultObject(CurrentThemeClass, CurrentTheme);
	
	return Cast<URWSUtilsTheme>(CurrentTheme);
}

URWSUtilsTheme* URWSUtilsLibrary::GetTheme(TSubclassOf<URWSUtilsTheme> ThemeClass)
{
	if (!ThemeClass)
	{
		return nullptr;
	}
	
	UObject* Theme;
	GetDefaultObject(ThemeClass, Theme);
	
	return Cast<URWSUtilsTheme>(Theme);
}

URWSUtilsWidgetsLibrary* URWSUtilsLibrary::GetCurrentWidgetsLibrary()
{
	URWSUtilsSettings* Settings = FRWSUtilsModule::Get().GetSettings();
	TSubclassOf<URWSUtilsWidgetsLibrary> CurrentWidgetsLibraryClass = Settings->WidgetsLibrary;
	if (!CurrentWidgetsLibraryClass)
	{
		return nullptr;
	}

	UObject* CurrentWidgetsLibrary;
	GetDefaultObject(CurrentWidgetsLibraryClass, CurrentWidgetsLibrary);

	return Cast<URWSUtilsWidgetsLibrary>(CurrentWidgetsLibrary);
}

URWSUtilsWidgetsLibrary* URWSUtilsLibrary::GetWidgetsLibrary(TSubclassOf<URWSUtilsWidgetsLibrary> WidgetLibraryClass)
{
	if (!WidgetLibraryClass)
	{
		return nullptr;
	}

	UObject* WidgetLibrary;
	GetDefaultObject(WidgetLibraryClass, WidgetLibrary);

	return Cast<URWSUtilsWidgetsLibrary>(WidgetLibrary);
}

FDateTime URWSUtilsLibrary::MakeDateTimeFromTimestamp(const int64 Time)
{
	return FDateTime::FromUnixTimestamp(Time);
}

int64 URWSUtilsLibrary::GetSecondsFromUnixTimestamp(const FDateTime& DateTime)
{
	return DateTime.ToUnixTimestamp();
}

void URWSUtilsLibrary::AddParametersToJsonObject(TSharedPtr<FJsonObject> JsonObject, const FRWSParameters& CustomParameters)
{
	Internal_AddParametersToJsonObject(JsonObject, CustomParameters);
}

void URWSUtilsLibrary::AddParametersToJsonObjectByFieldName(TSharedPtr<FJsonObject> JsonObject, const FString& FieldName,
	const FRWSParameters& CustomParameters)
{
	Internal_AddParametersToJsonObject(JsonObject, CustomParameters, FieldName);
}

FRWSJsonVariant URWSUtilsLibrary::Conv_IntToRWSJsonVariant(const int Value)
{
	return FRWSJsonVariant(Value);
}

FRWSJsonVariant URWSUtilsLibrary::Conv_FloatToRWSJsonVariant(const float Value)
{
	return FRWSJsonVariant(Value);
}

FRWSJsonVariant URWSUtilsLibrary::Conv_StringToRWSJsonVariant(const FString& Value)
{
	return FRWSJsonVariant(Value);
}

FRWSJsonVariant URWSUtilsLibrary::Conv_BoolToRWSJsonVariant(const bool Value)
{
	return FRWSJsonVariant(Value);
}

void URWSUtilsLibrary::SetPartnerInfo(const FString& Referral, const FString& ReferralVersion)
{
	XReferral = Referral;
	XReferralVersion = ReferralVersion;
}

void URWSUtilsLibrary::GetPartnerInfo(FString& Referral, FString& ReferralVersion)
{
	Referral = XReferral;
	ReferralVersion = XReferralVersion;
}

FString URWSUtilsLibrary::EncodeFormData(TSharedPtr<FJsonObject> FormDataJson)
{
	FString EncodedFormData = "";
	uint16 ParamIndex = 0;

	for (auto FormDataIt = FormDataJson->Values.CreateIterator(); FormDataIt; ++FormDataIt)
	{
		FString Key = FormDataIt.Key();
		FString Value = FormDataIt.Value().Get()->AsString();

		if (!Key.IsEmpty() && !Value.IsEmpty())
		{
			EncodedFormData += ParamIndex == 0 ? "" : "&";
			EncodedFormData += FGenericPlatformHttp::UrlEncode(Key) + "=" + FGenericPlatformHttp::UrlEncode(Value);
		}

		ParamIndex++;
	}

	return EncodedFormData;
}

FString URWSUtilsLibrary::GetPluginName(const FName& ModuleName)
{
	for (const auto& Plugin : IPluginManager::Get().GetEnabledPlugins())
	{
		for (const auto& PluginModule : Plugin->GetDescriptor().Modules)
		{
			if(PluginModule.Name == ModuleName)
			{
				return Plugin->GetName();
			}
		}
	}

	return TEXT("");
}
