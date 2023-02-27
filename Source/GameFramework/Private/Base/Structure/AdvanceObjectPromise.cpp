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

			if (this) {
				SetReadyToDestroy();
			}
		}));
		Future = Promise->GetFuture();

		SetFlags(RF_StrongRefOnFrame);
		auto World = GetWorld();
		ensure(World);
		if (World) {
			World->GetGameInstance()->RegisterReferencedObject(this);
		}
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

UAdvanceObjectPromise* UAdvanceObjectPromise::K2_AddSuccessListener(const FOnObjectPromiseDynSuccess& SuccessCallback) {
	return AddSuccessListener(FOnObjectPromiseSuccess::CreateLambda([SuccessCallback](UObject* ReturnResult) {
		SuccessCallback.ExecuteIfBound(ReturnResult);
	}));
}

UAdvanceObjectPromise* UAdvanceObjectPromise::AddSuccessListener(const FOnObjectPromiseSuccess& SuccessCallback) {
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
	return this;
}

UAdvanceObjectPromise* UAdvanceObjectPromise::K2_AddFailureListener(const FOnObjectPromiseDynFail& FailureCallback) {
	return AddFailureListener(FOnObjectPromiseFail::CreateLambda([FailureCallback](const FString& ReturnFailReason) {
		FailureCallback.ExecuteIfBound(ReturnFailReason);
	}));
}

UAdvanceObjectPromise* UAdvanceObjectPromise::AddFailureListener(const FOnObjectPromiseFail& FailureCallback) {
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
	return this;
}

void UAdvanceObjectPromise::BindOtherPromise(UAdvanceObjectPromise* OtherPromise) {
	OtherPromise->AddSuccessListener(FOnObjectPromiseSuccess::CreateUObject(this, &UAdvanceObjectPromise::BindOtherPromiseSuccessCallback))
		->AddFailureListener(FOnObjectPromiseFail::CreateUObject(this, &UAdvanceObjectPromise::BindOtherPromiseFailureCallback));
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

void UAdvanceObjectPromise::SetReadyToDestroy() {
	if (HasAnyFlags(RF_StrongRefOnFrame)) {
		ClearFlags(RF_StrongRefOnFrame);
	}

	auto World = GetWorld();
	if (World) {
		auto GameInstance = World->GetGameInstance();
		if (GameInstance) {
			GameInstance->UnregisterReferencedObject(this);
		}
	}
}