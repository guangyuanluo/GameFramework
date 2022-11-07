// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/ECS/GameEntityComponent.h"
#include "PlayerComponent.generated.h"


UCLASS(BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEFRAMEWORK_API UPlayerComponent : public UGameEntityComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerComponent();

	UPROPERTY(replicated, BlueprintReadOnly, Category = "Player")
	FString RoleID;

	UPROPERTY(replicated, BlueprintReadWrite, Category = "Player")
	int UnitID;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

public:	
		
};