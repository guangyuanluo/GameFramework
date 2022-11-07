#include "GenerateEnumFactory.h"
#include "Editor/Enum/GenerateEnum.h"
#include "Kismet2/EnumEditorUtils.h"
#include "Misc/MessageDialog.h"
#include "Misc/FeedbackContext.h"
#include "Internationalization/StringTable.h"

#define LOCTEXT_NAMESPACE "EditorFactories"

UGenerateEnumFactory::UGenerateEnumFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = UGenerateEnum::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

bool UGenerateEnumFactory::ShouldShowInNewMenu() const {
    return false;
}

UObject* UGenerateEnumFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	ensure(UGenerateEnum::StaticClass() == Class);

	if(!FEnumEditorUtils::IsNameAvailebleForUserDefinedEnum(Name))
	{
		const FText Message = FText::Format( LOCTEXT("EnumWithNameAlreadyExists", "Enum '{0}' already exists. The name must be unique."), FText::FromName( Name ) );
		if(Warn)
		{
			Warn->Log( Message );
		}
		FMessageDialog::Open( EAppMsgType::Ok, Message);
		return nullptr;
	}

	UEnum* Enum = NewObject<UGenerateEnum>(InParent, Name, Flags);

	if (NULL != Enum)
	{
		TArray<TPair<FName, int64>> EmptyNames;
		Enum->SetEnums(EmptyNames, UEnum::ECppForm::Namespaced);
		Enum->SetMetaData(TEXT("BlueprintType"), TEXT("true"));
	}

	return Enum;
}

#undef LOCTEXT_NAMESPACE