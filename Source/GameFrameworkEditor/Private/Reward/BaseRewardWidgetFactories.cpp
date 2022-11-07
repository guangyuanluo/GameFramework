#include "BaseRewardWidgetFactories.h"
#include "RewardWidgetFactory.h"
#include "RewardWidgetManager.h"
#include "SRewardWidget.h"

#include "Data/Reward/Reward/ExpReward.h"
#include "Data/Reward/Reward/IntimacyReward.h"
#include "Data/Reward/Reward/ItemReward.h"
#include "Data/Reward/Reward/MoneyReward.h"

#include "Modules/Item/ItemSetting.h"
#include "GameFrameworkEditor.h"
#include "GameFrameworkEditorWidgetTool.h"
#include "SRowTableRefBox.h"
#include "Modules/Money/MoneySetting.h"
#include "Modules/Exp/ExpSetting.h"
#include "Modules/Unit/UnitSetting.h"

namespace RewardUI {
	class SRewardWidgetDefault : public SRewardWidget {
	public:
		SLATE_BEGIN_ARGS(SRewardWidgetDefault) {}

		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, UCoreReward* InWidgetReward, SVerticalBox::FSlot& InParentSlot) {
			SRewardWidget::Construct(SRewardWidget::FArguments(), InWidgetReward);
			ParentSlot = &InParentSlot;

			TSharedRef<SHorizontalBox> Panel = SNew(SHorizontalBox);

			ChildSlot
			[
				Panel
			];

			WidgetSlot = &(Panel->AddSlot());

			Panel->AddSlot()
				[
					SNew(SButton)
					.OnClicked(this, &SRewardWidgetDefault::RemoveClick)
					[
						SNew(SBorder)
						.HAlign(HAlign_Center)
						.VAlign(VAlign_Center)
						[
							SNew(STextBlock)
							.Font(FCoreStyle::GetDefaultFontStyle("Regular", 8))
							.ColorAndOpacity(FLinearColor(.8f, .8f, .8f, 1.0f))
							.Text(FText::FromString(TEXT("移除")))
						]
					]
				];
		}

		FReply RemoveClick() {
			SRewardWidget::OnRewardWidgetPreremove.ExecuteIfBound(WidgetReward);

			ParentSlot->DetachWidget();

			return FReply::Handled();
		}

	protected:
		void NotifyRewardChange() {
			SRewardWidget::OnRewardWidgetChange.ExecuteIfBound(WidgetReward);
		}

	private:
		SVerticalBox::FSlot* ParentSlot;
	protected:
		SHorizontalBox::FSlot* WidgetSlot;
	};

	/*物品奖励*/
	class ItemRewardWidget : public SRewardWidgetDefault {
	public:
		virtual ~ItemRewardWidget() {}

		SLATE_BEGIN_ARGS(ItemRewardWidget) {}

		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, UCoreReward* InWidgetReward, SVerticalBox::FSlot& InParentSlot) {
			SRewardWidgetDefault::Construct(SRewardWidgetDefault::FArguments(), InWidgetReward, InParentSlot);

            UItemReward* ItemReward = (UItemReward*)InWidgetReward;
            const UItemSetting* ItemSetting = GetDefault<UItemSetting>();
            auto ItemDataTable = ItemSetting->ItemTable.LoadSynchronous();
            ItemTableRefBox = SNew(SRowTableRefBox, ItemDataTable, ItemReward->ItemId);
            ItemTableRefBox->RowSelectChanged.BindSP(this, &ItemRewardWidget::OnSelectionChanged);

			(*WidgetSlot)
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
                    .AutoHeight()
					[
						SNew(STextBlock)
						.Text(FText::FromString(TEXT("奖励物品")))
					]
					+ SVerticalBox::Slot()
                    .AutoHeight()
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
										UItemReward* ItemReward = (UItemReward*)WidgetReward;
										return ItemReward->Count;
									})
									.OnValueCommitted(this, &ItemRewardWidget::OnItemCountCommitted)
								]
							]
						]
					]
				];
		}

	private:
        TSharedPtr<SRowTableRefBox> ItemTableRefBox;

        void OnSelectionChanged(int ItemId) {
			FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();

			UItemReward* ItemReward = (UItemReward*)WidgetReward;
			ItemReward->ItemId = ItemId;
			NotifyRewardChange();
		}

		void OnItemCountCommitted(int NewValue, ETextCommit::Type InCommitType) {
			FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();

			if (NewValue > 0) {
				UItemReward* ItemReward = (UItemReward*)WidgetReward;
				ItemReward->Count = NewValue;
				NotifyRewardChange();
			}
			else {
				GameFrameworkEditorModule.GetEditorWidgetTool()->ShowNotification(FText::FromString(TEXT("itemCount必须大于0，回退操作")), 5.0);
			}
		}
	};

	/*货币奖励*/
	class MoneyRewardWidget : public SRewardWidgetDefault {
	public:
		virtual ~MoneyRewardWidget() {}

		SLATE_BEGIN_ARGS(MoneyRewardWidget) {}

		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, UCoreReward* InWidgetReward, SVerticalBox::FSlot& InParentSlot) {
			SRewardWidgetDefault::Construct(SRewardWidgetDefault::FArguments(), InWidgetReward, InParentSlot);
			
            UMoneyReward* MoneyReward = (UMoneyReward*)InWidgetReward;

            const UMoneySetting* MoneySetting = GetDefault<UMoneySetting>();
            auto MoneyTypeDataTable = MoneySetting->MoneyTypeTable.LoadSynchronous();

            MoneyTypeTableRefBox = SNew(SRowTableRefBox, MoneyTypeDataTable, MoneyReward->MoneyType);
            MoneyTypeTableRefBox->RowSelectChanged.BindSP(this, &MoneyRewardWidget::OnSelectionChanged);

			(*WidgetSlot)
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
                    .AutoHeight()
					[
						SNew(STextBlock)
						.Text(FText::FromString(TEXT("货币奖励")))
					]
					+ SVerticalBox::Slot()
                    .AutoHeight()
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
									.Text(FText::FromString(TEXT("货币类型:")))
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
										UMoneyReward* MoneyReward = (UMoneyReward*)WidgetReward;
										return MoneyReward->MoneyCount;
									})
									.OnValueCommitted(this, &MoneyRewardWidget::OnMoneyCountCommitted)
								]
							]
						]
					]
				];
		}

	private:
		TSharedPtr<SRowTableRefBox> MoneyTypeTableRefBox;

		void OnSelectionChanged(int MoneyTypeId) {
			UMoneyReward* MoneyReward = (UMoneyReward*)WidgetReward;
			MoneyReward->MoneyType = MoneyTypeId;
            NotifyRewardChange();
		}

		void OnMoneyCountCommitted(int NewValue, ETextCommit::Type InCommitType) {
			FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();

			if (NewValue > 0) {
				UMoneyReward* MoneyReward = (UMoneyReward*)WidgetReward;
				MoneyReward->MoneyCount = NewValue;
				NotifyRewardChange();
			}
			else {
				GameFrameworkEditorModule.GetEditorWidgetTool()->ShowNotification(FText::FromString(TEXT("MoneyCount必须大于0，回退操作")), 5.0);
			}
		}
	};

	/*经验奖励*/
	class ExpRewardWidget : public SRewardWidgetDefault {
	public:
		virtual ~ExpRewardWidget() {}

		SLATE_BEGIN_ARGS(ExpRewardWidget) {}

		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, UCoreReward* InWidgetReward, SVerticalBox::FSlot& InParentSlot) {
			SRewardWidgetDefault::Construct(SRewardWidgetDefault::FArguments(), InWidgetReward, InParentSlot);

            UExpReward* ExpReward = (UExpReward*)InWidgetReward;
            const UExpSetting* ExpSetting = GetDefault<UExpSetting>();
            auto ExpTypeDataTable = ExpSetting->ExpTypeTable.LoadSynchronous();

            ExpTypeTableRefBox = SNew(SRowTableRefBox, ExpTypeDataTable, ExpReward->ExpType);
            ExpTypeTableRefBox->RowSelectChanged.BindSP(this, &ExpRewardWidget::OnSelectionChanged);

			(*WidgetSlot)[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
                .AutoHeight()
				[
					SNew(STextBlock)
					.Text(FText::FromString(TEXT("经验奖励")))
				]
				+ SVerticalBox::Slot()
                .AutoHeight()
				[
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
								.Text(FText::FromString(TEXT("经验类型:")))
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
								.Text(FText::FromString(TEXT("数值:")))
							]
							+ SHorizontalBox::Slot()
							[
								SNew(SNumericEntryBox<int>)
								.Font(FCoreStyle::GetDefaultFontStyle("Regular", 8))
								.Value_Lambda([this] {
									UExpReward* ExpReward = (UExpReward*)WidgetReward;
									return ExpReward->Count;
								})
								.OnValueCommitted(this, &ExpRewardWidget::OnExpCountCommitted)
							]
						]
					]
				]
			];
		}

	private:
        TSharedPtr<SRowTableRefBox> ExpTypeTableRefBox;

		void OnSelectionChanged(int ExpTypeId) {
			UExpReward* ExpReward = (UExpReward*)WidgetReward;
			ExpReward->ExpType = ExpTypeId;
            NotifyRewardChange();
		}

		void OnExpCountCommitted(int NewValue, ETextCommit::Type InCommitType) {
			FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();

			if (NewValue > 0) {
				UExpReward* ExpReward = (UExpReward*)WidgetReward;
				ExpReward->Count = NewValue;
				NotifyRewardChange();
			}
			else {
				GameFrameworkEditorModule.GetEditorWidgetTool()->ShowNotification(FText::FromString(TEXT("数值必须大于0，回退操作")), 5.0);
			}
		}
	};

	/*好感度奖励*/
	class IntimacyRewardWidget : public SRewardWidgetDefault {
	public:
		virtual ~IntimacyRewardWidget() {}

		SLATE_BEGIN_ARGS(IntimacyRewardWidget) {}

		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, UCoreReward* InWidgetReward, SVerticalBox::FSlot& InParentSlot) {
			SRewardWidgetDefault::Construct(SRewardWidgetDefault::FArguments(), InWidgetReward, InParentSlot);

            const UUnitSetting* UnitSetting = GetDefault<UUnitSetting>();
            auto UnitDataTable = UnitSetting->UnitTable.LoadSynchronous();

            UIntimacyReward* IntimacyReward = (UIntimacyReward*)InWidgetReward;
            UnitTableRefBox = SNew(SRowTableRefBox, UnitDataTable, IntimacyReward->UnitId);
            UnitTableRefBox->RowSelectChanged.BindSP(this, &IntimacyRewardWidget::OnSelectionChanged);

			(*WidgetSlot)[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
                .AutoHeight()
				[
					SNew(STextBlock)
					.Text(FText::FromString(TEXT("好感度奖励")))
				]
				+ SVerticalBox::Slot()
                .AutoHeight()
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
									UIntimacyReward* IntimacyReward = (UIntimacyReward*)WidgetReward;
									return IntimacyReward->IntimacyValue;
								})
								.OnValueCommitted(this, &IntimacyRewardWidget::OnIntimacyRequestCommitted)
							]
						]
					]
				]
			];
		}

	private:		
        TSharedPtr<SRowTableRefBox> UnitTableRefBox;

        void OnSelectionChanged(int UnitId) {
			UIntimacyReward* IntimacyReward = (UIntimacyReward*)WidgetReward;
			IntimacyReward->UnitId = UnitId;
            NotifyRewardChange();
        }

		void OnIntimacyRequestCommitted(int NewValue, ETextCommit::Type InCommitType) {
			FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();

			UIntimacyReward* IntimacyReward = (UIntimacyReward*)WidgetReward;
			IntimacyReward->IntimacyValue = NewValue;
			NotifyRewardChange();
		}
	};
}

namespace BaseRewardFactory {
	class RewardWidgetFactory_Item : public RewardWidgetFactory {
	public:
		virtual FString GetRewardWidgetName() override {
			return TEXT("物品奖励");
		}
		virtual TSubclassOf<class UCoreReward> GetRewardClass() override {
			return UItemReward::StaticClass();
		}
		virtual TSharedPtr<class SRewardWidget> CreateRewardWidget(UObject* InOuter, class UCoreReward* Reward, SVerticalBox::FSlot& ParentSlot) override {
			FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
			auto EditorWidgetTool = GameFrameworkEditorModule.GetEditorWidgetTool();
			if (Reward) {
				return SNew(RewardUI::ItemRewardWidget, Reward, ParentSlot);
			}
			else {
				auto ItemSource = EditorWidgetTool->GetItemInfoSource();
				if (ItemSource.Num() == 0) {
					EditorWidgetTool->ShowNotification(FText::FromString(TEXT("没有可选的物品，请先配置物品")), 5.0f);
					return TSharedPtr<SRewardWidget>();
				}
				else {
					UItemReward* ItemReward = NewObject<UItemReward>(InOuter);
					FItemConfigTableRow* RowData = (FItemConfigTableRow*)(ItemSource[0]->ConfigTableRow);
					ItemReward->ItemId = RowData->ItemId;
					ItemReward->Count = 1;

					return SNew(RewardUI::ItemRewardWidget, ItemReward, ParentSlot);
				}
			}
		}
	};

	class RewardWidgetFactory_Money : public RewardWidgetFactory {
	public:
		virtual FString GetRewardWidgetName() override {
			return TEXT("货币奖励");
		}
		virtual TSubclassOf<class UCoreReward> GetRewardClass() override {
			return UMoneyReward::StaticClass();
		}
		virtual TSharedPtr<class SRewardWidget> CreateRewardWidget(UObject* InOuter, class UCoreReward* Reward, SVerticalBox::FSlot& ParentSlot) override {
			FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
			auto EditorWidgetTool = GameFrameworkEditorModule.GetEditorWidgetTool();
			if (Reward) {
				return SNew(RewardUI::MoneyRewardWidget, Reward, ParentSlot);
			}
			else {
				auto MoneyTypeSource = EditorWidgetTool->GetMoneyTypeSource();
				if (MoneyTypeSource.Num() == 0) {
					EditorWidgetTool->ShowNotification(FText::FromString(TEXT("没有可选的Money类型，请先配置Money类型")), 5.0f);
					return TSharedPtr<SRewardWidget>();
				}
				else {
					UMoneyReward* MoneyReward = NewObject<UMoneyReward>(InOuter);
					FMoneyTypeConfigTableRow* RowData = (FMoneyTypeConfigTableRow*)(MoneyTypeSource[0]->ConfigTableRow);
					MoneyReward->MoneyType = RowData->MoneyTypeId;
					MoneyReward->MoneyCount = 1;

					return SNew(RewardUI::MoneyRewardWidget, MoneyReward, ParentSlot);
				}
			}
		}
	};

	class RewardWidgetFactory_Exp : public RewardWidgetFactory {
	public:
		virtual FString GetRewardWidgetName() override {
			return TEXT("经验奖励");
		}
		virtual TSubclassOf<class UCoreReward> GetRewardClass() override {
			return UExpReward::StaticClass();
		}
		virtual TSharedPtr<class SRewardWidget> CreateRewardWidget(UObject* InOuter, class UCoreReward* Reward, SVerticalBox::FSlot& ParentSlot) override {
			FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
			auto EditorWidgetTool = GameFrameworkEditorModule.GetEditorWidgetTool();
			if (Reward) {
				return SNew(RewardUI::ExpRewardWidget, Reward, ParentSlot);
			}
			else {
				auto ExpTypeSource = EditorWidgetTool->GetExpTypeSource();
				if (ExpTypeSource.Num() == 0) {
					EditorWidgetTool->ShowNotification(FText::FromString(TEXT("没有可选的Exp类型，请先配置Exp类型")), 5.0f);
					return TSharedPtr<SRewardWidget>();
				}
				else {
					UExpReward* ExpReward = NewObject<UExpReward>(InOuter);
					FExpTypeConfigTableRow* RowData = (FExpTypeConfigTableRow*)(ExpTypeSource[0]->ConfigTableRow);

					ExpReward->ExpType = RowData->ExpTypeId;
					ExpReward->Count = 1;

					return SNew(RewardUI::ExpRewardWidget, ExpReward, ParentSlot);
				}
			}
		}
	};

	class RewardWidgetFactory_Intimacy : public RewardWidgetFactory {
	public:
		virtual FString GetRewardWidgetName() override {
			return TEXT("好感奖励");
		}
		virtual TSubclassOf<class UCoreReward> GetRewardClass() override {
			return UIntimacyReward::StaticClass();
		}
		virtual TSharedPtr<class SRewardWidget> CreateRewardWidget(UObject* InOuter, class UCoreReward* Reward, SVerticalBox::FSlot& ParentSlot) override {
			FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
			auto EditorWidgetTool = GameFrameworkEditorModule.GetEditorWidgetTool();
			if (Reward) {
				return SNew(RewardUI::IntimacyRewardWidget, Reward, ParentSlot);
			}
			else {
				auto UnitInfoSource = EditorWidgetTool->GetUnitInfoSource();
				if (UnitInfoSource.Num() == 0) {
					EditorWidgetTool->ShowNotification(FText::FromString(TEXT("没有可选的单位，请先配置单位")), 5.0f);
					return TSharedPtr<SRewardWidget>();
				}
				else {
					UIntimacyReward* IntimacyReward = NewObject<UIntimacyReward>(InOuter);
					FUnitInfoConfigTableRow* RowData = (FUnitInfoConfigTableRow*)(UnitInfoSource[0]->ConfigTableRow);
					IntimacyReward->UnitId = RowData->UnitId;
					IntimacyReward->IntimacyValue = 0;

					return SNew(RewardUI::IntimacyRewardWidget, IntimacyReward, ParentSlot);
				}
			}
		}
	};
}

TArray<TSharedPtr<RewardWidgetFactory>> BaseRewardWidgetFactories::BaseFactories = {
    TSharedPtr<RewardWidgetFactory>(new BaseRewardFactory::RewardWidgetFactory_Item),
    TSharedPtr<RewardWidgetFactory>(new BaseRewardFactory::RewardWidgetFactory_Money),
    TSharedPtr<RewardWidgetFactory>(new BaseRewardFactory::RewardWidgetFactory_Exp),
    TSharedPtr<RewardWidgetFactory>(new BaseRewardFactory::RewardWidgetFactory_Intimacy),
};

void BaseRewardWidgetFactories::Init()
{
    for (int Index = 0; Index < BaseFactories.Num(); ++Index)
    {
        RewardWidgetManager::RegistRewardWidgetFactory(BaseFactories[Index]);
    }
}
void BaseRewardWidgetFactories::Uninit()
{
    for (int Index = 0; Index < BaseFactories.Num(); ++Index) {
        RewardWidgetManager::UnregistRewardWidgetFactory(BaseFactories[Index]);
    }
}