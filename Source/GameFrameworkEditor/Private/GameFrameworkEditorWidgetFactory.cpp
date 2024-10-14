// Fill out your copyright notice in the Description page of Project Settings.
#include "GameFrameworkEditorWidgetFactory.h"

bool GameFrameworkEditorWidgetFactory::CheckOpenCondition() {
    return true;
}

void GameFrameworkEditorWidgetFactory::Open(TSharedPtr<FUICommandList> InCommandList) {
    const FVector2D BrowserWindowSize(1280, 720);
    TSharedPtr<SWindow> RootWindow = FGlobalTabmanager::Get()->GetRootWindow();
    FSlateApplication::Get().AddModalWindow(
        SNew(SWindow)
        .Title(GetWindowName())
        .HasCloseButton(true)
        .ClientSize(BrowserWindowSize)
        [
            ConstructPage(InCommandList)
        ], RootWindow);
}