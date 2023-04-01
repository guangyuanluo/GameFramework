#include "BaseConditionWidgetFactories.h"
#include "ConditionWidgetFactory.h"
#include "ConditionWidgetManager.h"
//farm
#include "Global/Conditions/PlayerFarmCondition.h"
#include "Global/Conditions/PlayerFarmConditionProgress.h"
//finish quest
#include "Global/Conditions/PlayerQuestFinishCondition.h"
#include "Global/Conditions/PlayerQuestFinishConditionProgress.h"
//collect item
#include "Global/Conditions/PlayerCollectItemCondition.h"
#include "Global/Conditions/PlayerCollectItemConditionProgress.h"
//collect money
#include "Global/Conditions/PlayerCollectMoneyCondition.h"
#include "Global/Conditions/PlayerCollectMoneyConditionProgress.h"
//hunt
#include "Global/Conditions/PlayerHuntCondition.h"
#include "Global/Conditions/PlayerHuntConditionProgress.h"
//move to
#include "Global/Conditions/ArrivingAtCondition.h"
#include "Global/Conditions/ArrivingAtConditionProgress.h"
//consume item
#include "Global/Conditions/PlayerConsumeItemCondition.h"
#include "Global/Conditions/PlayerConsumeItemConditionProgress.h"
//consume money
#include "Global/Conditions/PlayerConsumeMoneyCondition.h"
#include "Global/Conditions/PlayerConsumeMoneyConditionProgress.h"
//player reach exp level
#include "Global/Conditions/PlayerReachExpLevelCondition.h"
#include "Global/Conditions/PlayerReachExpLevelConditionProgress.h"
//intimacy request
#include "Global/Conditions/PlayerIntimacyRequestCondition.h"
#include "Global/Conditions/PlayerIntimacyRequestConditionProgress.h"
//unit reach exp level
#include "Global/Conditions/UnitReachExpLevelCondition.h"
#include "Global/Conditions/UnitReachExpLevelConditionProgress.h"
//talk to
#include "Global/Conditions/TalkToCondition.h"
#include "Global/Conditions/TalkToConditionProgress.h"
//acquire npcs
#include "Global/Conditions/AcquireNPCsCondition.h"
#include "Global/Conditions/AcquireNPCsConditionProgress.h"
//deduct item
#include "Global/Conditions/PlayerDeductItemCondition.h"
#include "Global/Conditions/PlayerDeductItemConditionProgress.h"
//deduct money
#include "Global/Conditions/PlayerDeductMoneyCondition.h"
#include "Global/Conditions/PlayerDeductMoneyConditionProgress.h"
//clost to npc
#include "Global/Conditions/CloseToNPCCondition.h"
#include "Global/Conditions/CloseToNPCConditionProgress.h"
//skill reach level
#include "Global/Conditions/PlayerSkillReachLevelCondition.h"
#include "Global/Conditions/PlayerSkillReachLevelConditionProgress.h"

#include "GameInstance/CoreGameInstance.h"

#include "Modules/Money/MoneySetting.h"
#include "Modules/Exp/ExpSetting.h"
#include "Modules/Item/ItemSetting.h"
#include "Modules/Unit/UnitSetting.h"
#include "Modules/Skill/SkillConfigTableRow.h"

#include "Runtime/Slate/Public/Widgets/Input/SComboBox.h"
#include "Runtime/Slate/Public/Widgets/Input/SEditableTextBox.h"

#include "SConditionWidget.h"
#include "Widgets/Input/SNumericEntryBox.h"

#include "Developer/AssetTools/Public/AssetToolsModule.h"
#include "GameFrameworkEditor.h"
#include "GameFrameworkEditorWidgetTool.h"
#include "SRowTableRefBox.h"

namespace ConditionUI {
	class SConditionWidgetDefault : public SConditionWidget {
	public:
		SLATE_BEGIN_ARGS(SConditionWidget) {}

		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, UCoreCondition* InWidgetCondition, SVerticalBox::FSlot* InParentSlot, const FString& ConditionWidgetName) {
			SConditionWidget::Construct(SConditionWidget::FArguments(), InWidgetCondition);
			ParentSlot = InParentSlot;

			TSharedRef<SHorizontalBox> Panel = SNew(SHorizontalBox);

			ChildSlot
			[
				SNew(SExpandableArea)
				.HeaderPadding(FMargin(2.0, 2.0))
				.Padding(FMargin(10.0f, 2.0f))
				.BorderBackgroundColor(FLinearColor(.6, .6, .6, 1.0f))
				.BodyBorderBackgroundColor(FLinearColor(1.0, 0.0, 0.0))
				.HeaderContent()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Fill)
					[
						SNew(STextBlock)
						.Text(FText::FromString(ConditionWidgetName))
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(2)
					[
						SNew(SButton)
#if ENGINE_MAJOR_VERSION > 4
						.ButtonStyle(FAppStyle::Get(), "HoverHintOnly")
#else
						.ButtonStyle(FEditorStyle::Get(), "HoverHintOnly")
#endif
						.ForegroundColor(FSlateColor::UseForeground())
						.HAlign(HAlign_Center)
						.VAlign(VAlign_Center)
						.OnClicked(this, &SConditionWidgetDefault::RemoveClick)
						.Content()
						[
							SNew(STextBlock)
							.Text(FText::FromString("-"))
						]
						[
							SNew(SImage)
#if ENGINE_MAJOR_VERSION > 4
							.Image(FAppStyle::Get().GetBrush("Cross"))
#else
							.Image(FEditorStyle::Get().GetBrush("Cross"))
#endif
						]
					]
				]
				.BodyContent()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.HAlign(HAlign_Fill)
					[
						Panel
					]
				]
			];

			SHorizontalBox::FScopedWidgetSlotArguments AddWidgetSlot = Panel->AddSlot();
			AddWidgetSlot.AutoWidth();
			AddWidgetSlot.HAlign(HAlign_Fill);
			WidgetSlot = AddWidgetSlot.GetSlot();
		}

		FReply RemoveClick() {
			SConditionWidget::OnConditionWidgetPreremove.ExecuteIfBound(WidgetCondition);

			ParentSlot->DetachWidget();

			return FReply::Handled();
		}

	protected:
		void NotifyConditionChange() {
			SConditionWidget::OnConditionWidgetChange.ExecuteIfBound(WidgetCondition);
		}

	private:
		SVerticalBox::FSlot* ParentSlot;
	protected:
		SHorizontalBox::FSlot* WidgetSlot;
	};

	/*收集物品任务条件*/
	class CollectItemConditionWidget : public SConditionWidgetDefault {
	public:
		virtual ~CollectItemConditionWidget() {}

		SLATE_BEGIN_ARGS(CollectItemConditionWidget) {}

		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, UCoreCondition* InWidgetCondition, SVerticalBox::FSlot* InParentSlot, const FString& ConditionWidgetName) {
			SConditionWidgetDefault::Construct(SConditionWidgetDefault::FArguments(), InWidgetCondition, InParentSlot, ConditionWidgetName);

            UPlayerCollectItemCondition* CollectItemCondition = (UPlayerCollectItemCondition*)WidgetCondition;
            const UItemSetting* ItemSetting = GetDefault<UItemSetting>();
            auto ItemDataTable = ItemSetting->ItemTable.LoadSynchronous();
            ItemTableRefBox = SNew(SRowTableRefBox, ItemDataTable, CollectItemCondition->ItemId);
            ItemTableRefBox->RowSelectChanged.BindSP(this, &CollectItemConditionWidget::OnSelectionChanged);

			(*WidgetSlot)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					[
						SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SNew(SHorizontalBox)
							+ SHorizontalBox::Slot()
							[
								SNew(STextBlock)
								.Text(FText::FromString(TEXT("ItemId:")))
							]
							+ SHorizontalBox::Slot()
							[
                                ItemTableRefBox.ToSharedRef()
							]
						]
						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SNew(SHorizontalBox)
							+ SHorizontalBox::Slot()
							[
								SNew(STextBlock)
								.Text(FText::FromString(TEXT("Count:")))
							]
							+ SHorizontalBox::Slot()
							[
								SNew(SNumericEntryBox<int>)
								.Font(FCoreStyle::GetDefaultFontStyle("Regular", 8))
								.Value_Lambda([this] {
									UPlayerCollectItemCondition* CollectItemCondition = (UPlayerCollectItemCondition*)WidgetCondition;
									return CollectItemCondition->ItemCount;
								})
								.OnValueCommitted(this, &CollectItemConditionWidget::OnItemCountCommitted)
							]
						]
					]
				];
		}

	private:
        TSharedPtr<SRowTableRefBox> ItemTableRefBox;

        void OnSelectionChanged(int ItemId) {
			FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();

            UPlayerCollectItemCondition* CollectItemCondition = (UPlayerCollectItemCondition*)WidgetCondition;
            CollectItemCondition->ItemId = ItemId;
            NotifyConditionChange();
		}

		void OnItemCountCommitted(int NewValue, ETextCommit::Type InCommitType) {
			FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();

			if (NewValue > 0) {
				UPlayerCollectItemCondition* CollectItemCondition = (UPlayerCollectItemCondition*)WidgetCondition;
                CollectItemCondition->ItemCount = NewValue;
				NotifyConditionChange();
			}
			else {
				GameFrameworkEditorModule.GetEditorWidgetTool()->ShowNotification(FText::FromString(TEXT("itemCount必须大于0，回退操作")), 5.0);
			}
		}
	};

	/*收集money任务条件*/
	class CollectMoneyConditionWidget : public SConditionWidgetDefault {
	public:
		virtual ~CollectMoneyConditionWidget() {}

		SLATE_BEGIN_ARGS(CollectMoneyConditionWidget) {}

		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, UCoreCondition* InWidgetCondition, SVerticalBox::FSlot* InParentSlot, const FString& ConditionWidgetName) {
			SConditionWidgetDefault::Construct(SConditionWidgetDefault::FArguments(), InWidgetCondition, InParentSlot, ConditionWidgetName);
			
            UPlayerCollectMoneyCondition* CollectMoneyCondition = (UPlayerCollectMoneyCondition*)WidgetCondition;

            const UMoneySetting* MoneySetting = GetDefault<UMoneySetting>();
            auto MoneyTypeDataTable = MoneySetting->MoneyTypeTable.LoadSynchronous();

            MoneyTypeTableRefBox = SNew(SRowTableRefBox, MoneyTypeDataTable, CollectMoneyCondition->MoneyType);
            MoneyTypeTableRefBox->RowSelectChanged.BindSP(this, &CollectMoneyConditionWidget::OnSelectionChanged);

			(*WidgetSlot)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					[
						SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SNew(SHorizontalBox)
							+ SHorizontalBox::Slot()
							[
								SNew(STextBlock)
								.Text(FText::FromString(TEXT("MoneyType:")))
							]
							+ SHorizontalBox::Slot()
							[
                                MoneyTypeTableRefBox.ToSharedRef()
							]
						]
						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SNew(SHorizontalBox)
							+ SHorizontalBox::Slot()
							[
								SNew(STextBlock)
								.Text(FText::FromString(TEXT("Count:")))
							]
							+ SHorizontalBox::Slot()
							[
								SNew(SNumericEntryBox<int>)
								.Font(FCoreStyle::GetDefaultFontStyle("Regular", 8))
								.Value_Lambda([this] {
									UPlayerCollectMoneyCondition* CollectMoneyCondition = (UPlayerCollectMoneyCondition*)WidgetCondition;
									return CollectMoneyCondition->MoneyCount;
								})
								.OnValueCommitted(this, &CollectMoneyConditionWidget::OnMoneyCountCommitted)
							]
						]
					]
				];
		}

	private:
		TSharedPtr<SRowTableRefBox> MoneyTypeTableRefBox;

		void OnSelectionChanged(int MoneyTypeId) {
            UPlayerCollectMoneyCondition* CollectMoneyCondition = (UPlayerCollectMoneyCondition*)WidgetCondition;
            CollectMoneyCondition->MoneyType = MoneyTypeId;
            NotifyConditionChange();
		}

		void OnMoneyCountCommitted(int NewValue, ETextCommit::Type InCommitType) {
			FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();

			if (NewValue > 0) {
				UPlayerCollectMoneyCondition* CollectMoneyCondition = (UPlayerCollectMoneyCondition*)WidgetCondition;
                CollectMoneyCondition->MoneyCount = NewValue;
				NotifyConditionChange();
			}
			else {
				GameFrameworkEditorModule.GetEditorWidgetTool()->ShowNotification(FText::FromString(TEXT("MoneyCount必须大于0，回退操作")), 5.0);
			}
		}
	};

	/*消耗物品任务条件*/
	class ConsumeItemConditionWidget : public SConditionWidgetDefault {
	public:
		virtual ~ConsumeItemConditionWidget() {}

		SLATE_BEGIN_ARGS(ConsumeItemConditionWidget) {}

		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, UCoreCondition* InWidgetCondition, SVerticalBox::FSlot* InParentSlot, const FString& ConditionWidgetName) {
			SConditionWidgetDefault::Construct(SConditionWidgetDefault::FArguments(), InWidgetCondition, InParentSlot, ConditionWidgetName);

            UPlayerConsumeItemCondition* ConsumeItemCondition = (UPlayerConsumeItemCondition*)WidgetCondition;
            const UItemSetting* ItemSetting = GetDefault<UItemSetting>();
            auto ItemDataTable = ItemSetting->ItemTable.LoadSynchronous();
            ItemTableRefBox = SNew(SRowTableRefBox, ItemDataTable, ConsumeItemCondition->ItemId);
            ItemTableRefBox->RowSelectChanged.BindSP(this, &ConsumeItemConditionWidget::OnSelectionChanged);

			(*WidgetSlot)[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						[
							SNew(STextBlock)
							.Text(FText::FromString(TEXT("ItemId:")))
						]
						+ SHorizontalBox::Slot()
						[
                            ItemTableRefBox.ToSharedRef()
						]
					]
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						[
							SNew(STextBlock)
							.Text(FText::FromString(TEXT("Count:")))
						]
						+ SHorizontalBox::Slot()
						[
							SNew(SNumericEntryBox<int>)
							.Font(FCoreStyle::GetDefaultFontStyle("Regular", 8))
							.Value_Lambda([this] {
								UPlayerConsumeItemCondition* ConsumeItemCondition = (UPlayerConsumeItemCondition*)WidgetCondition;
								return ConsumeItemCondition->ItemCount;
							})
							.OnValueCommitted(this, &ConsumeItemConditionWidget::OnItemCountCommitted)
						]
					]
				]
			];
		}

	private:
        TSharedPtr<SRowTableRefBox> ItemTableRefBox;

        void OnSelectionChanged(int ItemId) {
            FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();

            UPlayerConsumeItemCondition* CollectItemCondition = (UPlayerConsumeItemCondition*)WidgetCondition;
            CollectItemCondition->ItemId = ItemId;
            NotifyConditionChange();
        }

		void OnItemCountCommitted(int NewValue, ETextCommit::Type InCommitType) {
			FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();

			if (NewValue > 0) {
				UPlayerConsumeItemCondition* ConsumeItemCondition = (UPlayerConsumeItemCondition*)WidgetCondition;
                ConsumeItemCondition->ItemCount = NewValue;
				NotifyConditionChange();
			}
			else {
				GameFrameworkEditorModule.GetEditorWidgetTool()->ShowNotification(FText::FromString(TEXT("itemCount必须大于0，回退操作")), 5.0);
			}
		}
	};

	/*消耗money任务条件*/
	class ConsumeMoneyConditionWidget : public SConditionWidgetDefault {
	public:
		virtual ~ConsumeMoneyConditionWidget() {}

		SLATE_BEGIN_ARGS(ConsumeMoneyConditionWidget) {}

		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, UCoreCondition* InWidgetCondition, SVerticalBox::FSlot* InParentSlot, const FString& ConditionWidgetName) {
			SConditionWidgetDefault::Construct(SConditionWidgetDefault::FArguments(), InWidgetCondition, InParentSlot, ConditionWidgetName);

            UPlayerConsumeMoneyCondition* ConsumeMoneyCondition = (UPlayerConsumeMoneyCondition*)WidgetCondition;

            const UMoneySetting* MoneySetting = GetDefault<UMoneySetting>();
            auto MoneyTypeDataTable = MoneySetting->MoneyTypeTable.LoadSynchronous();

            MoneyTypeTableRefBox = SNew(SRowTableRefBox, MoneyTypeDataTable, ConsumeMoneyCondition->MoneyType);
            MoneyTypeTableRefBox->RowSelectChanged.BindSP(this, &ConsumeMoneyConditionWidget::OnSelectionChanged);

			(*WidgetSlot)[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						[
							SNew(STextBlock)
							.Text(FText::FromString(TEXT("MoneyType:")))
						]
						+ SHorizontalBox::Slot()
						[
                            MoneyTypeTableRefBox.ToSharedRef()
						]
					]
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						[
							SNew(STextBlock)
							.Text(FText::FromString(TEXT("Count:")))
						]
						+ SHorizontalBox::Slot()
						[
							SNew(SNumericEntryBox<int>)
							.Font(FCoreStyle::GetDefaultFontStyle("Regular", 8))
							.Value_Lambda([this] {
							UPlayerConsumeMoneyCondition* ConsumeMoneyCondition = (UPlayerConsumeMoneyCondition*)WidgetCondition;
								return ConsumeMoneyCondition->MoneyCount;
							})
							.OnValueCommitted(this, &ConsumeMoneyConditionWidget::OnMoneyCountCommitted)
						]
					]
				]
			];
		}

	private:
        TSharedPtr<SRowTableRefBox> MoneyTypeTableRefBox;

        void OnSelectionChanged(int MoneyTypeId) {
            UPlayerConsumeMoneyCondition* ConsumeMoneyCondition = (UPlayerConsumeMoneyCondition*)WidgetCondition;
            ConsumeMoneyCondition->MoneyType = MoneyTypeId;
            NotifyConditionChange();
        }

		void OnMoneyCountCommitted(int NewValue, ETextCommit::Type InCommitType) {
			FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();

			if (NewValue > 0) {
				UPlayerConsumeMoneyCondition* ConsumeMoneyCondition = (UPlayerConsumeMoneyCondition*)WidgetCondition;
				ConsumeMoneyCondition->MoneyCount = NewValue;
				NotifyConditionChange();
			}
			else {
				GameFrameworkEditorModule.GetEditorWidgetTool()->ShowNotification(FText::FromString(TEXT("MoneyCount必须大于0，回退操作")), 5.0);
			}
		}
	};

	/*到达exp等级任务条件*/
	class PlayerReachExpLevelConditionWidget : public SConditionWidgetDefault {
	public:
		virtual ~PlayerReachExpLevelConditionWidget() {}

		SLATE_BEGIN_ARGS(PlayerReachExpLevelConditionWidget) {}

		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, UCoreCondition* InWidgetCondition, SVerticalBox::FSlot* InParentSlot, const FString& ConditionWidgetName) {
			SConditionWidgetDefault::Construct(SConditionWidgetDefault::FArguments(), InWidgetCondition, InParentSlot, ConditionWidgetName);

            UPlayerReachExpLevelCondition* ReachExpLevelCondition = (UPlayerReachExpLevelCondition*)WidgetCondition;
            const UExpSetting* ExpSetting = GetDefault<UExpSetting>();
            auto ExpTypeDataTable = ExpSetting->ExpTypeTable.LoadSynchronous();

            ExpTypeTableRefBox = SNew(SRowTableRefBox, ExpTypeDataTable, ReachExpLevelCondition->ExpType);
            ExpTypeTableRefBox->RowSelectChanged.BindSP(this, &PlayerReachExpLevelConditionWidget::OnSelectionChanged);

			(*WidgetSlot)[
				SNew( SHorizontalBox)
				+ SHorizontalBox::Slot()
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						[
							SNew(STextBlock)
							.Text(FText::FromString(TEXT("ExpType:")))
						]
						+ SHorizontalBox::Slot()
						[
                            ExpTypeTableRefBox.ToSharedRef()
						]
					]
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						[
							SNew(STextBlock)
							.Text(FText::FromString(TEXT("Level:")))
						]
						+ SHorizontalBox::Slot()
						[
							SNew(SNumericEntryBox<int>)
							.Font(FCoreStyle::GetDefaultFontStyle("Regular", 8))
							.Value_Lambda([this] {
								UPlayerReachExpLevelCondition* ReachExpLevelCondition = (UPlayerReachExpLevelCondition*)WidgetCondition;
								return ReachExpLevelCondition->ExpLevel;
							})
							.OnValueCommitted(this, &PlayerReachExpLevelConditionWidget::OnExpLevelCommitted)
						]
					]
				]
			];
		}

	private:
        TSharedPtr<SRowTableRefBox> ExpTypeTableRefBox;

		void OnSelectionChanged(int ExpTypeId) {
            UPlayerReachExpLevelCondition* ReachExpLevelCondition = (UPlayerReachExpLevelCondition*)WidgetCondition;
            ReachExpLevelCondition->ExpType = ExpTypeId;
            NotifyConditionChange();
		}

		void OnExpLevelCommitted(int NewValue, ETextCommit::Type InCommitType) {
			FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();

			if (NewValue > 0) {
				UPlayerReachExpLevelCondition* ReachExpLevelCondition = (UPlayerReachExpLevelCondition*)WidgetCondition;
				ReachExpLevelCondition->ExpLevel = NewValue;
				NotifyConditionChange();
			}
			else {
				GameFrameworkEditorModule.GetEditorWidgetTool()->ShowNotification(FText::FromString(TEXT("ExpLevel必须大于0，回退操作")), 5.0);
			}
		}
	};

	/*farm任务条件*/
	class FarmConditionWidget : public SConditionWidgetDefault {
	public:
		virtual ~FarmConditionWidget() {}

		SLATE_BEGIN_ARGS(FarmConditionWidget) {}

		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, UCoreCondition* InWidgetCondition, SVerticalBox::FSlot* InParentSlot, const FString& ConditionWidgetName) {
			SConditionWidgetDefault::Construct(SConditionWidgetDefault::FArguments(), InWidgetCondition, InParentSlot, ConditionWidgetName);

            const UUnitSetting* UnitSetting = GetDefault<UUnitSetting>();
            auto UnitDataTable = UnitSetting->UnitTable.LoadSynchronous();

            UPlayerFarmCondition* FarmCondition = (UPlayerFarmCondition*)WidgetCondition;
            UnitTableRefBox = SNew(SRowTableRefBox, UnitDataTable, FarmCondition->UnitId);
            UnitTableRefBox->RowSelectChanged.BindSP(this, &FarmConditionWidget::OnSelectionChanged);

			(*WidgetSlot)[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						[
							SNew(STextBlock)
							.Text(FText::FromString(TEXT("单位:")))
						]
						+ SHorizontalBox::Slot()
						[
                            UnitTableRefBox.ToSharedRef()
						]
					]
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						[
							SNew(STextBlock)
							.Text(FText::FromString(TEXT("Count:")))
						]
						+ SHorizontalBox::Slot()
						[
							SNew(SNumericEntryBox<int>)
							.Font(FCoreStyle::GetDefaultFontStyle("Regular", 8))
							.Value_Lambda([this] {
								UPlayerFarmCondition* FarmCondition = (UPlayerFarmCondition*)WidgetCondition;
								return FarmCondition->Count;
							})
							.OnValueCommitted(this, &FarmConditionWidget::OnFarmCountCommitted)
						]
					]
				]
			];
		}

	private:
        TSharedPtr<SRowTableRefBox> UnitTableRefBox;

		void OnSelectionChanged(int UnitId) {
			UPlayerFarmCondition* FarmCondition = (UPlayerFarmCondition*)WidgetCondition;
            FarmCondition->UnitId = UnitId;
			NotifyConditionChange();
		}

		void OnFarmCountCommitted(int NewValue, ETextCommit::Type InCommitType) {
			FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();

			if (NewValue > 0) {
				UPlayerFarmCondition* FarmCondition = (UPlayerFarmCondition*)WidgetCondition;
                FarmCondition->Count = NewValue;
				NotifyConditionChange();
			}
			else {
				GameFrameworkEditorModule.GetEditorWidgetTool()->ShowNotification(FText::FromString(TEXT("Count必须大于0，回退操作")), 5.0);
			}
		}
	};

	/*好感度要求任务条件*/
	class IntimacyRequestConditionWidget : public SConditionWidgetDefault {
	public:
		virtual ~IntimacyRequestConditionWidget() {}

		SLATE_BEGIN_ARGS(IntimacyRequestConditionWidget) {}

		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, UCoreCondition* InWidgetCondition, SVerticalBox::FSlot* InParentSlot, const FString& ConditionWidgetName) {
			SConditionWidgetDefault::Construct(SConditionWidgetDefault::FArguments(), InWidgetCondition, InParentSlot, ConditionWidgetName);

            const UUnitSetting* UnitSetting = GetDefault<UUnitSetting>();
            auto UnitDataTable = UnitSetting->UnitTable.LoadSynchronous();

            UPlayerIntimacyRequestCondition* IntimacyRequestCondition = (UPlayerIntimacyRequestCondition*)WidgetCondition;
            UnitTableRefBox = SNew(SRowTableRefBox, UnitDataTable, IntimacyRequestCondition->NPCId);
            UnitTableRefBox->RowSelectChanged.BindSP(this, &IntimacyRequestConditionWidget::OnSelectionChanged);

			CompareTypeSource.Add(MakeShareable(new EIntimacyCompare(EIntimacyCompare::Above)));
			CompareTypeSource.Add(MakeShareable(new EIntimacyCompare(EIntimacyCompare::Below)));

			(*WidgetSlot)[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						[
							SNew(STextBlock)
							.Text(FText::FromString(TEXT("单位:")))
						]
						+ SHorizontalBox::Slot()
						[
                            UnitTableRefBox.ToSharedRef()
						]
					]
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						[
							SNew(STextBlock)
							.Text(FText::FromString(TEXT("Count:")))
						]
						+ SHorizontalBox::Slot()
						[
							SNew(SNumericEntryBox<int>)
							.Font(FCoreStyle::GetDefaultFontStyle("Regular", 8))
							.Value_Lambda([this] {
								UPlayerIntimacyRequestCondition* IntimacyRequestCondition = (UPlayerIntimacyRequestCondition*)WidgetCondition;
								return IntimacyRequestCondition->IntimacyRequest;
							})
							.OnValueCommitted(this, &IntimacyRequestConditionWidget::OnIntimacyRequestCommitted)
						]
					]
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						[
							SNew(STextBlock)
							.Text(FText::FromString(TEXT("CompareType:")))
						]
						+ SHorizontalBox::Slot()
						[
							SAssignNew(CompareTypeComboBox, SComboBox<TSharedPtr<EIntimacyCompare>>)
							.OptionsSource(GetCompareTypeSource())
							.OnGenerateWidget(this, &IntimacyRequestConditionWidget::GenerateCompareTypeComboItem)
							.OnSelectionChanged(this, &IntimacyRequestConditionWidget::CompareTypeComboBox_OnSelectionChanged)
							[
								SNew(STextBlock)
								.Text(this, &IntimacyRequestConditionWidget::GetCompareTypeComboText)
							]
						]
					]
				]
			];
			CompareTypeComboBox->SetSelectedItem(MakeShareable(new EIntimacyCompare(IntimacyRequestCondition->Compare)));
		}

	private:		
		TSharedPtr<SComboBox<TSharedPtr<EIntimacyCompare>>> CompareTypeComboBox;
		TSharedPtr<EIntimacyCompare> SelectCompare;
		TArray<TSharedPtr<EIntimacyCompare>> CompareTypeSource;
		
        TSharedPtr<SRowTableRefBox> UnitTableRefBox;

        void OnSelectionChanged(int UnitId) {
            UPlayerIntimacyRequestCondition* IntimacyRequestCondition = (UPlayerIntimacyRequestCondition*)WidgetCondition;
            IntimacyRequestCondition->NPCId = UnitId;
            NotifyConditionChange();
        }

		TSharedRef<SWidget> GenerateCompareTypeComboItem(TSharedPtr<EIntimacyCompare> InItem) {
			FText Display;
			const UEnum* EnumPtr = FindObject<UEnum>(nullptr, TEXT("/Script/GameFramework.EIntimacyCompare"), true);
			if (!EnumPtr) {
				Display = FText::FromString(FString("Invalid"));
			}
			else {
				Display = EnumPtr->GetDisplayNameTextByValue((int64)(*InItem));
			}

			return SNew(STextBlock)
				.Text(Display);
		}

		void CompareTypeComboBox_OnSelectionChanged(TSharedPtr<EIntimacyCompare> NewGroupingMode, ESelectInfo::Type SelectInfo) {
			if (!NewGroupingMode.IsValid()) {
				return;
			}
			SelectCompare = NewGroupingMode;
			UPlayerIntimacyRequestCondition* IntimacyRequestCondition = (UPlayerIntimacyRequestCondition*)WidgetCondition;
            IntimacyRequestCondition->Compare = *SelectCompare;
			NotifyConditionChange();
		}

		void OnIntimacyRequestCommitted(int NewValue, ETextCommit::Type InCommitType) {
			FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();

			UPlayerIntimacyRequestCondition* IntimacyRequestCondition = (UPlayerIntimacyRequestCondition*)WidgetCondition;
            IntimacyRequestCondition->IntimacyRequest = NewValue;
			NotifyConditionChange();
		}

		FText GetCompareTypeComboText() const {
			FText Display = FText::FromString(FString("Invalid"));
			if (SelectCompare.IsValid()) {
				const UEnum* EnumPtr = FindObject<UEnum>(nullptr, TEXT("/Script/GameFramework.EIntimacyCompare"), true);
				if (EnumPtr) {
					Display = EnumPtr->GetDisplayNameTextByValue((int64)(*SelectCompare));
				}
			}
			return Display;
		}

		TArray<TSharedPtr<EIntimacyCompare>>* GetCompareTypeSource() {
			return &CompareTypeSource;
		}
	};

	/*NPC谈话条件*/
	class TalkToConditionWidget : public SConditionWidgetDefault {
	public:
		virtual ~TalkToConditionWidget() {}

		SLATE_BEGIN_ARGS(TalkToConditionWidget) {}

		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, UCoreCondition* InWidgetCondition, SVerticalBox::FSlot* InParentSlot, const FString& ConditionWidgetName) {
			SConditionWidgetDefault::Construct(SConditionWidgetDefault::FArguments(), InWidgetCondition, InParentSlot, ConditionWidgetName);

            const UUnitSetting* UnitSetting = GetDefault<UUnitSetting>();
            auto UnitDataTable = UnitSetting->UnitTable.LoadSynchronous();

			UTalkToCondition* TalkToCondition = (UTalkToCondition*)WidgetCondition;
            UnitTableRefBox = SNew(SRowTableRefBox, UnitDataTable, TalkToCondition->UnitID);
            UnitTableRefBox->RowSelectChanged.BindSP(this, &TalkToConditionWidget::OnSelectionChanged);

			(*WidgetSlot)[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						[
							SNew(STextBlock)
							.Text(FText::FromString(TEXT("单位:")))
						]
						+ SHorizontalBox::Slot()
						[
                            UnitTableRefBox.ToSharedRef()
						]
					]
				]
			];
		}

	private:		
        TSharedPtr<SRowTableRefBox> UnitTableRefBox;

        void OnSelectionChanged(int UnitId) {
			UTalkToCondition* TalkToCondition = (UTalkToCondition*)WidgetCondition;
			TalkToCondition->UnitID = UnitId;
            NotifyConditionChange();
        }
	};

	/*占住NPC条件*/
	class AcquireNPCsConditionWidget : public SConditionWidgetDefault {
	public:
		virtual ~AcquireNPCsConditionWidget() {}

		SLATE_BEGIN_ARGS(AcquireNPCsConditionWidget) {}

		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, UCoreCondition* InWidgetCondition, SVerticalBox::FSlot* InParentSlot, const FString& ConditionWidgetName) {
			SConditionWidgetDefault::Construct(SConditionWidgetDefault::FArguments(), InWidgetCondition, InParentSlot, ConditionWidgetName);

			(*WidgetSlot)[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					[
						SNew(SButton)
						.ForegroundColor(FSlateColor::UseForeground())
						.OnClicked(this, &AcquireNPCsConditionWidget::AddNPC)
						[
							SNew(SBorder)
							.HAlign(HAlign_Center)
							.VAlign(VAlign_Center)
							[
								SNew(STextBlock)
								.Font(FCoreStyle::GetDefaultFontStyle("Regular", 8))
								.ColorAndOpacity(FLinearColor(0, 0, 0, 1.0f))
								.Text(FText::FromString(TEXT("添加")))
							]
						]
					]
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SAssignNew(NPCsPanel, SVerticalBox)
				]
			];

			Refresh();
		}

	private:
		TSharedPtr<SVerticalBox> NPCsPanel;

		FReply AddNPC() {
			UAcquireNPCsCondition* AcquireNPCsCondition = (UAcquireNPCsCondition*)WidgetCondition;
			AcquireNPCsCondition->UnitIDs.Add(-1);

			Refresh();

			return FReply::Handled();
		}

        void OnUnitIDSelectionChanged(int UnitId, int CurrentIndex) {
			UAcquireNPCsCondition* AcquireNPCsCondition = (UAcquireNPCsCondition*)WidgetCondition;
			if (UnitId != -1) {
				TSet<int32> IDSet;
				for (int Index = 0; Index < AcquireNPCsCondition->UnitIDs.Num(); ++Index) {
					if (Index != CurrentIndex) {
						int UnitID = AcquireNPCsCondition->UnitIDs[Index];
						if (UnitID != -1) {
							IDSet.Add(UnitID);
						}
					}
				}
				if (IDSet.Contains(UnitId)) {
					AcquireNPCsCondition->UnitIDs[CurrentIndex] = -1;
					Refresh();

					FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
					GameFrameworkEditorModule.GetEditorWidgetTool()->ShowNotification(FText::FromString(TEXT("占据npcid不能重复")), 5.0);
					return;
				}
			}
			AcquireNPCsCondition->UnitIDs[CurrentIndex] = UnitId;

            NotifyConditionChange();
        }

		FReply RemoveItem(int CurrentIndex) {
			UAcquireNPCsCondition* AcquireNPCsCondition = (UAcquireNPCsCondition*)WidgetCondition;
			if (CurrentIndex < AcquireNPCsCondition->UnitIDs.Num()) {
				AcquireNPCsCondition->UnitIDs.RemoveAt(CurrentIndex);
			}

			Refresh();

			NotifyConditionChange();

			return FReply::Handled();
		}

		void Refresh() {
			const UUnitSetting* UnitSetting = GetDefault<UUnitSetting>();
			auto UnitDataTable = UnitSetting->UnitTable.LoadSynchronous();

			NPCsPanel->ClearChildren();

			UAcquireNPCsCondition* AcquireNPCsCondition = (UAcquireNPCsCondition*)WidgetCondition;
			for (int Index = 0; Index < AcquireNPCsCondition->UnitIDs.Num(); ++Index) {
				auto CurrentIndex = Index;
				auto UnitTableRefBox = SNew(SRowTableRefBox, UnitDataTable, AcquireNPCsCondition->UnitIDs[Index]);
				UnitTableRefBox->RowSelectChanged.BindSP(this, &AcquireNPCsConditionWidget::OnUnitIDSelectionChanged, CurrentIndex);

				NPCsPanel->AddSlot()
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					[
						UnitTableRefBox
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(2)
					[
						SNew(SButton)
#if ENGINE_MAJOR_VERSION > 4
						.ButtonStyle(FAppStyle::Get(), "HoverHintOnly")
#else
						.ButtonStyle(FEditorStyle::Get(), "HoverHintOnly")
#endif
						.ForegroundColor(FSlateColor::UseForeground())
						.HAlign(HAlign_Center)
						.VAlign(VAlign_Center)
						.OnClicked(this, &AcquireNPCsConditionWidget::RemoveItem, CurrentIndex)
						.Content()
						[
							SNew(STextBlock)
							.Text(FText::FromString("-"))
						]
						[
							SNew(SImage)
#if ENGINE_MAJOR_VERSION > 4
							.Image(FAppStyle::Get().GetBrush("Cross"))
#else
							.Image(FEditorStyle::Get().GetBrush("Cross"))
#endif
						]
					]
				];
			}
		}
	};

	/*扣除物品任务条件*/
	class DeductItemConditionWidget : public SConditionWidgetDefault {
	public:
		virtual ~DeductItemConditionWidget() {}

		SLATE_BEGIN_ARGS(DeductItemConditionWidget) {}

		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, UCoreCondition* InWidgetCondition, SVerticalBox::FSlot* InParentSlot, const FString& ConditionWidgetName) {
			SConditionWidgetDefault::Construct(SConditionWidgetDefault::FArguments(), InWidgetCondition, InParentSlot, ConditionWidgetName);

			UPlayerDeductItemCondition* DeductItemCondition = (UPlayerDeductItemCondition*)WidgetCondition;

			FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
			FDetailsViewArgs DetailsViewArgs;
			DetailsViewArgs.bAllowSearch = false;
			DetailsViewArgs.DefaultsOnlyVisibility = EEditDefaultsOnlyNodeVisibility::Hide;
			TSharedPtr<class IDetailsView> DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
			DetailsView->SetObject(DeductItemCondition);
			DetailsView->OnFinishedChangingProperties().AddSP(this, &DeductItemConditionWidget::OnPropertyChanged);

			(*WidgetSlot)[
				DetailsView.ToSharedRef()
			];
		}

	private:
        TSharedPtr<SRowTableRefBox> ItemTableRefBox;

        void OnPropertyChanged(const FPropertyChangedEvent& PropertyChangedEvent) {
            NotifyConditionChange();
        }
	};

	/*扣除money任务条件*/
	class DeductMoneyConditionWidget : public SConditionWidgetDefault {
	public:
		virtual ~DeductMoneyConditionWidget() {}

		SLATE_BEGIN_ARGS(DeductMoneyConditionWidget) {}

		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, UCoreCondition* InWidgetCondition, SVerticalBox::FSlot* InParentSlot, const FString& ConditionWidgetName) {
			SConditionWidgetDefault::Construct(SConditionWidgetDefault::FArguments(), InWidgetCondition, InParentSlot, ConditionWidgetName);

			UPlayerDeductMoneyCondition* DeductMoneyCondition = (UPlayerDeductMoneyCondition*)WidgetCondition;

			FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
			FDetailsViewArgs DetailsViewArgs;
			DetailsViewArgs.bAllowSearch = false;
			DetailsViewArgs.DefaultsOnlyVisibility = EEditDefaultsOnlyNodeVisibility::Hide;
			TSharedPtr<class IDetailsView> DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
			DetailsView->SetObject(DeductMoneyCondition);
			DetailsView->OnFinishedChangingProperties().AddSP(this, &DeductMoneyConditionWidget::OnPropertyChanged);

			(*WidgetSlot)[
				DetailsView.ToSharedRef()
			];
		}

	private:
        TSharedPtr<SRowTableRefBox> ItemTableRefBox;

        void OnPropertyChanged(const FPropertyChangedEvent& PropertyChangedEvent) {
            NotifyConditionChange();
        }
	};

	/*靠近NPC条件*/
	class CloseToNPCConditionWidget : public SConditionWidgetDefault {
	public:
		virtual ~CloseToNPCConditionWidget() {}

		SLATE_BEGIN_ARGS(CloseToNPCConditionWidget) {}

		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, UCoreCondition* InWidgetCondition, SVerticalBox::FSlot* InParentSlot, const FString& ConditionWidgetName) {
			SConditionWidgetDefault::Construct(SConditionWidgetDefault::FArguments(), InWidgetCondition, InParentSlot, ConditionWidgetName);

            const UUnitSetting* UnitSetting = GetDefault<UUnitSetting>();
            auto UnitDataTable = UnitSetting->UnitTable.LoadSynchronous();

			UCloseToNPCCondition* CloseToNPCCondition = (UCloseToNPCCondition*)WidgetCondition;
            UnitTableRefBox = SNew(SRowTableRefBox, UnitDataTable, CloseToNPCCondition->UnitID);
            UnitTableRefBox->RowSelectChanged.BindSP(this, &CloseToNPCConditionWidget::OnSelectionChanged);

			(*WidgetSlot)[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						[
							SNew(STextBlock)
							.Text(FText::FromString(TEXT("单位:")))
						]
						+ SHorizontalBox::Slot()
						[
                            UnitTableRefBox.ToSharedRef()
						]
					]
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						[
							SNew(STextBlock)
							.Text(FText::FromString(TEXT("距离:")))
						]
						+ SHorizontalBox::Slot()
						[
							SNew(SNumericEntryBox<float>)
							.Font(FCoreStyle::GetDefaultFontStyle("Regular", 8))
							.Value_Lambda([this] {
								UCloseToNPCCondition* CloseToNPCCondition = (UCloseToNPCCondition*)WidgetCondition;
								return CloseToNPCCondition->Radius;
							})
							.OnValueCommitted(this, &CloseToNPCConditionWidget::OnCountCommitted)
						]
					]
				]
			];
		}

	private:		
        TSharedPtr<SRowTableRefBox> UnitTableRefBox;

        void OnSelectionChanged(int UnitId) {
			UCloseToNPCCondition* CloseToNPCCondition = (UCloseToNPCCondition*)WidgetCondition;
			CloseToNPCCondition->UnitID = UnitId;
            NotifyConditionChange();
        }

		void OnCountCommitted(float NewValue, ETextCommit::Type InCommitType) {
			FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();

			UCloseToNPCCondition* CloseToNPCCondition = (UCloseToNPCCondition*)WidgetCondition;
			CloseToNPCCondition->Radius = NewValue;
			NotifyConditionChange();
		}
	};

	/*玩家技能达到等级任务条件*/
	class PlayerSkillReachLevelConditionWidget : public SConditionWidgetDefault {
	public:
		virtual ~PlayerSkillReachLevelConditionWidget() {}

		SLATE_BEGIN_ARGS(PlayerSkillReachLevelConditionWidget) {}

		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, UCoreCondition* InWidgetCondition, SVerticalBox::FSlot* InParentSlot, const FString& ConditionWidgetName) {
			SConditionWidgetDefault::Construct(SConditionWidgetDefault::FArguments(), InWidgetCondition, InParentSlot, ConditionWidgetName);

			UPlayerSkillReachLevelCondition* PlayerSkillReachLevelCondition = (UPlayerSkillReachLevelCondition*)WidgetCondition;

			FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
			FDetailsViewArgs DetailsViewArgs;
			DetailsViewArgs.bAllowSearch = false;
			DetailsViewArgs.DefaultsOnlyVisibility = EEditDefaultsOnlyNodeVisibility::Hide;
			TSharedPtr<class IDetailsView> DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
			DetailsView->SetObject(PlayerSkillReachLevelCondition);
			DetailsView->OnFinishedChangingProperties().AddSP(this, &PlayerSkillReachLevelConditionWidget::OnPropertyChanged);

			(*WidgetSlot)[
				DetailsView.ToSharedRef()
			];
		}

	private:
        TSharedPtr<SRowTableRefBox> ItemTableRefBox;

        void OnPropertyChanged(const FPropertyChangedEvent& PropertyChangedEvent) {
            NotifyConditionChange();
        }
	};
}

namespace BaseConditionFactory {
    class ConditionWidgetFactory_CollectItem : public ConditionWidgetFactory {
    public:
        virtual FString GetConditionWidgetName() override {
            return TEXT("收集物品");
        }
        virtual TSubclassOf<class UCoreCondition> GetConditionClass() override
        {
            return UPlayerCollectItemCondition::StaticClass();
        }
        virtual TSharedPtr<class SConditionWidget> CreateConditionWidget(UObject* Outer, class UCoreCondition* Condition, SVerticalBox::FSlot* ParentSlot) override {
            FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
            auto EditorWidgetTool = GameFrameworkEditorModule.GetEditorWidgetTool();
            if (Condition) {
                return SNew(ConditionUI::CollectItemConditionWidget, Condition, ParentSlot, GetConditionWidgetName());
            }
            else {
                auto ItemSource = EditorWidgetTool->GetItemInfoSource();
                if (ItemSource.Num() == 0) {
                    EditorWidgetTool->ShowNotification(FText::FromString(TEXT("没有可选的物品，请先配置物品")), 5.0f);
                    return TSharedPtr<SConditionWidget>();
                }
                else {
                    UPlayerCollectItemCondition* CollectItemCondition = NewObject<UPlayerCollectItemCondition>(Outer);
                    FItemConfigTableRow* RowData = (FItemConfigTableRow*)(ItemSource[0]->ConfigTableRow);
                    CollectItemCondition->ItemId = RowData->ItemId;
                    CollectItemCondition->ItemCount = 1;

                    return SNew(ConditionUI::CollectItemConditionWidget, CollectItemCondition, ParentSlot, GetConditionWidgetName());
                }
            }
        }
    };

    class ConditionWidgetFactory_CollectMoney : public ConditionWidgetFactory {
    public:
        virtual FString GetConditionWidgetName() override {
            return TEXT("收集金币");
        }
        virtual TSubclassOf<class UCoreCondition> GetConditionClass() override {
            return UPlayerCollectMoneyCondition::StaticClass();
        }
        virtual TSharedPtr<class SConditionWidget> CreateConditionWidget(UObject* Outer, class UCoreCondition* Condition, SVerticalBox::FSlot* ParentSlot) override {
            FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
            auto EditorWidgetTool = GameFrameworkEditorModule.GetEditorWidgetTool();
            if (Condition) {
                return SNew(ConditionUI::CollectMoneyConditionWidget, Condition, ParentSlot, GetConditionWidgetName());
            }
            else {
                auto MoneyTypeSource = EditorWidgetTool->GetMoneyTypeSource();
                if (MoneyTypeSource.Num() == 0) {
                    EditorWidgetTool->ShowNotification(FText::FromString(TEXT("没有可选的Money类型，请先配置Money类型")), 5.0f);
                    return TSharedPtr<SConditionWidget>();
                }
                else {
                    UPlayerCollectMoneyCondition* CollectMoneyCondition = NewObject<UPlayerCollectMoneyCondition>(Outer);
                    FMoneyTypeConfigTableRow* RowData = (FMoneyTypeConfigTableRow*)(MoneyTypeSource[0]->ConfigTableRow);
                    CollectMoneyCondition->MoneyType = RowData->MoneyTypeId;
                    CollectMoneyCondition->MoneyCount = 1;

                    return SNew(ConditionUI::CollectMoneyConditionWidget, CollectMoneyCondition, ParentSlot, GetConditionWidgetName());
                }
            }
        }
    };

    class ConditionWidgetFactory_PlayerConsumeItem : public ConditionWidgetFactory {
    public:
        virtual FString GetConditionWidgetName() override {
            return TEXT("玩家消耗物品");
        }
        virtual TSubclassOf<class UCoreCondition> GetConditionClass() override {
            return UPlayerConsumeItemCondition::StaticClass();
        }
        virtual TSharedPtr<class SConditionWidget> CreateConditionWidget(UObject* Outer, class UCoreCondition* Condition, SVerticalBox::FSlot* ParentSlot) override {
            FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
            auto EditorWidgetTool = GameFrameworkEditorModule.GetEditorWidgetTool();
            if (Condition) {
                return SNew(ConditionUI::ConsumeItemConditionWidget, Condition, ParentSlot, GetConditionWidgetName());
            }
            else {
                auto ItemSource = EditorWidgetTool->GetItemInfoSource();
                if (ItemSource.Num() == 0) {
                    EditorWidgetTool->ShowNotification(FText::FromString(TEXT("没有可选的物品，请先配置物品")), 5.0f);
                    return TSharedPtr<SConditionWidget>();
                }
                else {
                    UPlayerConsumeItemCondition* ConsumeItemCondition = NewObject<UPlayerConsumeItemCondition>(Outer);
                    FItemConfigTableRow* RowData = (FItemConfigTableRow*)(ItemSource[0]->ConfigTableRow);
                    ConsumeItemCondition->ItemId = RowData->ItemId;
                    ConsumeItemCondition->ItemCount = 1;

                    return SNew(ConditionUI::ConsumeItemConditionWidget, ConsumeItemCondition, ParentSlot, GetConditionWidgetName());
                }
            }
        }
    };

    class ConditionWidgetFactory_PlayerConsumeMoney : public ConditionWidgetFactory {
    public:
        virtual FString GetConditionWidgetName() override {
            return TEXT("玩家消耗货币");
        }
        virtual TSubclassOf<class UCoreCondition> GetConditionClass() override {
            return UPlayerConsumeMoneyCondition::StaticClass();
        }
        virtual TSharedPtr<class SConditionWidget> CreateConditionWidget(UObject* Outer, class UCoreCondition* Condition, SVerticalBox::FSlot* ParentSlot) override {
            FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
            auto EditorWidgetTool = GameFrameworkEditorModule.GetEditorWidgetTool();
            if (Condition) {
                return SNew(ConditionUI::ConsumeMoneyConditionWidget, Condition, ParentSlot, GetConditionWidgetName());
            }
            else {
                auto MoneyTypeSource = EditorWidgetTool->GetMoneyTypeSource();
                if (MoneyTypeSource.Num() == 0) {
                    EditorWidgetTool->ShowNotification(FText::FromString(TEXT("没有可选的Money类型，请先配置Money类型")), 5.0f);
                    return TSharedPtr<SConditionWidget>();
                }
                else {
                    UPlayerConsumeMoneyCondition* ConsumeMoneyCondition = NewObject<UPlayerConsumeMoneyCondition>(Outer);
                    FMoneyTypeConfigTableRow* RowData = (FMoneyTypeConfigTableRow*)(MoneyTypeSource[0]->ConfigTableRow);
                    ConsumeMoneyCondition->MoneyType = RowData->MoneyTypeId;
                    ConsumeMoneyCondition->MoneyCount = 1;

                    return SNew(ConditionUI::ConsumeMoneyConditionWidget, ConsumeMoneyCondition, ParentSlot, GetConditionWidgetName());
                }
            }
        }
    };

    class ConditionWidgetFactory_PlayerFarm : public ConditionWidgetFactory {
    public:
        virtual FString GetConditionWidgetName() override {
            return TEXT("杀怪");
        }
        virtual TSubclassOf<class UCoreCondition> GetConditionClass() override {
            return UPlayerFarmCondition::StaticClass();
        }
        virtual TSharedPtr<class SConditionWidget> CreateConditionWidget(UObject* Outer, class UCoreCondition* Condition, SVerticalBox::FSlot* ParentSlot) override {
            FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
            auto EditorWidgetTool = GameFrameworkEditorModule.GetEditorWidgetTool();
            if (Condition) {
                return SNew(ConditionUI::FarmConditionWidget, Condition, ParentSlot, GetConditionWidgetName());
            }
            else {
                auto UnitInfoSource = EditorWidgetTool->GetUnitInfoSource();
                if (UnitInfoSource.Num() == 0) {
                    EditorWidgetTool->ShowNotification(FText::FromString(TEXT("没有可选的单位，请先配置单位")), 5.0f);
                    return TSharedPtr<SConditionWidget>();
                }
                else {
                    UPlayerFarmCondition* FarmCondition = NewObject<UPlayerFarmCondition>(Outer);
                    FUnitInfoConfigTableRow* RowData = (FUnitInfoConfigTableRow*)(UnitInfoSource[0]->ConfigTableRow);
                    FarmCondition->UnitId = RowData->UnitId;
                    FarmCondition->Count = 1;

                    return SNew(ConditionUI::FarmConditionWidget, FarmCondition, ParentSlot, GetConditionWidgetName());
                }
            }
        }
    };

    class ConditionWidgetFactory_PlayerHunt : public ConditionWidgetFactory {
    public:
        virtual FString GetConditionWidgetName() override {
            return TEXT("Boss贡献");
        }
        virtual TSubclassOf<class UCoreCondition> GetConditionClass() override {
            return UPlayerHuntCondition::StaticClass();
        }
        virtual TSharedPtr<class SConditionWidget> CreateConditionWidget(UObject* Outer, class UCoreCondition* Condition, SVerticalBox::FSlot* ParentSlot) override {
            FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
            auto EditorWidgetTool = GameFrameworkEditorModule.GetEditorWidgetTool();
            return TSharedPtr<SConditionWidget>();
        }
    };

    class ConditionWidgetFactory_ArrivingAt : public ConditionWidgetFactory {
    public:
        virtual FString GetConditionWidgetName() override {
            return TEXT("到达地点");
        }
        virtual TSubclassOf<class UCoreCondition> GetConditionClass() override {
            return UArrivingAtCondition::StaticClass();
        }
        virtual TSharedPtr<class SConditionWidget> CreateConditionWidget(UObject* Outer, class UCoreCondition* Condition, SVerticalBox::FSlot* ParentSlot) override {
            FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
            auto EditorWidgetTool = GameFrameworkEditorModule.GetEditorWidgetTool();
            return TSharedPtr<SConditionWidget>();
        }
    };

    class ConditionWidgetFactory_PlayerReachExpLevel : public ConditionWidgetFactory {
    public:
        virtual FString GetConditionWidgetName() override {
            return TEXT("玩家达到等级");
        }
        virtual TSubclassOf<class UCoreCondition> GetConditionClass() override {
            return UPlayerReachExpLevelCondition::StaticClass();
        }
        virtual TSharedPtr<class SConditionWidget> CreateConditionWidget(UObject* Outer, class UCoreCondition* Condition, SVerticalBox::FSlot* ParentSlot) override {
            FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
            auto EditorWidgetTool = GameFrameworkEditorModule.GetEditorWidgetTool();
            if (Condition) {
                return SNew(ConditionUI::PlayerReachExpLevelConditionWidget, Condition, ParentSlot, GetConditionWidgetName());
            }
            else {
                auto ExpTypeSource = EditorWidgetTool->GetExpTypeSource();
                if (ExpTypeSource.Num() == 0) {
                    EditorWidgetTool->ShowNotification(FText::FromString(TEXT("没有可选的Exp类型，请先配置Exp类型")), 5.0f);
                    return TSharedPtr<SConditionWidget>();
                }
                else {
                    UPlayerReachExpLevelCondition* ReachExpLevelCondition = NewObject<UPlayerReachExpLevelCondition>(Outer);
                    FExpTypeConfigTableRow* RowData = (FExpTypeConfigTableRow*)(ExpTypeSource[0]->ConfigTableRow);

                    ReachExpLevelCondition->ExpType = RowData->ExpTypeId;
                    ReachExpLevelCondition->ExpLevel = 1;

                    return SNew(ConditionUI::PlayerReachExpLevelConditionWidget, ReachExpLevelCondition, ParentSlot, GetConditionWidgetName());
                }
            }
        }
    };

    class ConditionWidgetFactory_PlayerIntimacyRequest : public ConditionWidgetFactory {
    public:
        virtual FString GetConditionWidgetName() override {
            return TEXT("玩家好感度需求");
        }
        virtual TSubclassOf<class UCoreCondition> GetConditionClass() override {
            return UPlayerIntimacyRequestCondition::StaticClass();
        }
        virtual TSharedPtr<class SConditionWidget> CreateConditionWidget(UObject* Outer, class UCoreCondition* Condition, SVerticalBox::FSlot* ParentSlot) override {
            FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
            auto EditorWidgetTool = GameFrameworkEditorModule.GetEditorWidgetTool();
            if (Condition) {
                return SNew(ConditionUI::IntimacyRequestConditionWidget, Condition, ParentSlot, GetConditionWidgetName());
            }
            else {
                auto UnitInfoSource = EditorWidgetTool->GetUnitInfoSource();
                if (UnitInfoSource.Num() == 0) {
                    EditorWidgetTool->ShowNotification(FText::FromString(TEXT("没有可选的单位，请先配置单位")), 5.0f);
                    return TSharedPtr<SConditionWidget>();
                }
                else {
                    UPlayerIntimacyRequestCondition* IntimacyRequestCondition = NewObject<UPlayerIntimacyRequestCondition>(Outer);
                    FUnitInfoConfigTableRow* RowData = (FUnitInfoConfigTableRow*)(UnitInfoSource[0]->ConfigTableRow);
                    IntimacyRequestCondition->NPCId = RowData->UnitId;
                    IntimacyRequestCondition->IntimacyRequest = 0;
                    IntimacyRequestCondition->Compare = EIntimacyCompare::Above;

                    return SNew(ConditionUI::IntimacyRequestConditionWidget, IntimacyRequestCondition, ParentSlot, GetConditionWidgetName());
                }
            }
        }
    };

	class ConditionWidgetFactory_TalkTo : public ConditionWidgetFactory {
	public:
		virtual FString GetConditionWidgetName() override {
			return TEXT("NPC交谈");
		}
		virtual TSubclassOf<class UCoreCondition> GetConditionClass() override {
			return UTalkToCondition::StaticClass();
		}
		virtual TSharedPtr<class SConditionWidget> CreateConditionWidget(UObject* Outer, class UCoreCondition* Condition, SVerticalBox::FSlot* ParentSlot) override {
			FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
			auto EditorWidgetTool = GameFrameworkEditorModule.GetEditorWidgetTool();
			if (Condition) {
				return SNew(ConditionUI::TalkToConditionWidget, Condition, ParentSlot, GetConditionWidgetName());
			}
			else {
				auto UnitInfoSource = EditorWidgetTool->GetUnitInfoSource();
				if (UnitInfoSource.Num() == 0) {
					EditorWidgetTool->ShowNotification(FText::FromString(TEXT("没有可选的单位，请先配置单位")), 5.0f);
					return TSharedPtr<SConditionWidget>();
				}
				else {
					UTalkToCondition* TalkToCondition = NewObject<UTalkToCondition>(Outer);
					FUnitInfoConfigTableRow* RowData = (FUnitInfoConfigTableRow*)(UnitInfoSource[0]->ConfigTableRow);
					TalkToCondition->UnitID = RowData->UnitId;

					return SNew(ConditionUI::TalkToConditionWidget, TalkToCondition, ParentSlot, GetConditionWidgetName());
				}
			}
		}
	};

	class ConditionWidgetFactory_AcquireNPCs : public ConditionWidgetFactory {
	public:
		virtual FString GetConditionWidgetName() override {
			return TEXT("占有NPC");
		}
		virtual TSubclassOf<class UCoreCondition> GetConditionClass() override {
			return UAcquireNPCsCondition::StaticClass();
		}
		virtual TSharedPtr<class SConditionWidget> CreateConditionWidget(UObject* Outer, class UCoreCondition* Condition, SVerticalBox::FSlot* ParentSlot) override {
			FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
			auto EditorWidgetTool = GameFrameworkEditorModule.GetEditorWidgetTool();
			if (Condition) {
				return SNew(ConditionUI::AcquireNPCsConditionWidget, Condition, ParentSlot, GetConditionWidgetName());
			}
			else {
				auto UnitInfoSource = EditorWidgetTool->GetUnitInfoSource();
				if (UnitInfoSource.Num() == 0) {
					EditorWidgetTool->ShowNotification(FText::FromString(TEXT("没有可选的单位，请先配置单位")), 5.0f);
					return TSharedPtr<SConditionWidget>();
				}
				else {
					UAcquireNPCsCondition* AcquireNPCsCondition = NewObject<UAcquireNPCsCondition>(Outer);
					FUnitInfoConfigTableRow* RowData = (FUnitInfoConfigTableRow*)(UnitInfoSource[0]->ConfigTableRow);
					AcquireNPCsCondition->UnitIDs.Add(RowData->UnitId);

					return SNew(ConditionUI::AcquireNPCsConditionWidget, AcquireNPCsCondition, ParentSlot, GetConditionWidgetName());
				}
			}
		}
	};

	class ConditionWidgetFactory_PlayerDeductItem : public ConditionWidgetFactory {
	public:
		virtual FString GetConditionWidgetName() override {
			return TEXT("玩家扣除物品");
		}
		virtual TSubclassOf<class UCoreCondition> GetConditionClass() override {
			return UPlayerDeductItemCondition::StaticClass();
		}
		virtual TSharedPtr<class SConditionWidget> CreateConditionWidget(UObject* Outer, class UCoreCondition* Condition, SVerticalBox::FSlot* ParentSlot) override {
			FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
			auto EditorWidgetTool = GameFrameworkEditorModule.GetEditorWidgetTool();
			if (Condition) {
				return SNew(ConditionUI::DeductItemConditionWidget, Condition, ParentSlot, GetConditionWidgetName());
			}
			else {
				auto ItemSource = EditorWidgetTool->GetItemInfoSource();
				if (ItemSource.Num() == 0) {
					EditorWidgetTool->ShowNotification(FText::FromString(TEXT("没有可选的物品，请先配置物品")), 5.0f);
					return TSharedPtr<SConditionWidget>();
				}
				
				UPlayerDeductItemCondition* DeductItemCondition = NewObject<UPlayerDeductItemCondition>(Outer);

				return SNew(ConditionUI::DeductItemConditionWidget, DeductItemCondition, ParentSlot, GetConditionWidgetName());
			}
		}
	};

	class ConditionWidgetFactory_PlayerDeductMoney : public ConditionWidgetFactory {
	public:
		virtual FString GetConditionWidgetName() override {
			return TEXT("玩家扣除货币");
		}
		virtual TSubclassOf<class UCoreCondition> GetConditionClass() override {
			return UPlayerDeductMoneyCondition::StaticClass();
		}
		virtual TSharedPtr<class SConditionWidget> CreateConditionWidget(UObject* Outer, class UCoreCondition* Condition, SVerticalBox::FSlot* ParentSlot) override {
			FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
			auto EditorWidgetTool = GameFrameworkEditorModule.GetEditorWidgetTool();
			if (Condition) {
				return SNew(ConditionUI::DeductMoneyConditionWidget, Condition, ParentSlot, GetConditionWidgetName());
			}
			else {
				auto MoneyTypeSource = EditorWidgetTool->GetMoneyTypeSource();
				if (MoneyTypeSource.Num() == 0) {
					EditorWidgetTool->ShowNotification(FText::FromString(TEXT("没有可选的Money类型，请先配置Money类型")), 5.0f);
					return TSharedPtr<SConditionWidget>();
				}
				
				UPlayerDeductMoneyCondition* DeductMoneyCondition = NewObject<UPlayerDeductMoneyCondition>(Outer);

				return SNew(ConditionUI::DeductMoneyConditionWidget, DeductMoneyCondition, ParentSlot, GetConditionWidgetName());
			}
		}
	};

	class ConditionWidgetFactory_CloseToNPC : public ConditionWidgetFactory {
	public:
		virtual FString GetConditionWidgetName() override {
			return TEXT("靠近NPC");
		}
		virtual TSubclassOf<class UCoreCondition> GetConditionClass() override {
			return UCloseToNPCCondition::StaticClass();
		}
		virtual TSharedPtr<class SConditionWidget> CreateConditionWidget(UObject* Outer, class UCoreCondition* Condition, SVerticalBox::FSlot* ParentSlot) override {
			FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
			auto EditorWidgetTool = GameFrameworkEditorModule.GetEditorWidgetTool();
			if (Condition) {
				return SNew(ConditionUI::CloseToNPCConditionWidget, Condition, ParentSlot, GetConditionWidgetName());
			}
			else {
				auto UnitInfoSource = EditorWidgetTool->GetUnitInfoSource();
				if (UnitInfoSource.Num() == 0) {
					EditorWidgetTool->ShowNotification(FText::FromString(TEXT("没有可选的单位，请先配置单位")), 5.0f);
					return TSharedPtr<SConditionWidget>();
				}
				else {
					UCloseToNPCCondition* CloseToNPCCondition = NewObject<UCloseToNPCCondition>(Outer);
					FUnitInfoConfigTableRow* RowData = (FUnitInfoConfigTableRow*)(UnitInfoSource[0]->ConfigTableRow);
					CloseToNPCCondition->UnitID = RowData->UnitId;

					return SNew(ConditionUI::CloseToNPCConditionWidget, CloseToNPCCondition, ParentSlot, GetConditionWidgetName());
				}
			}
		}
	};

	class ConditionWidgetFactory_SkillReachLevel : public ConditionWidgetFactory {
	public:
		virtual FString GetConditionWidgetName() override {
			return TEXT("玩家技能达到等级");
		}
		virtual TSubclassOf<class UCoreCondition> GetConditionClass() override {
			return UPlayerSkillReachLevelCondition::StaticClass();
		}
		virtual TSharedPtr<class SConditionWidget> CreateConditionWidget(UObject* Outer, class UCoreCondition* Condition, SVerticalBox::FSlot* ParentSlot) override {
			FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
			auto EditorWidgetTool = GameFrameworkEditorModule.GetEditorWidgetTool();
			if (Condition) {
				return SNew(ConditionUI::PlayerSkillReachLevelConditionWidget, Condition, ParentSlot, GetConditionWidgetName());
			}
			else {
				auto SkillSource = EditorWidgetTool->GetSkillSource();
				if (SkillSource.Num() == 0) {
					EditorWidgetTool->ShowNotification(FText::FromString(TEXT("没有可选的技能，请先配置技能")), 5.0f);
					return TSharedPtr<SConditionWidget>();
				}
				else {
					UPlayerSkillReachLevelCondition* PlayerSkillReachLevelCondition = NewObject<UPlayerSkillReachLevelCondition>(Outer);
					FSkillConfigTableRow* RowData = (FSkillConfigTableRow*)(SkillSource[0]->ConfigTableRow);
					PlayerSkillReachLevelCondition->Skill.SkillID = RowData->SkillId;

					return SNew(ConditionUI::PlayerSkillReachLevelConditionWidget, PlayerSkillReachLevelCondition, ParentSlot, GetConditionWidgetName());
				}
			}
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