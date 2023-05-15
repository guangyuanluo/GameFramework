#include "SimpleCodePipeline.h"
#include "AdvanceObjectPromise.h"
#include "Async/Async.h"

USimpleCodePipeline::USimpleCodePipeline(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	if (!HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject)) {
		SetFlags(RF_StrongRefOnFrame);

		auto World = GetWorld();
		ensure(World);
		if (World) {
			World->GetGameInstance()->RegisterReferencedObject(this);
		}
	}
}

USimpleCodePipeline* USimpleCodePipeline::K2_PushSyncFunction(const FPipelineSyncDynFunction& SyncFunction, bool Condition, bool IsFraming, const FString& DebugName) {
	if (!Condition) {
		return this;
	}
	return PushSyncFunction(FPipelineSyncFunction::CreateLambda([SyncFunction](UObject* CallbackContext) {
		SyncFunction.ExecuteIfBound(CallbackContext);
	}), IsFraming, DebugName);
}

USimpleCodePipeline* USimpleCodePipeline::PushSyncFunction(const FPipelineSyncFunction& SyncFunction, bool IsFraming, const FString& DebugName) {
	check(ExecutePromise == nullptr);

	FPipelineFunction PipelineFunction;
	PipelineFunction.IsAsyncFunction = false;
	PipelineFunction.IsFraming = IsFraming;
	PipelineFunction.SyncFunction = SyncFunction;
	PipelineFunction.DebugName = DebugName;
	Executors.Add(PipelineFunction);

	return this;
}

USimpleCodePipeline* USimpleCodePipeline::K2_PushAsyncFunction(const FPipelineAsyncDynFunction& AsyncFunction, bool Condition, bool IsFraming, const FString& DebugName) {
	if (!Condition) {
		return this;
	}
	return PushAsyncFunction(FPipelineAsyncFunction::CreateLambda([AsyncFunction](UObject* CallbackContext) {
		return AsyncFunction.Execute(CallbackContext);
	}), IsFraming, DebugName);
}

USimpleCodePipeline* USimpleCodePipeline::PushAsyncFunction(const FPipelineAsyncFunction& AsyncFunction, bool IsFraming, const FString& DebugName) {
	check(ExecutePromise == nullptr);

	FPipelineFunction PipelineFunction;
	PipelineFunction.IsAsyncFunction = true;
	PipelineFunction.IsFraming = IsFraming;
	PipelineFunction.AsyncFunction = AsyncFunction;
	PipelineFunction.DebugName = DebugName;
	Executors.Add(PipelineFunction);

	return this;
}

class UAdvanceObjectPromise* USimpleCodePipeline::Execute() {
	check(ExecutePromise == nullptr);

	ExecutePromise = NewObject<UAdvanceObjectPromise>(this);

	if (Executors.Num() == 0) {
		ExecutePromise->TrySuccess(nullptr);
	}
	else {
		RunExecutor();
	}
	ExecutePromise->AddSuccessListener(FOnObjectPromiseSuccess::CreateUObject(this, &USimpleCodePipeline::ExecutePromiseCallbackSuccess))
		->AddFailureListener(FOnObjectPromiseFail::CreateUObject(this, &USimpleCodePipeline::ExecutePromiseCallbackFail));

	return ExecutePromise;
}

void USimpleCodePipeline::Cancel() {
	check(ExecutePromise);
	ExecutePromise->TryFailure(TEXT("UserCancel"));

	SetReadyToDestroy();
}

void USimpleCodePipeline::RunExecutor() {
	auto Executor = Executors[CurrentExecuteIndex];
	if (Executor.IsFraming) {
		AsyncTask(ENamedThreads::GameThread, [this]() {
			ExecuteNext();
		});
	}
	else {
		ExecuteNext();
	}
}

void USimpleCodePipeline::ExecuteNext() {
	if (ShowDebugInfo) {
		UE_LOG(LogTemp, Log, TEXT("%s USimpleCodePipeline::ExecuteNext"), *DebugPipelineName);
	}
	if (ExecutePromise->IsReady()) {
		//已经完成了，应该是被流程中断了
		if (ShowDebugInfo) {
			UE_LOG(LogTemp, Log, TEXT("%s USimpleCodePipeline::ExecuteNext already complete"), *DebugPipelineName);
		}
		return;
	}
	auto Executor = Executors[CurrentExecuteIndex];
	if (Executor.IsAsyncFunction) {
		if (ShowDebugInfo) {
			UE_LOG(LogTemp, Log, TEXT("%s USimpleCodePipeline::Executor ExecuteNext Start%s"), *DebugPipelineName, *Executor.DebugName);
		}
		UAdvanceObjectPromise* CurrentExecutePromise = Executor.AsyncFunction.Execute(Context);
		CurrentExecutePromise->AddSuccessListener(FOnObjectPromiseSuccess::CreateUObject(this, &USimpleCodePipeline::ExecutorCallbackSuccess))
			->AddFailureListener(FOnObjectPromiseFail::CreateUObject(this, &USimpleCodePipeline::ExecutorCallbackFail));
	}
	else {
		Executor.SyncFunction.Execute(Context);
		ExecutorExecuteComplete();
	}
}

void USimpleCodePipeline::ExecutorCallbackSuccess(UObject* Result) {
	ExecutorExecuteComplete();
}

void USimpleCodePipeline::ExecutorCallbackFail(const FString& FailureReason) {
	UE_LOG(LogTemp, Warning, TEXT("USimpleCodePipeline::ExecutorCallbackFail %s CurrentIndex:%d"), *DebugPipelineName, CurrentExecuteIndex);

	ExecutePromise->TryFailure(FailureReason);
}

void USimpleCodePipeline::ExecutorExecuteComplete() {
	//已经完成了，应该是被流程中断了
	if (ShowDebugInfo) {
		UE_LOG(LogTemp, Log, TEXT("%s USimpleCodePipeline::Executor ExecuteComplete %s"), *DebugPipelineName, *Executors[CurrentExecuteIndex].DebugName);
	}

	CurrentExecuteIndex = CurrentExecuteIndex + 1;
	//进度通知
	OnCodePipelineProgressChange.Broadcast(this, CurrentExecuteIndex, Executors.Num());

	if (CurrentExecuteIndex >= Executors.Num()) {
		ExecutePromise->TrySuccess(nullptr);

		SetReadyToDestroy();
	}
	else {
		RunExecutor();
	}
}

void USimpleCodePipeline::SetReadyToDestroy() {
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

void USimpleCodePipeline::ExecutePromiseCallbackSuccess(UObject* Result) {
	OnCodePipelineFinish.Broadcast(this, CurrentExecuteIndex, ExecutePromise);
}

void USimpleCodePipeline::ExecutePromiseCallbackFail(const FString& FailureReason) {
	OnCodePipelineFinish.Broadcast(this, CurrentExecuteIndex, ExecutePromise);
}