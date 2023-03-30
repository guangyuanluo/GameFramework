// Fill out your copyright notice in the Description page of Project Settings.

#include "StringUtils.h"

bool UStringUtils::StartsWith(const FString& SearchStr, const FString& MatchStr, bool IgnoreCase) {
    return SearchStr.StartsWith(MatchStr, IgnoreCase ? ESearchCase::IgnoreCase : ESearchCase::CaseSensitive);
}

bool UStringUtils::EndsWith(const FString& SearchStr, const FString& MatchStr, bool IgnoreCase) {
    return SearchStr.EndsWith(MatchStr, IgnoreCase ? ESearchCase::IgnoreCase : ESearchCase::CaseSensitive);
}

int64 UStringUtils::Conv_StringToInt64(const FString& InString) {
    return FCString::Atoi64(*InString);
}

FString UStringUtils::BinaryToString(const TArray<uint8>& Data) {
    if (Data.Num() == 0) {
        return FString();
    }
    else {
        uint8* DataPtr = (uint8*)(void*)&(Data[0]);
        check(DataPtr[0] == Data[0]);
        return BinaryToString(DataPtr, Data.Num());
    }
}

FString UStringUtils::BinaryToString(uint8* Data, int num) {
    FString Result = BytesToString(Data, num);
    check(Result.Len() == num);
    return Result;
}

TArray<uint8> UStringUtils::StringToBinary(const FString& Str) {
    TArray<uint8> Bytes;
    Bytes.SetNumUninitialized(Str.Len());
    int32 NumBytes = StringToBytes(Str, Bytes.GetData(), Bytes.Num());

    return Bytes;
}