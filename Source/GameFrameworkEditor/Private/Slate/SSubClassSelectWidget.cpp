#include "SSubClassSelectWidget.h"
#include "Graph/GameFrameworkGraphTypes.h"
#include "Widgets/Input/SComboButton.h"
#include "Widgets/Input/SSearchBox.h"

void SSubClassSelectWidget::Construct(const FArguments& InArgs)
{
	MetaClass = InArgs._MetaClass;
	bAllowAbstract = InArgs._AllowAbstract;
	SelectedClass = InArgs._SelectedClass;
	OnSetClass = InArgs._OnSetClass;

	TSharedPtr<struct FGraphNodeClassHelper> ClassCache = MakeShareable(new FGraphNodeClassHelper(MetaClass));
	ClassCache->UpdateAvailableBlueprintClasses();

	FCategorizedGraphActionListBuilder ClassBuilder(TEXT("ClassBuilder"));

	TArray<FGameFrameworkGraphNodeClassData> NodeClasses;
	ClassCache->GatherClasses(MetaClass, NodeClasses);

	TSet<FString> CategoryNames;

	for (auto NodeClass : NodeClasses) {
		auto ChildClass = NodeClass.GetClass();
		if (!bAllowAbstract && ChildClass->HasAnyClassFlags(CLASS_Abstract)) continue;
		UObject* ChildDefaultObject = ChildClass->GetDefaultObject();
		auto ClassDisplayName = ChildClass->GetDisplayNameText().ToString();
		if (ClassDisplayName.IsEmpty()) continue;
		ClassMap.Add(ClassDisplayName, ChildClass);
		FString Category = ChildClass->GetMetaDataText(TEXT("Category"), TEXT("UObjectCategory"), ChildClass->GetFullGroupName(false)).ToString();
		if (!Category.IsEmpty()) {
			auto& ClassArray = CategoryClassMap.FindOrAdd(Category);
			ClassArray.Add(ClassDisplayName);
			if (!CategoryNames.Contains(Category)) {
				CategoryNames.Add(Category);
				ClassTreeItemSource.Add(MakeShareable(new FString(Category)));
			}
		}
		else {
			ClassTreeItemSource.Add(MakeShareable(new FString(ClassDisplayName)));
		}
	}

	SAssignNew(ComboButton, SComboButton)
		.OnGetMenuContent(this, &SSubClassSelectWidget::GenerateClassPicker)
		.ToolTipText(this, &SSubClassSelectWidget::GetDisplayValueAsString)
		.ButtonContent()
		[
			SNew(STextBlock)
			.Text(this, &SSubClassSelectWidget::GetDisplayValueAsString)
		];

	ChildSlot
	[
		ComboButton.ToSharedRef()
	];
}

TSharedRef<SWidget> SSubClassSelectWidget::GenerateClassPicker() {
	FilterText.Empty();
	return SNew(SBox)
		.WidthOverride(280)
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				SAssignNew(SearchBox, SSearchBox)
				.OnTextChanged( this, &SSubClassSelectWidget::OnFilterTextChanged )
			]
			+SVerticalBox::Slot()
			.AutoHeight()
			.MaxHeight(500)
			[
				SAssignNew(TreeView, STreeView<TSharedPtr<FString>>)
				.TreeItemsSource(&ClassTreeItemSource)
				.SelectionMode(ESelectionMode::Single)
				.OnGenerateRow(this, &SSubClassSelectWidget::MakeTableRowWidget)
				.OnGetChildren(this, &SSubClassSelectWidget::HandleGetChildrenForTree)
				.OnSelectionChanged(this, &SSubClassSelectWidget::OnSelectionChanged)
			]			
		];
}

FText SSubClassSelectWidget::GetDisplayValueAsString() const {
	if (SelectedClass.Get()) {
		return SelectedClass.Get()->GetDisplayNameText();
	}
	return FText::FromString(TEXT("None"));
}

TSharedRef<ITableRow> SSubClassSelectWidget::MakeTableRowWidget(TSharedPtr<FString> Item, const TSharedRef<STableViewBase>& OwnerTable) {
	return SNew(STableRow<TSharedPtr<FString>>, OwnerTable)
		.Content()
		[
			SNew(STextBlock)
			.Text(FText::FromString(*Item))
		];
}

void SSubClassSelectWidget::HandleGetChildrenForTree(TSharedPtr<FString> InItem, TArray<TSharedPtr<FString>>& OutChildren) {
	TArray<FString>* FindPtr = CategoryClassMap.Find(*InItem);
	if (FindPtr) {
		for (const auto& Child : *FindPtr) {
			if (FilterText.IsEmpty() || Child.Find(FilterText) >= 0) {
				OutChildren.Add(MakeShareable(new FString(Child)));
			}
		}
	}
}

void SSubClassSelectWidget::OnSelectionChanged(TSharedPtr<FString> Selection, ESelectInfo::Type SelectInfo) {
	auto FindPtr = CategoryClassMap.Find(*Selection);
	if (FindPtr) {
		//能找到说明选中的是category
		SelectClassName.Reset();
		OnSetClass.Execute(NULL);
	}
	else {
		SelectClassName = Selection;
		auto FindClassPtr = ClassMap.Find(*SelectClassName);
		if (FindClassPtr) {
			OnSetClass.Execute(*FindClassPtr);
		}
	}
	ComboButton->SetIsOpen(false);
}

void SSubClassSelectWidget::OnFilterTextChanged(const FText& InFilterText) {
	FilterText = InFilterText.ToString();
	TreeView->RequestTreeRefresh();
}