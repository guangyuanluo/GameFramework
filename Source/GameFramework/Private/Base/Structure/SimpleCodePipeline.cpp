#include "SimpleCodePipeline.h"
#include "AdvanceObjectPromise.h"
#include "Async/Async.h"

USimpleCodePipeline* USimpleCodePipeline::K2_PushSyncFunction(const FPipelineSyncDynFunction& SyncFunction, bool IsFraming) {
	return PushSyncFunction(FPipelineSyncFunction::CreateLambda([SyncFunction](UObject* CallbackContext) {
		SyncFunction.ExecuteIfBound(CallbackContext);
	}));
}

USimpleCodePipeline* USimpleCodePipeline::PushSyncFunction(const FPipelineSyncFunction& SyncFunction, bool IsFraming) {
	check(ExecutePromise == nullptr);

	FPipelineFunction PipelineFunction;
	PipelineFunction.IsAsyncFunction = false;
	PipelineFunction.IsFraming = IsFraming;
	PipelineFunction.SyncFunction = SyncFunction;
	Executors.Add(PipelineFunction);

	return this;
}

USimpleCodePipeline* USimpleCodePipeline::K2_PushAsyncFunction(const FPipelineAsyncDynFunction& AsyncFunction, bool IsFraming) {
	return PushAsyncFunction(FPipelineAsyncFunction::CreateLambda([AsyncFunction](UObject* CallbackContext) {
		return AsyncFunction.Execute(CallbackContext);
	}));
}

USimpleCodePipeline* USimpleCodePipeline::PushAsyncFunction(const FPipelineAsyncFunction& AsyncFunction, bool IsFraming) {
	check(ExecutePromise == nullptr);

	FPipelineFunction PipelineFunction;
	PipelineFunction.IsAsyncFunction = true;
	PipelineFunction.IsFraming = IsFraming;
	PipelineFunction.AsyncFunction = AsyncFunction;
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

	return ExecutePromise;
}

void USimpleCodePipeline::Cancel() {
	check(ExecutePromise);
	ExecutePromise->TryFailure(TEXT("UserCancel"));
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
	if (ExecutePromise->IsReady()) {
		//已经完成了，应该是被流程中断了
		return;
	}
	auto Executor = Executors[CurrentExecuteIndex];
	if (Executor.IsAsyncFunction) {
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
	CurrentExecuteIndex = CurrentExecuteIndex + 1;
	//进度通知
	OnCodePipelineProgressChange.Broadcast(this, CurrentExecuteIndex, Executors.Num());

	if (CurrentExecuteIndex >= Executors.Num()) {
		ExecutePromise->TrySuccess(nullptr);
	}
	else {
		RunExecutor();
	}
}