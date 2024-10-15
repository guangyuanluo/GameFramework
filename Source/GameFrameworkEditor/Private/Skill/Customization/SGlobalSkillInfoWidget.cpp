#include "SGlobalSkillInfoWidget.h"
#include "Modules/Skill/CoreAbility.h"
#include "Graph/GameFrameworkGraphTypes.h"

namespace SkillUI
{
	const FName SkillPath(TEXT("技能路径"));
	const FName SkillSortPriority(TEXT("技能优先级"));
};

class SSkillInfoRow : public SMultiColumnTableRow<TSharedPtr<TSubclassOf<class UCoreAbility>>>
{
public:
	SLATE_BEGIN_ARGS(SSkillInfoRow) {}

	SLATE_END_ARGS()

	void Construct(const FArguments& Args, const TSharedRef<STableViewBase>& OwnerTableView, TSharedPtr<TSubclassOf<class UCoreAbility>> InPtr)
	{
		SkillPtr = InPtr;

		SMultiColumnTableRow<TSharedPtr<TSubclassOf<class UCoreAbility>>>::Construct(
			FSuperRowType::FArguments()
			.Padding(1.0f),
			OwnerTableView
		);
	}

	/** Overridden from SMultiColumnTableRow.  Generates a widget for this column of the list view. */
	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& ColumnName) override
	{
		auto AbilityCDO = Cast<UCoreAbility>(SkillPtr->GetDefaultObject());
		if (ColumnName == SkillUI::SkillPath)
		{
			return	SNew(STextBlock)
				.Text(FText::FromString(SkillPtr->Get()->GetClassPathName().ToString()));
		}
		else if (ColumnName == SkillUI::SkillSortPriority) {
			return SNew(STextBlock).Text(FText::FromString(FString::Format(TEXT("{0}"), { AbilityCDO->SortPriority })));
		}

		return SNullWidget::NullWidget;
	}

private:

	TSharedPtr<TSubclassOf<class UCoreAbility>> SkillPtr;
};

void SGlobalSkillInfoWidget::Construct(const FArguments& InArgs, TSubclassOf<class UCoreAbility> CurrentAbility) {
	auto FilterClass = UCoreAbility::StaticClass();

	TSharedPtr<struct FGameGraphNodeClassHelper> ClassCache = MakeShareable(new FGameGraphNodeClassHelper(FilterClass));
	ClassCache->UpdateAvailableBlueprintClasses();

	FCategorizedGraphActionListBuilder AbilityBuilder(TEXT("Ability"));

	TArray<FGameFrameworkGraphNodeClassData> AbilityClasses;
	ClassCache->GatherClasses(FilterClass, AbilityClasses);

	TArray<TSubclassOf<UCoreAbility>*> UnsortAbilities;
	for (auto& AbilityClass : AbilityClasses) {
		if (AbilityClass.GetClass()->HasAnyClassFlags(CLASS_Abstract)) continue;
		UnsortAbilities.Add(new TSubclassOf<UCoreAbility>(AbilityClass.GetClass()));
	}
	UnsortAbilities.Sort([](TSubclassOf<class UCoreAbility>& A, TSubclassOf<class UCoreAbility>& B){
		auto ACDO = Cast<UCoreAbility>(A->GetDefaultObject());
		auto BCDO = Cast<UCoreAbility>(B->GetDefaultObject());
		return ACDO->SortPriority > BCDO->SortPriority;
	});
	for (const auto& UnsortAbility : UnsortAbilities) {
		AbilityData.Add(MakeShareable(UnsortAbility));
	}

	ChildSlot
		[
			SAssignNew(ListView, SListView<TSharedPtr<TSubclassOf<class UCoreAbility>>>)
				.ListItemsSource(&AbilityData)
				.SelectionMode(ESelectionMode::Single)
				.OnGenerateRow(this, &SGlobalSkillInfoWidget::SkillInfoListViewOnGenerateRow)
				.HeaderRow
				(
					SNew(SHeaderRow)
					+ SHeaderRow::Column(SkillUI::SkillPath)
					.DefaultLabel(FText::FromName(SkillUI::SkillPath))
					.FillWidth(100.0f)
					+ SHeaderRow::Column(SkillUI::SkillSortPriority)
					.DefaultLabel(FText::FromName(SkillUI::SkillSortPriority))
					.FillWidth(10.0f)
				)
		];

	for (const auto& AbilityClass : AbilityData) {
		if (AbilityClass->Get() == CurrentAbility) {
			ListView->SetSelection(AbilityClass);
			ListView->RequestScrollIntoView(AbilityClass);
			break;
		}
	}
}

TSharedRef<ITableRow> SGlobalSkillInfoWidget::SkillInfoListViewOnGenerateRow(TSharedPtr<TSubclassOf<class UCoreAbility>> Item, const TSharedRef<STableViewBase>& OwnerTable) {
	return SNew(SSkillInfoRow, OwnerTable, Item);
}