#include "AdvanceObjectPromise.h"

const FString TimeoutFailureReason = TEXT("Timeout");

UAdvanceObjectPromise::UAdvanceObjectPromise(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
    if (!HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject)) {
        Promise = MakeShareable(new TPromise<bool>([this]() {
            if (Future.Get()) {
                //成功
                for (auto& SuccessCallback : OnSuccess) {
                    SuccessCallback.ExecuteIfBound(Result);
                }
            }
            else {
                //失败
                for (auto& FailureCallback : OnFailure) {
                    FailureCallback.ExecuteIfBound(FailureReason);
                }
            }
            OnSuccess.Empty();
            OnFailure.Empty();
        }));
        Future = Promise->GetFuture();
    }
}

UAdvanceObjectPromise::~UAdvanceObjectPromise() {
    if (!HaveSet) {
        OnSuccess.Empty();
        OnFailure.Empty();
        if (Promise) {
            Promise->SetValue(false);
        }
    }
}

void UAdvanceObjectPromise::TrySuccess(UObject* InResult) {

    if (HaveSet) {
        return;
    }
    HaveSet = true;

    Result = InResult;

    ClearTimeout();

    Promise->SetValue(true);
}

void UAdvanceObjectPromise::TryFailure(const FString& InFailureReason) {
    if (HaveSet) {
        return;
    }

    FailureReason = InFailureReason;

    HaveSet = true;

    ClearTimeout();

    Promise->SetValue(false);
}

bool UAdvanceObjectPromise::IsReady() const {
    return Future.IsReady();
}

bool UAdvanceObjectPromise::IsSuccess() const {
    check(Future.IsReady() && TEXT("请确保已经检查promise是否设置过值，再调用此接口，不然是无效调用"));
    if (Future.IsReady()) {
        return Future.Get();
    }
    return false;
}

UObject* UAdvanceObjectPromise::GetResult() const {
    ensureMsgf(Future.IsReady(), TEXT("请确保已经检查promise是否设置过值，再调用此接口，不然是无效调用"));
    return Result;
}

const FString& UAdvanceObjectPromise::GetFailureReason() const {
    ensureMsgf(Future.IsReady(), TEXT("请确保已经检查promise是否设置过值，再调用此接口，不然是无效调用"));
    return FailureReason;
}

void UAdvanceObjectPromise::AddSuccessListener(FOnObjectPromiseSuccess SuccessCallback) {
    if (Future.IsReady()) {
        bool IsSuccess = Future.Get();
        if (IsSuccess) {
            //已经设置过值了，直接通知
            SuccessCallback.ExecuteIfBound(Result);
        }
    }
    else {
        OnSuccess.Add(SuccessCallback);
    }
}

void UAdvanceObjectPromise::AddFailureListener(FOnObjectPromiseFail FailureCallback) {
    if (Future.IsReady()) {
        bool IsSuccess = Future.Get();
        if (!IsSuccess) {
            //已经设置过值了，直接通知
            FailureCallback.ExecuteIfBound(FailureReason);
        }
    }
    else {
        OnFailure.Add(FailureCallback);
    }
}

void UAdvanceObjectPromise::BindOtherPromise(UAdvanceObjectPromise* OtherPromise) {
    FOnObjectPromiseSuccess SuccessCallback;
    SuccessCallback.BindUFunction(this, TEXT("BindOtherPromiseSuccessCallback"));
    OtherPromise->AddSuccessListener(SuccessCallback);

    FOnObjectPromiseFail FailCallback;
    FailCallback.BindUFunction(this, TEXT("BindOtherPromiseFailureCallback"));
    OtherPromise->AddFailureListener(FailCallback);
}

void UAdvanceObjectPromise::SetTimeout(float Timeout) {
    auto World = GetWorld();
    if (!World) {
        auto Outer = GetOuter();
        if (Outer) {
            World = Outer->GetWorld();
        }
    }
    check(World);

    World->GetTimerManager().SetTimer(TimeoutHandle, this, &UAdvanceObjectPromise::TimeoutCallback, Timeout);
}

void UAdvanceObjectPromise::BindOtherPromiseSuccessCallback(UObject* InResult) {
    TrySuccess(InResult);
}

void UAdvanceObjectPromise::BindOtherPromiseFailureCallback(const FString& InFailureReason) {
    TryFailure(InFailureReason);
}

void UAdvanceObjectPromise::TimeoutCallback() {
    TryFailure(TimeoutFailureReason);
}

void UAdvanceObjectPromise::ClearTimeout() {
    if (TimeoutHandle.IsValid()) {
        auto World = GetWorld();
        if (!World) {
            auto Outer = GetOuter();
            if (Outer) {
                World = Outer->GetWorld();
            }
        }
        if (World) {
            World->GetTimerManager().ClearTimer(TimeoutHandle);
        }
        TimeoutHandle.Invalidate();
    }
}