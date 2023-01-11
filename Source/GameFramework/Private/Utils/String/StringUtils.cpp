// Fill out your copyright notice in the Description page of Project Settings.

#include "StringUtils.h"

bool UStringUtils::StartsWith(const FString& SearchStr, const FString& MatchStr, bool IgnoreCase) {
    return SearchStr.StartsWith(MatchStr, IgnoreCase ? ESearchCase::IgnoreCase : ESearchCase::CaseSensitive);
}

bool UStringUtils::EndsWith(const FString& SearchStr, const FString& MatchStr, bool IgnoreCase) {
    return SearchStr.EndsWith(MatchStr, IgnoreCase ? ESearchCase::IgnoreCase : ESearchCase::CaseSensitive);
}