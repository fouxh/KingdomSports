// Copyright 2021 ReverseWinterStudios Inc. All Rights Reserved.

#include "RWSUtilsHttpRequestHelper.h"
#include "RWSUtilsLibrary.h"

#include "Dom/JsonObject.h"
#include "Runtime/Launch/Resources/Version.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

const FString RWSUtilsHttpRequestHelper::NoResponseErrorMsg(TEXT("No response"));
const FString RWSUtilsHttpRequestHelper::UnknownErrorMsg(TEXT("Unknown error"));
const FString RWSUtilsHttpRequestHelper::DeserializationErrorMsg(TEXT("Failed to deserialize response"));
const FString RWSUtilsHttpRequestHelper::ConversionErrorMsg(TEXT("Failed to convert response"));

TSharedRef<IHttpRequest> RWSUtilsHttpRequestHelper::CreateHttpRequest(const FString& Url,
	const ERWSHttpRequestVerb Verb,
	const FString& AuthToken,
	const FString& Content,
	const FString& SdkModuleName,
	const FString& SdkModuleVersion)
{
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();

	// Referral analytics meta
	FString XRef = TEXT("");
	FString XRefV = TEXT("");

	URWSUtilsLibrary::GetPartnerInfo(XRef, XRefV);

	bool IsReferralAnalyticsSet = !XRef.IsEmpty() && !XRefV.IsEmpty();

	// RWS analytics URL meta
	const FString MetaUrl = FString::Printf(TEXT("%sengine=ue4&engine_v=%s&sdk=%s&sdk_v=%s"),
		Url.Contains(TEXT("?")) ? TEXT("&") : TEXT("?"),
		ENGINE_VERSION_STRING,
		*SdkModuleName.ToLower(),
		*SdkModuleVersion);

	// Referral analytics URL meta
	const FString ReferralMetaUrl = IsReferralAnalyticsSet ? FString::Printf(TEXT("&ref=%s&ref_v=%s"), *XRef.ToLower(), *XRefV.ToLower()) : TEXT("");

	HttpRequest->SetURL(Url + MetaUrl + ReferralMetaUrl);

	// RWS analytics header meta
	HttpRequest->SetHeader(TEXT("X-ENGINE"), TEXT("UE4"));
	HttpRequest->SetHeader(TEXT("X-ENGINE-V"), ENGINE_VERSION_STRING);
	HttpRequest->SetHeader(TEXT("X-SDK"), SdkModuleName);
	HttpRequest->SetHeader(TEXT("X-SDK-V"), SdkModuleVersion);

	// Referral analytics header meta
	if (IsReferralAnalyticsSet)
	{
		HttpRequest->SetHeader(TEXT("X-REF"), XRef);
		HttpRequest->SetHeader(TEXT("X-REF-V"), XRefV);
	}

	switch (Verb)
	{
	case ERWSHttpRequestVerb::VERB_GET:
		HttpRequest->SetVerb(TEXT("GET"));
		break;

	case ERWSHttpRequestVerb::VERB_POST:
		HttpRequest->SetVerb(TEXT("POST"));
		break;

	case ERWSHttpRequestVerb::VERB_PUT:
		HttpRequest->SetVerb(TEXT("PUT"));
		break;

	case ERWSHttpRequestVerb::VERB_DELETE:
		HttpRequest->SetVerb(TEXT("DELETE"));
		break;

	case ERWSHttpRequestVerb::VERB_PATCH:
		HttpRequest->SetVerb(TEXT("PATCH"));
		break;

	default:
		unimplemented();
	}

	if (!AuthToken.IsEmpty())
	{
		HttpRequest->SetHeader(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *AuthToken));
	}

	if (!Content.IsEmpty())
	{
		HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
		HttpRequest->SetContentAsString(Content);
	}

	return HttpRequest;
}

bool RWSUtilsHttpRequestHelper::ParseResponse(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, const bool bSucceeded, RWSHttpRequestError& OutError)
{
	FString ResponseStrr = HttpResponse->GetContentAsString();
	const TArray<uint8> RequestContent = HttpRequest->GetContent();

	FUTF8ToTCHAR Converter((ANSICHAR*)RequestContent.GetData(), RequestContent.Num());
	FString RequestStr = FString(Converter.Length(), Converter.Get());


	if (bSucceeded && HttpResponse.IsValid())
	{
		if (EHttpResponseCodes::IsOk(HttpResponse->GetResponseCode()))
		{
			return true;
		}
		else
		{
			FString ResponseStr = HttpResponse->GetContentAsString();

			TSharedPtr<FJsonObject> JsonObject;
			const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(*ResponseStr);

			if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
			{
				if (!ParseError(JsonObject, OutError))
				{
					OutError = RWSHttpRequestError(UnknownErrorMsg);
				}
			}
			else
			{
				OutError = RWSHttpRequestError(DeserializationErrorMsg);
			}
		}
	}
	else
	{
		OutError = RWSHttpRequestError(NoResponseErrorMsg);
	}

	return false;
}

bool RWSUtilsHttpRequestHelper::ParseResponseAsJson(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, const bool bSucceeded, TSharedPtr<FJsonObject>& OutResponse, RWSHttpRequestError& OutError)
{
	if (bSucceeded && HttpResponse.IsValid())
	{
		FString ResponseStr = HttpResponse->GetContentAsString();

		TSharedPtr<FJsonObject> JsonObject;
		const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(*ResponseStr);

		if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
		{
			if (EHttpResponseCodes::IsOk(HttpResponse->GetResponseCode()))
			{
				OutResponse = JsonObject;
				return true;
			}
			else
			{
				if (!ParseError(JsonObject, OutError))
				{
					OutError = RWSHttpRequestError(UnknownErrorMsg);
				}
			}
		}
		else
		{
			OutError = RWSHttpRequestError(DeserializationErrorMsg);
		}
	}
	else
	{
		OutError = RWSHttpRequestError(NoResponseErrorMsg);
	}

	return false;
}

bool RWSUtilsHttpRequestHelper::ParseResponseAsStruct(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, const bool bSucceeded, const UStruct* OutResponseDefinition, void* OutResponse, RWSHttpRequestError& OutError)
{
	TSharedPtr<FJsonObject> JsonObject;

	if (ParseResponseAsJson(HttpRequest, HttpResponse, bSucceeded, JsonObject, OutError))
	{
		if (FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), OutResponseDefinition, OutResponse))
		{
			return true;
		}

		OutError = RWSHttpRequestError(ConversionErrorMsg);
	}

	return false;
}

bool RWSUtilsHttpRequestHelper::ParseError(TSharedPtr<FJsonObject> JsonObject, RWSHttpRequestError& OutError)
{
	// Try parse Manager API error
	if (JsonObject->HasTypedField<EJson::Object>(TEXT("error")))
	{
		TSharedPtr<FJsonObject> ErrorObject = JsonObject.Get()->GetObjectField(TEXT("error"));
		OutError.code = ErrorObject.Get()->GetStringField(TEXT("code"));
		OutError.description = ErrorObject.Get()->GetStringField(TEXT("description"));
		return true;
	}

	// If No error return false
	return false;
}
