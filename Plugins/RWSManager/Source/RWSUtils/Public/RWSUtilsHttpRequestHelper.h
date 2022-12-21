// Copyright 2021 ReverseWinterStudios Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Http.h"
#include "JsonObjectConverter.h"

class FJsonObject;

enum class ERWSHttpRequestVerb : uint8
{
	VERB_GET,
	VERB_POST,
	VERB_PUT,
	VERB_DELETE,
	VERB_PATCH
};

struct RWSUTILS_API RWSHttpRequestError
{
	FString code;
	FString description;
	FString errorMessage;
	int32 statusCode;
	int32 errorCode;

	RWSHttpRequestError()
	{
	}

	RWSHttpRequestError(const FString& errorMsg)
		: description(errorMsg)
		, errorMessage(errorMsg)
	{
	}
};

class RWSUTILS_API RWSUtilsHttpRequestHelper
{
public:
	static TSharedRef<IHttpRequest> CreateHttpRequest(const FString& Url,
		const ERWSHttpRequestVerb Verb = ERWSHttpRequestVerb::VERB_GET,
		const FString& AuthToken = FString(),
		const FString& Content = FString(),
		const FString& SdkModuleName = FString(),
		const FString& SdkModuleVersion = FString());

	static bool ParseResponse(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, const bool bSucceeded, RWSHttpRequestError& OutError);
	static bool ParseResponseAsJson(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, const bool bSucceeded, TSharedPtr<FJsonObject>& OutResponse, RWSHttpRequestError& OutError);
	static bool ParseResponseAsStruct(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, const bool bSucceeded, const UStruct* OutResponseDefinition, void* OutResponse, RWSHttpRequestError& OutError);
	
	template <typename OutStructType>
	static bool ParseResponseAsArray(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, const bool bSucceeded, TArray<OutStructType>* OutResponse, RWSHttpRequestError& OutError);
	
	static bool ParseError(TSharedPtr<FJsonObject> JsonObject, RWSHttpRequestError& OutError);

private:

	static const FString NoResponseErrorMsg;
	static const FString UnknownErrorMsg;
	static const FString DeserializationErrorMsg;
	static const FString ConversionErrorMsg;
};

template <typename OutStructType>
bool RWSUtilsHttpRequestHelper::ParseResponseAsArray(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, const bool bSucceeded, TArray<OutStructType>* OutResponse, RWSHttpRequestError& OutError)
{
	if (bSucceeded && HttpResponse.IsValid())
	{
		FString ResponseStr = HttpResponse->GetContentAsString();

		if (EHttpResponseCodes::IsOk(HttpResponse->GetResponseCode()))
		{
			if (FJsonObjectConverter::JsonArrayStringToUStruct(ResponseStr, OutResponse, 0, 0))
			{
				return true;
			}

			OutError = RWSHttpRequestError(ConversionErrorMsg);
		}
		else
		{
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
