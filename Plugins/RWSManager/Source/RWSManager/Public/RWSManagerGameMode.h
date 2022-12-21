// Copyright 2021 ReverseWinterStudios Inc. All Rights Reserved.

#pragma once

#include "RWSUtilsDataModel.h"
#include "RWSFunctionLibrary.h"
#include "RWSUtilsHttpRequestHelper.h"

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "RWSManagerGameMode.generated.h"

/** Common callback for operations without any user-friendly messages from the server in case of success. */
DECLARE_DYNAMIC_DELEGATE(FOnRequestSuccess);

DECLARE_DYNAMIC_DELEGATE_ThreeParams(FOnServerCreationReceived, const FString&, ServerID, const FString&, GameMode, const FString&, GameType);
DECLARE_DYNAMIC_DELEGATE(FOnServerUpdateReceived);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnServerHeartBeatReceived, const FString&, ServerID);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnServerCleanup, const FString&, ServerID);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnServerPlayerJoined, const FString&, PlayerJoined);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnServerPlayerLeft, const FString&, PlayerLeft);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnAuthError, const FString&, Code, const FString&, Description);

class FJsonObject;
class APawn;

UCLASS()
class RWSMANAGER_API ARWSManagerGameMode : public AGameModeBase
{
	GENERATED_BODY()


public:
	FString ServerID;
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	/** RWS Create Session
	 *
	 * @param SuccessCallback Callback function called after successful Session Creation. The new server will start up and load into it.
	 * @param ErrorCallback Callback function called after the request resulted with an error.
	 * @param MapName This is what ever you want your map to be called and will be publicly viewed with this name.
	 * @param GameType This is your Type, Can be FPS, Third Person, Battle Royale. It will be publicly viewed.
	 * @param GameMode This is your mode, Can be Solo, Duo, Squads for a game that has different team based modes if you don't need this option just leave blank.
	 * @param MaxPlayers If you run your own server there is no limit to how many players you fit in a lobby.
	 * 
	 */
	UFUNCTION(BlueprintCallable, Category = "RWS|Server Manager|Session", meta = (AutoCreateRefTerm = "SuccessCallback, ErrorCallback"))
	void RWSCreateSession(const FString& MapName, const FString& GameType, const FString& GameRegion, const FString& GameMode, const int32 MaxPlayers, const FOnServerCreationReceived& SuccessCallback, const FOnAuthError& ErrorCallback);

	/** RWS Update Server
	 *
	 * @param SuccessCallback Callback function called after successful Session Creation. The new server will start up and load into it.
	 * @param ErrorCallback Callback function called after the request resulted with an error.
	 * @param variable This can be found on the documentation page of ReverseWinterStudios. (e.g. status)
	 * @param value This can be found on the documentation page of ReverseWinterStudios. (e.g. Starting)
	 *
	 */
	UFUNCTION(BlueprintCallable, Category = "RWS|Server Manager|Session", meta = (AutoCreateRefTerm = "SuccessCallback, ErrorCallback"))
	void RWSUpdateServer(const FString& variable, const FString& value, const FOnServerUpdateReceived& SuccessCallback, const FOnAuthError& ErrorCallback);

protected:
	/** Checks RWSCreateSession for any errors or if its success it will pass the information back */
	void RWSCreateSession_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, const bool bSucceeded,
		FOnServerCreationReceived SuccessCallback, FOnAuthError ErrorCallback);

	/** Checks RWSUpdateServer for any errors or if its success it will pass the information back */
	void RWSUpdateServer_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, const bool bSucceeded,
		FOnServerUpdateReceived SuccessCallback, FOnAuthError ErrorCallback);

	/** Returns true if the error occurs. */
	void HandleRequestError(RWSHttpRequestError ErrorData, FOnAuthError ErrorCallback);

private:
	void BeginPlay() override;
	void PostLogin(APlayerController* NewPlayer) override;
	void Logout(AController* Exiting) override;

	/** Sets a JSON string array field named FieldName and value of Array. */
	void SetStringArrayField(TSharedPtr<FJsonObject> Object, const FString& FieldName, const TArray<FString>& Array) const;

	/** Create http request and add RWS API meta */
	TSharedRef<IHttpRequest> CreateHttpRequest(const FString& Url, const ERWSHttpRequestVerb Verb = ERWSHttpRequestVerb::VERB_GET,
	const FString& Content = FString(), const FString& AuthToken = FString());

protected:
	//Automatically get port number for server
	static FString GetPortNumber(AGameModeBase* CurrentGameMode);

	//Send heartbeat to master server to keep server alive.
	FTimerHandle ServerHeartBeat;
	void RWSServerHeartBeat();
	void RWSServerHeartBeat_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, const bool bSucceeded, FOnServerHeartBeatReceived SuccessCallback, FOnAuthError ErrorCallback);

	//Check if there is any players left on server if not close the server.
	FTimerHandle CheckPlayerLeft;
	void RWSServerPlayerLeft();
	void RWSServerPlayerLeft_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, const bool bSucceeded, FOnServerPlayerLeft SuccessCallback, FOnAuthError ErrorCallback);

	//When a player joins the server
	void RWSServerPlayerJoined();
	void RWSServerPlayerJoined_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, const bool bSucceeded, FOnServerPlayerJoined SuccessCallback, FOnAuthError ErrorCallback);

	//Shutdown server
	void RWSShutDownServer();
};
