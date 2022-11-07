// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/ECS/GameEntityComponent.h"
#include "ItemComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEFRAMEWORK_API UItemComponent : public UGameEntityComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemComponent();

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = "Asset")
	int32 ItemId;					///< ��Ʒid

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = "Asset")
	int32 ItemCount;				///< ��Ʒ����

protected:
    virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
};
