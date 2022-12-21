// Copyright 2021 ReverseWinterStudios Inc. All Rights Reserved.

#include "RWSManagerGameMode.h"
#include "RWSManagerDefines.h"
#include "RWSManager.h"
#include "RWSFunctionLibrary.h"

#include "RWSManagerSettings.h"

#include "RWSManagerPlayerState.h"
#include "RWSManagerGameInstance.h"

#include "RWSUtilsUrlBuilder.h"

#include "Dom/JsonObject.h"
#include "JsonObjectConverter.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"

#include "Engine.h"

#include "GameFramework/GameStateBase.h"

void ARWSManagerGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ARWSManagerGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	ErrorMessage = GameSession->ApproveLogin(Options);
	FGameModeEvents::GameModePreLoginEvent.Broadcast(this, UniqueId, ErrorMessage);
}

void ARWSManagerGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	if (NewPlayer)
	{
		if (IsRunningDedicatedServer()) {
			ARWSManagerGameMode::RWSServerPlayerJoined();
		}
	}
}

void ARWSManagerGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	if (Exiting)
	{
		if (IsRunningDedicatedServer()) {
			ARWSManagerGameMode::RWSServerPlayerLeft();
		}
	}
}

//Create Request and send it to complete to check for errors or success then send to blueprint
//Create Session RWS
void ARWSManagerGameMode::RWSCreateSession(const FString& MapName, const FString& GameType, const FString& GameRegion, const FString& GameMode, const int32 MaxPlayers, const FOnServerCreationReceived& SuccessCallback, const FOnAuthError& ErrorCallback)
{
	TSharedPtr<FJsonObject> RequestDataJson = MakeShareable(new FJsonObject());
	FString PortNumber = ARWSManagerGameMode::GetPortNumber(this);
	RequestDataJson->SetStringField(TEXT("PortNumber"), PortNumber);
	RequestDataJson->SetStringField(TEXT("MapName"), MapName);
	RequestDataJson->SetNumberField(TEXT("Max_Players"), MaxPlayers);
	RequestDataJson->SetStringField(TEXT("GameType"), GameType);
	RequestDataJson->SetStringField(TEXT("GameMode"), GameMode);
	RequestDataJson->SetStringField(TEXT("GameRegion"), GameRegion);

	FString ProjectVersion;
	GConfig->GetString(TEXT("/Script/EngineSettings.GeneralProjectSettings"), TEXT("ProjectVersion"), ProjectVersion, GGameIni);
	if (ProjectVersion != "") { RequestDataJson->SetStringField(TEXT("server_version"), ProjectVersion); }

	FString PostContent;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&PostContent);
	FJsonSerializer::Serialize(RequestDataJson.ToSharedRef(), Writer);

	// Generate endpoint url
	const URWSManagerSettings* Settings = FRWSManager::Get().GetSettings();
	const FString Endpoint = TEXT("https://reversewinterstudios.com/game/api/v1/server/add_server");
	const FString Url = RWSUtilsUrlBuilder(Endpoint)
							.AddStringQueryParam(TEXT("game_id"), Settings->ProjectID)	   
		                    .AddStringQueryParam(TEXT("login_project_id"), Settings->LoginID)	
							.Build();

	TSharedRef<IHttpRequest> HttpRequest = CreateHttpRequest(Url, ERWSHttpRequestVerb::VERB_POST, PostContent);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &ARWSManagerGameMode::RWSCreateSession_HttpRequestComplete, SuccessCallback, ErrorCallback);
	HttpRequest->ProcessRequest();
}

//Update Server with any variable in the list of the API and send it to complete to check for errors or success then send to blueprint
void ARWSManagerGameMode::RWSUpdateServer(const FString& variable, const FString& value, const FOnServerUpdateReceived& SuccessCallback, const FOnAuthError& ErrorCallback)
{
	TSharedPtr<FJsonObject> RequestDataJson = MakeShareable(new FJsonObject());
	FString PortNumber = ARWSManagerGameMode::GetPortNumber(this);
	RequestDataJson->SetStringField(TEXT("ServerID"), ServerID);
	RequestDataJson->SetStringField(TEXT("variable"), variable);
	RequestDataJson->SetStringField(TEXT("value"), value);

	FString PostContent;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&PostContent);
	FJsonSerializer::Serialize(RequestDataJson.ToSharedRef(), Writer);

	// Generate endpoint url
	const URWSManagerSettings* Settings = FRWSManager::Get().GetSettings();
	const FString Endpoint = TEXT("https://reversewinterstudios.com/game/api/v1/server/server_update");
	const FString Url = RWSUtilsUrlBuilder(Endpoint)
		.AddStringQueryParam(TEXT("game_id"), Settings->ProjectID)
		.AddStringQueryParam(TEXT("login_project_id"), Settings->LoginID)
		.Build();

	TSharedRef<IHttpRequest> HttpRequest = CreateHttpRequest(Url, ERWSHttpRequestVerb::VERB_POST, PostContent);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &ARWSManagerGameMode::RWSUpdateServer_HttpRequestComplete, SuccessCallback, ErrorCallback);
	HttpRequest->ProcessRequest();
}

//Send server heartbeat to tell master server that this one is still alive and doesnt get shutdown from master server.
void ARWSManagerGameMode::RWSServerHeartBeat()
{
	const FOnServerHeartBeatReceived SuccessCallback;
	const FOnAuthError ErrorCallback;

	TSharedPtr<FJsonObject> RequestDataJson = MakeShareable(new FJsonObject());
	RequestDataJson->SetStringField(TEXT("ServerID"), ServerID);

	FString PostContent;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&PostContent);
	FJsonSerializer::Serialize(RequestDataJson.ToSharedRef(), Writer);

	// Generate endpoint url
	const URWSManagerSettings* Settings = FRWSManager::Get().GetSettings();
	const FString Endpoint = TEXT("https://reversewinterstudios.com/game/api/v1/server/server_heartbeat");
	const FString Url = RWSUtilsUrlBuilder(Endpoint)
							.AddStringQueryParam(TEXT("game_id"), Settings->ProjectID)
							.AddStringQueryParam(TEXT("login_project_id"), Settings->LoginID)
							.Build();

	TSharedRef<IHttpRequest> HttpRequest = CreateHttpRequest(Url, ERWSHttpRequestVerb::VERB_POST, PostContent);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &ARWSManagerGameMode::RWSServerHeartBeat_HttpRequestComplete, SuccessCallback, ErrorCallback);
	HttpRequest->ProcessRequest();
}

//Tell server that someone has joined.
void ARWSManagerGameMode::RWSServerPlayerJoined()
{
	const FOnServerPlayerJoined SuccessCallback;
	const FOnAuthError ErrorCallback;

	TSharedPtr<FJsonObject> RequestDataJson = MakeShareable(new FJsonObject());
	RequestDataJson->SetStringField(TEXT("ServerID"), ServerID);

	FString PostContent;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&PostContent);
	FJsonSerializer::Serialize(RequestDataJson.ToSharedRef(), Writer);

	// Generate endpoint url
	const URWSManagerSettings* Settings = FRWSManager::Get().GetSettings();
	const FString Endpoint = TEXT("https://reversewinterstudios.com/game/api/v1/server/player_joined");
	const FString Url = RWSUtilsUrlBuilder(Endpoint)
							.AddStringQueryParam(TEXT("game_id"), Settings->ProjectID)
							.AddStringQueryParam(TEXT("login_project_id"), Settings->LoginID)
							.Build();

	TSharedRef<IHttpRequest> HttpRequest = CreateHttpRequest(Url, ERWSHttpRequestVerb::VERB_POST, PostContent);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &ARWSManagerGameMode::RWSServerPlayerJoined_HttpRequestComplete, SuccessCallback, ErrorCallback);
	HttpRequest->ProcessRequest();
}

//Tell server that someone has Left.
void ARWSManagerGameMode::RWSServerPlayerLeft()
{
	const FOnServerPlayerLeft SuccessCallback;
	const FOnAuthError ErrorCallback;

	TSharedPtr<FJsonObject> RequestDataJson = MakeShareable(new FJsonObject());
	RequestDataJson->SetStringField(TEXT("ServerID"), ServerID);

	FString PostContent;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&PostContent);
	FJsonSerializer::Serialize(RequestDataJson.ToSharedRef(), Writer);

	// Generate endpoint url
	const URWSManagerSettings* Settings = FRWSManager::Get().GetSettings();
	const FString Endpoint = TEXT("https://reversewinterstudios.com/game/api/v1/server/player_left");
	const FString Url = RWSUtilsUrlBuilder(Endpoint)
							.AddStringQueryParam(TEXT("game_id"), Settings->ProjectID)
							.AddStringQueryParam(TEXT("login_project_id"), Settings->LoginID)
							.Build();

	TSharedRef<IHttpRequest> HttpRequest = CreateHttpRequest(Url, ERWSHttpRequestVerb::VERB_POST, PostContent);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &ARWSManagerGameMode::RWSServerPlayerLeft_HttpRequestComplete, SuccessCallback, ErrorCallback);
	HttpRequest->ProcessRequest();
}

void ARWSManagerGameMode::RWSUpdateServer_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, const bool bSucceeded,
	FOnServerUpdateReceived SuccessCallback, FOnAuthError ErrorCallback)
{
	TSharedPtr<FJsonObject> JsonObject;
	RWSHttpRequestError OutError;

	if (RWSUtilsHttpRequestHelper::ParseResponseAsJson(HttpRequest, HttpResponse, bSucceeded, JsonObject, OutError))
	{
		static const FString ServerUpdateContent = TEXT("content");
		if (JsonObject->HasTypedField<EJson::String>(ServerUpdateContent))
		{
			SuccessCallback.ExecuteIfBound();
			return;
		}
		TSharedPtr<FJsonObject> ErrorObject = JsonObject.Get()->GetObjectField(TEXT("error"));
		OutError.description = ErrorObject.Get()->GetStringField(TEXT("description"));
	}

	HandleRequestError(OutError, ErrorCallback);
}

void ARWSManagerGameMode::RWSCreateSession_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, const bool bSucceeded,
	FOnServerCreationReceived SuccessCallback, FOnAuthError ErrorCallback)
{
	TSharedPtr<FJsonObject> JsonObject;
	RWSHttpRequestError OutError;

	if (RWSUtilsHttpRequestHelper::ParseResponseAsJson(HttpRequest, HttpResponse, bSucceeded, JsonObject, OutError))
	{
		static const FString ServerIpContent = TEXT("content");
		if (JsonObject->HasTypedField<EJson::String>(ServerIpContent))
		{
			const FString Content = JsonObject.Get()->GetStringField(ServerIpContent);

			//Get Server Information
			TSharedPtr<FJsonObject> ServerObject = JsonObject.Get()->GetObjectField(TEXT("server"));
			ServerID = ServerObject.Get()->GetStringField(TEXT("ServerID"));
			FString GameType = ServerObject.Get()->GetStringField(TEXT("GameType"));
			FString GameMode = ServerObject.Get()->GetStringField(TEXT("GameMode"));

			//Execute if server started then check if any players left just in-case server doesnt shutdown. 
			//Default is 50 seconds.
			GetWorldTimerManager().SetTimer(CheckPlayerLeft, this, &ARWSManagerGameMode::RWSShutDownServer, 50.0f, true);

			//Execute server heartbeat.
			//Default is 50 seconds.
			GetWorldTimerManager().SetTimer(ServerHeartBeat, this, &ARWSManagerGameMode::RWSServerHeartBeat, 50.0f, true);

			SuccessCallback.ExecuteIfBound(ServerID, GameMode, GameType);
			return;
		}
		TSharedPtr<FJsonObject> ErrorObject = JsonObject.Get()->GetObjectField(TEXT("error"));
		OutError.description = ErrorObject.Get()->GetStringField(TEXT("description"));
		ARWSManagerGameMode::RWSShutDownServer();
	}

	HandleRequestError(OutError, ErrorCallback);
}

void ARWSManagerGameMode::RWSServerPlayerJoined_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, const bool bSucceeded,
	FOnServerPlayerJoined SuccessCallback, FOnAuthError ErrorCallback)
{
	TSharedPtr<FJsonObject> JsonObject;
	RWSHttpRequestError OutError;

	if (RWSUtilsHttpRequestHelper::ParseResponseAsJson(HttpRequest, HttpResponse, bSucceeded, JsonObject, OutError))
	{
		static const FString ServerPlayerJoined = TEXT("content");
		if (JsonObject->HasTypedField<EJson::String>(ServerPlayerJoined))
		{
			const FString Content = JsonObject.Get()->GetStringField(ServerPlayerJoined);
			SuccessCallback.ExecuteIfBound(Content);
			UE_LOG(LogRWSManager, Warning, TEXT("%s"), *Content);
			return;
		}
		TSharedPtr<FJsonObject> ErrorObject = JsonObject.Get()->GetObjectField(TEXT("error"));
		OutError.description = ErrorObject.Get()->GetStringField(TEXT("description"));
	}

	HandleRequestError(OutError, ErrorCallback);
}

void ARWSManagerGameMode::RWSServerPlayerLeft_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, const bool bSucceeded, FOnServerPlayerLeft SuccessCallback, FOnAuthError ErrorCallback)
{
	TSharedPtr<FJsonObject> JsonObject;
	RWSHttpRequestError OutError;

	if (RWSUtilsHttpRequestHelper::ParseResponseAsJson(HttpRequest, HttpResponse, bSucceeded, JsonObject, OutError))
	{
		static const FString ServerPlayerLeft = TEXT("content");
		if (JsonObject->HasTypedField<EJson::String>(ServerPlayerLeft))
		{
			const FString Content = JsonObject.Get()->GetStringField(ServerPlayerLeft);
			SuccessCallback.ExecuteIfBound(Content);
			UE_LOG(LogRWSManager, Warning, TEXT("%s"), *Content);
			return;
		}
		TSharedPtr<FJsonObject> ErrorObject = JsonObject.Get()->GetObjectField(TEXT("error"));
		OutError.description = ErrorObject.Get()->GetStringField(TEXT("description"));
	}

	HandleRequestError(OutError, ErrorCallback);
}

void ARWSManagerGameMode::RWSServerHeartBeat_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, const bool bSucceeded, FOnServerHeartBeatReceived SuccessCallback, FOnAuthError ErrorCallback)
{
	TSharedPtr<FJsonObject> JsonObject;
	RWSHttpRequestError OutError;

	if (RWSUtilsHttpRequestHelper::ParseResponseAsJson(HttpRequest, HttpResponse, bSucceeded, JsonObject, OutError))
	{
		static const FString ServerHeartbeat = TEXT("content");
		if (JsonObject->HasTypedField<EJson::String>(ServerHeartbeat))
		{
			const FString Content = JsonObject.Get()->GetStringField(ServerHeartbeat);
			SuccessCallback.ExecuteIfBound(Content);
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("RWS Heartbeat: %s"), *Content));
			UE_LOG(LogRWSManager, Warning, TEXT("RWS Heartbeat: %s"), *Content);
			return;
		}
		TSharedPtr<FJsonObject> ErrorObject = JsonObject.Get()->GetObjectField(TEXT("error"));
		OutError.description = ErrorObject.Get()->GetStringField(TEXT("description"));
	}

	HandleRequestError(OutError, ErrorCallback);
}

//Same as the one inside the function library
void ARWSManagerGameMode::RWSShutDownServer()
{
	if (IsRunningDedicatedServer()) {
		const URWSManagerSettings* Settings = FRWSManager::Get().GetSettings();
		NewObject<URWSFunctionLibrary>(this)->RWSShutDownServer(Settings->ProjectID, Settings->LoginID);
	}
}

FString ARWSManagerGameMode::GetPortNumber(AGameModeBase* CurrentGameMode)
{
	FString PortNumber;
	PortNumber = FString::FromInt(CurrentGameMode->GetWorld()->URL.Port);
	return PortNumber;
}

void ARWSManagerGameMode::SetStringArrayField(TSharedPtr<FJsonObject> Object, const FString& FieldName, const TArray<FString>& Array) const
{
	TArray<TSharedPtr<FJsonValue>> StringJsonArray;
	for (auto Item : Array)
	{
		StringJsonArray.Push(MakeShareable(new FJsonValueString(Item)));
	}

	Object->SetArrayField(FieldName, StringJsonArray);
}

void ARWSManagerGameMode::HandleRequestError(RWSHttpRequestError ErrorData, FOnAuthError ErrorCallback)
{
	UE_LOG(LogRWSManager, Error, TEXT("%s: request failed - Error code: %s, Error message: %s"), *VA_FUNC_LINE, *ErrorData.code, *ErrorData.description);
	ErrorCallback.ExecuteIfBound(ErrorData.code, ErrorData.description);
}

TSharedRef<IHttpRequest> ARWSManagerGameMode::CreateHttpRequest(const FString& Url, const ERWSHttpRequestVerb Verb, const FString& Content, const FString& AuthToken)
{
	return RWSUtilsHttpRequestHelper::CreateHttpRequest(Url, Verb, AuthToken, Content, TEXT("SERVER_MANAGER"), RWS_MANAGER_VERSION);
}