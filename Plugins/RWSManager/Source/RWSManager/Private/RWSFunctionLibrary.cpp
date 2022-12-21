// Copyright 2021 ReverseWinterStudios Inc. All Rights Reserved.

#include "RWSFunctionLibrary.h"
#include "RWSManagerDefines.h"
#include "RWSManager.h"
#include "RWSManagerSettings.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"
#include "RWSManagerGameMode.h"

#include "RWSUtilsUrlBuilder.h"

#include "Dom/JsonObject.h"
#include "JsonObjectConverter.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"

#include "Engine.h"

#include "GameFramework/GameStateBase.h"

void URWSFunctionLibrary::RWSShutDownServer(const FString& ProjectID, const FString& LoginID)
{
	if (IsRunningDedicatedServer()) {
		AGameStateBase* MyGameState = GetWorld()->GetGameState<AGameStateBase>();
		if (MyGameState->PlayerArray.Num() == 0)
		{
			URWSFunctionLibrary::RWSCheckServerJoining(URWSFunctionLibrary::GetServerID());
		}
	}
}

void URWSFunctionLibrary::RWSCheckServerJoining(const FString& ServerID)
{
	const FOnRequestSuccessFunctionLibrary SuccessCallback;
	const FOnAuthErrorFunctionLibrary ErrorCallback;

	TSharedPtr<FJsonObject> RequestDataJson = MakeShareable(new FJsonObject());
	RequestDataJson->SetStringField(TEXT("ServerID"), ServerID);

	FString PostContent;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&PostContent);
	FJsonSerializer::Serialize(RequestDataJson.ToSharedRef(), Writer);

	// Generate endpoint url
	const FString Endpoint = TEXT("https://reversewinterstudios.com/game/api/v1/server/check_server");
	const URWSManagerSettings* Settings = FRWSManager::Get().GetSettings();
	const FString Url = RWSUtilsUrlBuilder(Endpoint)
		.AddStringQueryParam(TEXT("game_id"), Settings->ProjectID)
		.AddStringQueryParam(TEXT("login_project_id"), Settings->LoginID)
		.Build();

	TSharedRef<IHttpRequest> HttpRequest = CreateHttpRequest(Url, ERWSHttpRequestVerb::VERB_POST, PostContent);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &URWSFunctionLibrary::RWSCheckServerJoining_HttpRequestComplete, SuccessCallback, ErrorCallback);
	HttpRequest->ProcessRequest();
}

void URWSFunctionLibrary::RWSCheckServerJoining_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, const bool bSucceeded, FOnRequestSuccessFunctionLibrary SuccessCallback, FOnAuthErrorFunctionLibrary ErrorCallback)
{
	TSharedPtr<FJsonObject> JsonObject;
	RWSHttpRequestError OutError;

	if (RWSUtilsHttpRequestHelper::ParseResponseAsJson(HttpRequest, HttpResponse, bSucceeded, JsonObject, OutError))
	{
		static const FString ServerRemoved = TEXT("content");
		if (JsonObject->HasTypedField<EJson::String>(ServerRemoved))
		{
			const FString Content = JsonObject.Get()->GetStringField(ServerRemoved);

			TSharedPtr<FJsonObject> MatchmakingObject = JsonObject.Get()->GetObjectField(TEXT("matchmaking"));

			const URWSManagerSettings* Settings = FRWSManager::Get().GetSettings();

			UE_LOG(LogRWSManager, Warning, TEXT("Requesting Server Shutdown!"));

			if (MatchmakingObject.Get()->GetStringField(TEXT("status")) == "dont_shutdown") 
			{
				UE_LOG(LogRWSManager, Warning, TEXT("Requesting Server Shutdown: Player Joining Server Canceled Shutdown."));
			}
			else if (MatchmakingObject.Get()->GetStringField(TEXT("status")) == "shutdown") 
			{
				UE_LOG(LogRWSManager, Warning, TEXT("Requesting Server Shutdown: Success."));
				URWSFunctionLibrary::RWSServerCleanup(URWSFunctionLibrary::GetServerID(), Settings->ProjectID, Settings->LoginID);
				FGenericPlatformMisc::RequestExit(false);
			}

			SuccessCallback.ExecuteIfBound();
			return;
		}
		TSharedPtr<FJsonObject> ErrorObject = JsonObject.Get()->GetObjectField(TEXT("error"));
		OutError.description = ErrorObject.Get()->GetStringField(TEXT("description"));
	}

	HandleRequestError(OutError, ErrorCallback);
}

void URWSFunctionLibrary::RWSServerCleanup(const FString& ServerID, const FString& ProjectID, const FString& LoginID)
{
	const FOnRequestSuccessFunctionLibrary SuccessCallback;
	const FOnAuthErrorFunctionLibrary ErrorCallback;

	TSharedPtr<FJsonObject> RequestDataJson = MakeShareable(new FJsonObject());
	RequestDataJson->SetStringField(TEXT("ServerID"), ServerID);

	FString PostContent;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&PostContent);
	FJsonSerializer::Serialize(RequestDataJson.ToSharedRef(), Writer);

	// Generate endpoint url
	const FString Endpoint = TEXT("https://reversewinterstudios.com/game/api/v1/server/server_remove");
	const FString Url = RWSUtilsUrlBuilder(Endpoint)
							.AddStringQueryParam(TEXT("game_id"), ProjectID)
		                    .AddStringQueryParam(TEXT("login_project_id"), LoginID)
		                    .AddStringQueryParam(TEXT("server_manager"), "instant")
							.Build();

	TSharedRef<IHttpRequest> HttpRequest = CreateHttpRequest(Url, ERWSHttpRequestVerb::VERB_POST, PostContent);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &URWSFunctionLibrary::RWSServerCleanup_HttpRequestComplete, SuccessCallback, ErrorCallback);
	HttpRequest->ProcessRequest();
}

void URWSFunctionLibrary::RWSServerCleanup_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, const bool bSucceeded, FOnRequestSuccessFunctionLibrary SuccessCallback, FOnAuthErrorFunctionLibrary ErrorCallback)
{
	TSharedPtr<FJsonObject> JsonObject;
	RWSHttpRequestError OutError;

	if (RWSUtilsHttpRequestHelper::ParseResponseAsJson(HttpRequest, HttpResponse, bSucceeded, JsonObject, OutError))
	{
		static const FString ServerRemoved = TEXT("content");
		if (JsonObject->HasTypedField<EJson::String>(ServerRemoved))
		{
			const FString Content = JsonObject.Get()->GetStringField(ServerRemoved);
			SuccessCallback.ExecuteIfBound();
			UE_LOG(LogRWSManager, Warning, TEXT("%s"), *Content);
			return;
		}
		TSharedPtr<FJsonObject> ErrorObject = JsonObject.Get()->GetObjectField(TEXT("error"));
		OutError.description = ErrorObject.Get()->GetStringField(TEXT("description"));
	}

	HandleRequestError(OutError, ErrorCallback);
}

FString URWSFunctionLibrary::GetServerID()
{
	for (TObjectIterator<ARWSManagerGameMode> It; It; ++It)
	{
		return It->ServerID;
	}
	return "";
}

FString URWSFunctionLibrary::RWSServerRegionRequested()
{
	FString param;
	FParse::Value(FCommandLine::Get(), TEXT("-region="), param);
	return param;
}

FString URWSFunctionLibrary::RWSServerModeRequested()
{
	FString param;
	FParse ::Value(FCommandLine::Get(), TEXT("-mode="), param);
	return param;
}

FString URWSFunctionLibrary::RWSServerTypeRequested()
{
	FString param;
	FParse ::Value(FCommandLine::Get(), TEXT("-type="), param);
	return param;
}

FString URWSFunctionLibrary::RWSServerMapNameRequested()
{
	FString param;
	FParse::Value(FCommandLine::Get(), TEXT("-map="), param);
	return param;
}

void URWSFunctionLibrary::SetStringArrayField(TSharedPtr<FJsonObject> Object, const FString& FieldName, const TArray<FString>& Array) const
{
	TArray<TSharedPtr<FJsonValue>> StringJsonArray;
	for (auto Item : Array)
	{
		StringJsonArray.Push(MakeShareable(new FJsonValueString(Item)));
	}

	Object->SetArrayField(FieldName, StringJsonArray);
}

void URWSFunctionLibrary::HandleRequestError(RWSHttpRequestError ErrorData, FOnAuthErrorFunctionLibrary ErrorCallback)
{
	UE_LOG(LogRWSManager, Error, TEXT("%s: request failed - Error code: %s, Error message: %s"), *VA_FUNC_LINE, *ErrorData.code, *ErrorData.description);
	ErrorCallback.ExecuteIfBound(ErrorData.code, ErrorData.description);
}

TSharedRef<IHttpRequest> URWSFunctionLibrary::CreateHttpRequest(const FString& Url, const ERWSHttpRequestVerb Verb, const FString& Content, const FString& AuthToken)
{
	return RWSUtilsHttpRequestHelper::CreateHttpRequest(Url, Verb, AuthToken, Content, TEXT("SERVER_MANAGER"), RWS_MANAGER_VERSION);
}