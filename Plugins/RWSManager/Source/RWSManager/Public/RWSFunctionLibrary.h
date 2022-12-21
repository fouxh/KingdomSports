// Copyright 2021 ReverseWinterStudios Inc. All Rights Reserved.

#pragma once

#include "RWSUtilsDataModel.h"
#include "RWSUtilsHttpRequestHelper.h"

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RWSFunctionLibrary.generated.h"

/** Common callback for operations without any user-friendly messages from the server in case of success. */
DECLARE_DYNAMIC_DELEGATE(FOnRequestSuccessFunctionLibrary);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnAuthErrorFunctionLibrary, const FString&, Code, const FString&, Description);

class FJsonObject;


UCLASS()
class RWSMANAGER_API URWSFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Get current Hosted ServerID to display for users to report issues or cheaters.. For future use in a reporting system.. You can still use this to display the ServerID publicly */
	UFUNCTION(BlueprintPure, Category = "RWS|Server Manager|Data", meta = (Keywords = "Server"))
	static FString GetServerID();

	/** Get current region of server requested */
	UFUNCTION(BlueprintPure, Category = "RWS|Server Manager|Data", meta = (Keywords = "Server"))
	static FString RWSServerRegionRequested();

	/** Get the current Game Mode that was submited into create session 
	* From GameMode "This is your mode, Can be Solo, Duo, Squads for a game that has different team based modes if you don't need this option just leave blank."
	*/
	UFUNCTION(BlueprintPure, Category = "RWS|Server Manager|Data", meta = (Keywords = "Server"))
	static FString RWSServerModeRequested();

	/** Get the current Game Type that was submited into create session 
	* From GameMode "This is your Type, Can be FPS, Third Person, Battle Royale. It will be publicly viewed."
	*/
	UFUNCTION(BlueprintPure, Category = "RWS|Server Manager|Data", meta = (Keywords = "Server"))
	static FString RWSServerTypeRequested();

	/** Get the current Game Map that was submited into create session */
	UFUNCTION(BlueprintPure, Category = "RWS|Server Manager|Data", meta = (Keywords = "Server"))
	static FString RWSServerMapNameRequested();

	//Server Cleanup
	void RWSServerCleanup(const FString& ServerID, const FString& ProjectID, const FString& LoginID);
	void RWSServerCleanup_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, const bool bSucceeded, FOnRequestSuccessFunctionLibrary SuccessCallback, FOnAuthErrorFunctionLibrary ErrorCallback);

	/** Shutdown server if needed this is done automatically but if you need the server to shut down run this. */
	void RWSShutDownServer(const FString& ProjectID, const FString& LoginID);

	/** Check if any players are joining before shutting down server. */
	void RWSCheckServerJoining(const FString& ServerID);
	void RWSCheckServerJoining_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, const bool bSucceeded, FOnRequestSuccessFunctionLibrary SuccessCallback, FOnAuthErrorFunctionLibrary ErrorCallback);

protected:
	/** Returns true if the error occurs. */
	void HandleRequestError(RWSHttpRequestError ErrorData, FOnAuthErrorFunctionLibrary ErrorCallback);

private:
	/** Sets a JSON string array field named FieldName and value of Array. */
	void SetStringArrayField(TSharedPtr<FJsonObject> Object, const FString& FieldName, const TArray<FString>& Array) const;

	/** Create http request and add RWS API meta */
	TSharedRef<IHttpRequest> CreateHttpRequest(const FString& Url, const ERWSHttpRequestVerb Verb = ERWSHttpRequestVerb::VERB_GET,
	const FString& Content = FString(), const FString& AuthToken = FString());
};
