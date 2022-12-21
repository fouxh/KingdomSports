// Copyright 2021 ReverseWinterStudios Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class RWSUTILS_API RWSUtilsUrlBuilder
{
public:
	/*
	 * Path params of input URL should be wrapped with curly braces in order to be processed properly
	 */
	RWSUtilsUrlBuilder(const FString& UrlTemplate);

	FString Build();

	RWSUtilsUrlBuilder& SetPathParam(const FString& ParamName, const FString& ParamValue);
	RWSUtilsUrlBuilder& SetPathParam(const FString& ParamName, const int32 ParamValue);
	RWSUtilsUrlBuilder& SetPathParam(const FString& ParamName, const int64 ParamValue);

	RWSUtilsUrlBuilder& AddStringQueryParam(const FString& ParamName, const FString& ParamValue, const bool IgnoreEmpty = true);
	RWSUtilsUrlBuilder& AddArrayQueryParam(const FString& ParamName, const TArray<FString>& ParamValueArray, const bool IgnoreEmpty = true, const bool AsOneParam = false);
	RWSUtilsUrlBuilder& AddNumberQueryParam(const FString& ParamName, const int32 ParamValue);
	RWSUtilsUrlBuilder& AddBoolQueryParam(const FString& ParamName, const bool ParamValue, const bool AsNumber = true);

private:
	FString Url;

	TArray<TPair<FString, FString>> PathParams;

	TArray<TPair<FString, FString>> StringQueryParams;
	TArray<TPair<FString, int32>> NumberQueryParams;
};