#include "BaseConditionWidgetFactories.h"
#include "ConditionWidgetFactory.h"
#include "ConditionWidgetManager.h"
//farm
#include "Global/Conditions/PlayerFarmCondition.h"
//finish quest
#include "Global/Conditions/PlayerQuestFinishCondition.h"
//collect item
#include "Global/Conditions/PlayerCollectItemCondition.h"
//collect money
#include "Global/Conditions/PlayerCollectMoneyCondition.h"
//hunt
#include "Global/Conditions/PlayerHuntCondition.h"
//move to
#include "Global/Conditions/ArrivingAtCondition.h"
//consume item
#include "Global/Conditions/PlayerConsumeItemCondition.h"
//consume money
#include "Global/Conditions/PlayerConsumeMoneyCondition.h"
//player reach exp level
#include "Global/Conditions/PlayerReachExpLevelCondition.h"
//intimacy request
#include "Global/Conditions/PlayerIntimacyRequestCondition.h"
//unit reach exp level
#include "Global/Conditions/UnitReachExpLevelCondition.h"
//talk to
#include "Global/Conditions/TalkToCondition.h"
//acquire npcs
#include "Global/Conditions/AcquireNPCsCondition.h"
//deduct item
#include "Global/Conditions/PlayerDeductItemCondition.h"
//deduct money
#include "Global/Conditions/PlayerDeductMoneyCondition.h"
//clost to npc
#include "Global/Conditions/CloseToNPCCondition.h"
//skill reach level
#include "Global/Conditions/PlayerSkillReachLevelCondition.h"

#include "GameInstance/CoreGameInstance.h"

#include "Modules/Money/MoneySetting.h"
#include "Modules/Exp/ExpSetting.h"
#include "Modules/Item/ItemSetting.h"
#include "Modules/Unit/UnitSetting.h"
#include "Modules/Skill/SkillConfigTableRow.h"

#include "Developer/AssetTools/Public/AssetToolsModule.h"
#include "GameFrameworkEditor.h"
#include "GameFrameworkEditorWidgetTool.h"

namespace BaseConditionFactory {
    class ConditionWidgetFactory_CollectItem : public ConditionWidgetFactory {
    public:
        virtual TSubclassOf<class UCoreCondition> GetConditionClass() override
        {
            return UPlayerCollectItemCondition::StaticClass();
        }
        virtual bool CanCreateCondition() override {
            FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
            auto EditorWidgetTool = GameFrameworkEditorModule.GetEditorWidgetTool();
            auto ItemSource = EditorWidgetTool->GetItemInfoSource();
            if (ItemSource.Num() == 0) {
                EditorWidgetTool->ShowNotification(FText::FromString(TEXT("没有可选的物品，请先配置物品")), 5.0f);
                return false;
            }
            return true;
        }
        virtual void PostInitConditionCreated(class UCoreCondition* Condition) override {
            FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
            auto EditorWidgetTool = GameFrameworkEditorModule.GetEditorWidgetTool();
            auto ItemSource = EditorWidgetTool->GetItemInfoSource();
            UPlayerCollectItemCondition* CollectItemCondition = Cast<UPlayerCollectItemCondition>(Condition);
            FItemConfigTableRow* RowData = (FItemConfigTableRow*)(ItemSource[0]->ConfigTableRow);
            CollectItemCondition->ItemIDContainer.ItemID = RowData->ItemId;
            CollectItemCondition->ItemCount = 1;
        }
    };

    class ConditionWidgetFactory_CollectMoney : public ConditionWidgetFactory {
    public:
        virtual TSubclassOf<class UCoreCondition> GetConditionClass() override {
            return UPlayerCollectMoneyCondition::StaticClass();
        }
        virtual bool CanCreateCondition() override {
            FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
            auto EditorWidgetTool = GameFrameworkEditorModule.GetEditorWidgetTool();
            auto MoneyTypeSource = EditorWidgetTool->GetMoneyTypeSource();
            if (MoneyTypeSource.Num() == 0) {
                EditorWidgetTool->ShowNotification(FText::FromString(TEXT("没有可选的Money类型，请先配置Money类型")), 5.0f);
                return false;
            }
            return true;
        }
        virtual void PostInitConditionCreated(class UCoreCondition* Condition) override {
            FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
            auto EditorWidgetTool = GameFrameworkEditorModule.GetEditorWidgetTool();
            auto MoneyTypeSource = EditorWidgetTool->GetMoneyTypeSource();
            UPlayerCollectMoneyCondition* CollectMoneyCondition = Cast<UPlayerCollectMoneyCondition>(Condition);
            FMoneyTypeConfigTableRow* RowData = (FMoneyTypeConfigTableRow*)(MoneyTypeSource[0]->ConfigTableRow);
            CollectMoneyCondition->MoneyTypeContainer.MoneyType = RowData->MoneyTypeId;
            CollectMoneyCondition->MoneyCount = 1;
        }
    };

    class ConditionWidgetFactory_PlayerConsumeItem : public ConditionWidgetFactory {
    public:
        virtual TSubclassOf<class UCoreCondition> GetConditionClass() override {
            return UPlayerConsumeItemCondition::StaticClass();
        }
        virtual bool CanCreateCondition() override {
            FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
            auto EditorWidgetTool = GameFrameworkEditorModule.GetEditorWidgetTool();
            auto ItemSource = EditorWidgetTool->GetItemInfoSource();
            if (ItemSource.Num() == 0) {
                EditorWidgetTool->ShowNotification(FText::FromString(TEXT("没有可选的物品，请先配置物品")), 5.0f);
                return false;
            }
            return true;
        }
        virtual void PostInitConditionCreated(class UCoreCondition* Condition) override {
            FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
            auto EditorWidgetTool = GameFrameworkEditorModule.GetEditorWidgetTool();
            auto ItemSource = EditorWidgetTool->GetItemInfoSource();
            UPlayerConsumeItemCondition* ConsumeItemCondition = Cast<UPlayerConsumeItemCondition>(Condition);
            FItemConfigTableRow* RowData = (FItemConfigTableRow*)(ItemSource[0]->ConfigTableRow);
            ConsumeItemCondition->ItemIDContainer.ItemID = RowData->ItemId;
            ConsumeItemCondition->ItemCount = 1;
        }
    };

    class ConditionWidgetFactory_PlayerConsumeMoney : public ConditionWidgetFactory {
    public:
        virtual TSubclassOf<class UCoreCondition> GetConditionClass() override {
            return UPlayerConsumeMoneyCondition::StaticClass();
        }
        virtual bool CanCreateCondition() override {
            FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
            auto EditorWidgetTool = GameFrameworkEditorModule.GetEditorWidgetTool();
            auto MoneyTypeSource = EditorWidgetTool->GetMoneyTypeSource();
            if (MoneyTypeSource.Num() == 0) {
                EditorWidgetTool->ShowNotification(FText::FromString(TEXT("没有可选的Money类型，请先配置Money类型")), 5.0f);
                return false;
            }
            return true;
        }
        virtual void PostInitConditionCreated(class UCoreCondition* Condition) override {
            FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
            auto EditorWidgetTool = GameFrameworkEditorModule.GetEditorWidgetTool();
            auto MoneyTypeSource = EditorWidgetTool->GetMoneyTypeSource();
            UPlayerConsumeMoneyCondition* ConsumeMoneyCondition = Cast<UPlayerConsumeMoneyCondition>(Condition);
            FMoneyTypeConfigTableRow* RowData = (FMoneyTypeConfigTableRow*)(MoneyTypeSource[0]->ConfigTableRow);
            ConsumeMoneyCondition->MoneyTypeContainer.MoneyType = RowData->MoneyTypeId;
            ConsumeMoneyCondition->MoneyCount = 1;
        }
    };

    class ConditionWidgetFactory_PlayerFarm : public ConditionWidgetFactory {
    public:
        virtual TSubclassOf<class UCoreCondition> GetConditionClass() override {
            return UPlayerFarmCondition::StaticClass();
        }
        virtual bool CanCreateCondition() override {
            FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
            auto EditorWidgetTool = GameFrameworkEditorModule.GetEditorWidgetTool();
            auto UnitInfoSource = EditorWidgetTool->GetUnitInfoSource();
            if (UnitInfoSource.Num() == 0) {
                EditorWidgetTool->ShowNotification(FText::FromString(TEXT("没有可选的单位，请先配置单位")), 5.0f);
                return false;
            }
            return true;
        }
        virtual void PostInitConditionCreated(class UCoreCondition* Condition) override {
            FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
            auto EditorWidgetTool = GameFrameworkEditorModule.GetEditorWidgetTool();
            auto UnitInfoSource = EditorWidgetTool->GetUnitInfoSource();

            UPlayerFarmCondition* FarmCondition = Cast<UPlayerFarmCondition>(Condition);
            FUnitInfoConfigTableRow* RowData = (FUnitInfoConfigTableRow*)(UnitInfoSource[0]->ConfigTableRow);
            FarmCondition->UnitIDContainer.UnitID = RowData->UnitId;
            FarmCondition->Count = 1;
        }
    };

    class ConditionWidgetFactory_PlayerHunt : public ConditionWidgetFactory {
    public:
        virtual TSubclassOf<class UCoreCondition> GetConditionClass() override {
            return UPlayerHuntCondition::StaticClass();
        }
        virtual bool CanCreateCondition() override {
            return true;
        }
        virtual void PostInitConditionCreated(class UCoreCondition* Condition) override {
        }
    };

    class ConditionWidgetFactory_ArrivingAt : public ConditionWidgetFactory {
    public:
        virtual TSubclassOf<class UCoreCondition> GetConditionClass() override {
            return UArrivingAtCondition::StaticClass();
        }
        virtual bool CanCreateCondition() override {
            return true;
        }
        virtual void PostInitConditionCreated(class UCoreCondition* Condition) override {
        }
    };

    class ConditionWidgetFactory_PlayerReachExpLevel : public ConditionWidgetFactory {
    public:
        virtual TSubclassOf<class UCoreCondition> GetConditionClass() override {
            return UPlayerReachExpLevelCondition::StaticClass();
        }
        virtual bool CanCreateCondition() override {
            FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
            auto EditorWidgetTool = GameFrameworkEditorModule.GetEditorWidgetTool();
            auto ExpTypeSource = EditorWidgetTool->GetExpTypeSource();
            if (ExpTypeSource.Num() == 0) {
                EditorWidgetTool->ShowNotification(FText::FromString(TEXT("没有可选的Exp类型，请先配置Exp类型")), 5.0f);
                return false;
            }
            return true;
        }
        virtual void PostInitConditionCreated(class UCoreCondition* Condition) override {
            FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
            auto EditorWidgetTool = GameFrameworkEditorModule.GetEditorWidgetTool();
            auto ExpTypeSource = EditorWidgetTool->GetExpTypeSource();

            UPlayerReachExpLevelCondition* ReachExpLevelCondition = Cast<UPlayerReachExpLevelCondition>(Condition);
            FExpTypeConfigTableRow* RowData = (FExpTypeConfigTableRow*)(ExpTypeSource[0]->ConfigTableRow);

            ReachExpLevelCondition->ExpTypeContainer.ExpType = RowData->ExpTypeId;
            ReachExpLevelCondition->ExpLevel = 1;
        }
    };

    class ConditionWidgetFactory_PlayerIntimacyRequest : public ConditionWidgetFactory {
    public:
        virtual TSubclassOf<class UCoreCondition> GetConditionClass() override {
            return UPlayerIntimacyRequestCondition::StaticClass();
        }
        virtual bool CanCreateCondition() override {
            FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
            auto EditorWidgetTool = GameFrameworkEditorModule.GetEditorWidgetTool();
            auto UnitInfoSource = EditorWidgetTool->GetUnitInfoSource();
            if (UnitInfoSource.Num() == 0) {
                EditorWidgetTool->ShowNotification(FText::FromString(TEXT("没有可选的单位，请先配置单位")), 5.0f);
                return false;
            }
            return true;
        }
        virtual void PostInitConditionCreated(class UCoreCondition* Condition) override {
            FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
            auto EditorWidgetTool = GameFrameworkEditorModule.GetEditorWidgetTool();
            auto UnitInfoSource = EditorWidgetTool->GetUnitInfoSource();

            UPlayerIntimacyRequestCondition* IntimacyRequestCondition = Cast<UPlayerIntimacyRequestCondition>(Condition);
            FUnitInfoConfigTableRow* RowData = (FUnitInfoConfigTableRow*)(UnitInfoSource[0]->ConfigTableRow);
            IntimacyRequestCondition->NPCIDContainer.UnitID = RowData->UnitId;
            IntimacyRequestCondition->IntimacyRequest = 0;
            IntimacyRequestCondition->Compare = EIntimacyCompare::Above;
        }
    };

	class ConditionWidgetFactory_TalkTo : public ConditionWidgetFactory {
	public:
		virtual TSubclassOf<class UCoreCondition> GetConditionClass() override {
			return UTalkToCondition::StaticClass();
		}
        virtual bool CanCreateCondition() override {
            FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
            auto EditorWidgetTool = GameFrameworkEditorModule.GetEditorWidgetTool();
            auto UnitInfoSource = EditorWidgetTool->GetUnitInfoSource();
            if (UnitInfoSource.Num() == 0) {
                EditorWidgetTool->ShowNotification(FText::FromString(TEXT("没有可选的单位，请先配置单位")), 5.0f);
                return false;
            }
            return true;
        }
        virtual void PostInitConditionCreated(class UCoreCondition* Condition) override {
            FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
            auto EditorWidgetTool = GameFrameworkEditorModule.GetEditorWidgetTool();
            auto UnitInfoSource = EditorWidgetTool->GetUnitInfoSource();

            UTalkToCondition* TalkToCondition = Cast<UTalkToCondition>(Condition);
            FUnitInfoConfigTableRow* RowData = (FUnitInfoConfigTableRow*)(UnitInfoSource[0]->ConfigTableRow);
            TalkToCondition->UnitIDContainer.UnitID = RowData->UnitId;
        }
	};

	class ConditionWidgetFactory_AcquireNPCs : public ConditionWidgetFactory {
	public:
		virtual TSubclassOf<class UCoreCondition> GetConditionClass() override {
			return UAcquireNPCsCondition::StaticClass();
		}
        virtual bool CanCreateCondition() override {
            FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
            auto EditorWidgetTool = GameFrameworkEditorModule.GetEditorWidgetTool();
            auto UnitInfoSource = EditorWidgetTool->GetUnitInfoSource();
            if (UnitInfoSource.Num() == 0) {
                EditorWidgetTool->ShowNotification(FText::FromString(TEXT("没有可选的单位，请先配置单位")), 5.0f);
                return false;
            }
            return true;
        }
        virtual void PostInitConditionCreated(class UCoreCondition* Condition) override {
            FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
            auto EditorWidgetTool = GameFrameworkEditorModule.GetEditorWidgetTool();
            auto UnitInfoSource = EditorWidgetTool->GetUnitInfoSource();

            UAcquireNPCsCondition* AcquireNPCsCondition = Cast<UAcquireNPCsCondition>(Condition);
            FUnitInfoConfigTableRow* RowData = (FUnitInfoConfigTableRow*)(UnitInfoSource[0]->ConfigTableRow);
            FUnitIDContainer IDContainer;
            IDContainer.UnitID = RowData->UnitId;
            AcquireNPCsCondition->UnitIDContainers.Add(IDContainer);
        }
	};

	class ConditionWidgetFactory_PlayerDeductItem : public ConditionWidgetFactory {
	public:
		virtual TSubclassOf<class UCoreCondition> GetConditionClass() override {
			return UPlayerDeductItemCondition::StaticClass();
		}
        virtual bool CanCreateCondition() override {
            FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
            auto EditorWidgetTool = GameFrameworkEditorModule.GetEditorWidgetTool();
            auto ItemSource = EditorWidgetTool->GetItemInfoSource();
            if (ItemSource.Num() == 0) {
                EditorWidgetTool->ShowNotification(FText::FromString(TEXT("没有可选的物品，请先配置物品")), 5.0f);
                return false;
            }
            return true;
        }
        virtual void PostInitConditionCreated(class UCoreCondition* Condition) override {
            
        }
	};

	class ConditionWidgetFactory_PlayerDeductMoney : public ConditionWidgetFactory {
	public:
		virtual TSubclassOf<class UCoreCondition> GetConditionClass() override {
			return UPlayerDeductMoneyCondition::StaticClass();
		}
        virtual bool CanCreateCondition() override {
            FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
            auto EditorWidgetTool = GameFrameworkEditorModule.GetEditorWidgetTool();
            auto MoneyTypeSource = EditorWidgetTool->GetMoneyTypeSource();
            if (MoneyTypeSource.Num() == 0) {
                EditorWidgetTool->ShowNotification(FText::FromString(TEXT("没有可选的Money类型，请先配置Money类型")), 5.0f);
                return false;
            }
            return true;
        }
        virtual void PostInitConditionCreated(class UCoreCondition* Condition) override {

        }
	};

	class ConditionWidgetFactory_CloseToNPC : public ConditionWidgetFactory {
	public:
		virtual TSubclassOf<class UCoreCondition> GetConditionClass() override {
			return UCloseToNPCCondition::StaticClass();
		}
        virtual bool CanCreateCondition() override {
            FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
            auto EditorWidgetTool = GameFrameworkEditorModule.GetEditorWidgetTool();
            auto UnitInfoSource = EditorWidgetTool->GetUnitInfoSource();
            if (UnitInfoSource.Num() == 0) {
                EditorWidgetTool->ShowNotification(FText::FromString(TEXT("没有可选的单位，请先配置单位")), 5.0f);
                return false;
            }
            return true;
        }
        virtual void PostInitConditionCreated(class UCoreCondition* Condition) override {
            FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
            auto EditorWidgetTool = GameFrameworkEditorModule.GetEditorWidgetTool();
            auto UnitInfoSource = EditorWidgetTool->GetUnitInfoSource();

            UCloseToNPCCondition* CloseToNPCCondition = Cast<UCloseToNPCCondition>(Condition);
            FUnitInfoConfigTableRow* RowData = (FUnitInfoConfigTableRow*)(UnitInfoSource[0]->ConfigTableRow);
            CloseToNPCCondition->UnitIDContainer.UnitID = RowData->UnitId;
        }
	};

	class ConditionWidgetFactory_SkillReachLevel : public ConditionWidgetFactory {
	public:
		virtual TSubclassOf<class UCoreCondition> GetConditionClass() override {
			return UPlayerSkillReachLevelCondition::StaticClass();
		}
        virtual bool CanCreateCondition() override {
            FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
            auto EditorWidgetTool = GameFrameworkEditorModule.GetEditorWidgetTool();
            auto SkillSource = EditorWidgetTool->GetSkillSource();
            if (SkillSource.Num() == 0) {
                EditorWidgetTool->ShowNotification(FText::FromString(TEXT("没有可选的技能，请先配置技能")), 5.0f);
                return false;
            }
            return true;
        }
        virtual void PostInitConditionCreated(class UCoreCondition* Condition) override {
            FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
            auto EditorWidgetTool = GameFrameworkEditorModule.GetEditorWidgetTool();
            auto SkillSource = EditorWidgetTool->GetSkillSource();

            UPlayerSkillReachLevelCondition* PlayerSkillReachLevelCondition = Cast<UPlayerSkillReachLevelCondition>(Condition);
            FSkillConfigTableRow* RowData = (FSkillConfigTableRow*)(SkillSource[0]->ConfigTableRow);
            PlayerSkillReachLevelCondition->Skill.SkillID = RowData->SkillId;
        }
	};
}

TArray<TSharedPtr<ConditionWidgetFactory>> BaseConditionWidgetFactories::BaseFactories = {
    TSharedPtr<ConditionWidgetFactory>(new BaseConditionFactory::ConditionWidgetFactory_CollectItem),
    TSharedPtr<ConditionWidgetFactory>(new BaseConditionFactory::ConditionWidgetFactory_CollectMoney),
    TSharedPtr<ConditionWidgetFactory>(new BaseConditionFactory::ConditionWidgetFactory_PlayerConsumeItem),
    TSharedPtr<ConditionWidgetFactory>(new BaseConditionFactory::ConditionWidgetFactory_PlayerConsumeMoney),
    TSharedPtr<ConditionWidgetFactory>(new BaseConditionFactory::ConditionWidgetFactory_PlayerFarm),
    TSharedPtr<ConditionWidgetFactory>(new BaseConditionFactory::ConditionWidgetFactory_PlayerHunt),
    TSharedPtr<ConditionWidgetFactory>(new BaseConditionFactory::ConditionWidgetFactory_ArrivingAt),
    TSharedPtr<ConditionWidgetFactory>(new BaseConditionFactory::ConditionWidgetFactory_PlayerReachExpLevel),
    TSharedPtr<ConditionWidgetFactory>(new BaseConditionFactory::ConditionWidgetFactory_PlayerIntimacyRequest),
	TSharedPtr<ConditionWidgetFactory>(new BaseConditionFactory::ConditionWidgetFactory_TalkTo),
	TSharedPtr<ConditionWidgetFactory>(new BaseConditionFactory::ConditionWidgetFactory_AcquireNPCs),
	TSharedPtr<ConditionWidgetFactory>(new BaseConditionFactory::ConditionWidgetFactory_PlayerDeductItem),
	TSharedPtr<ConditionWidgetFactory>(new BaseConditionFactory::ConditionWidgetFactory_PlayerDeductMoney),
	TSharedPtr<ConditionWidgetFactory>(new BaseConditionFactory::ConditionWidgetFactory_CloseToNPC),
	TSharedPtr<ConditionWidgetFactory>(new BaseConditionFactory::ConditionWidgetFactory_SkillReachLevel),
};

void BaseConditionWidgetFactories::Init()
{
    for (int Index = 0; Index < BaseFactories.Num(); ++Index)
    {
        ConditionWidgetManager::RegistConditionWidgetFactory(BaseFactories[Index]);
    }
}
void BaseConditionWidgetFactories::Uninit()
{
    for (int Index = 0; Index < BaseFactories.Num(); ++Index) {
        ConditionWidgetManager::UnregistConditionWidgetFactory(BaseFactories[Index]);
    }
}