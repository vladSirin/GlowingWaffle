// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffWorldUserWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"

void UWaffWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Make sure the attached actor is still there, if nullptr will crash
	if(!IsValid(AttachedActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("The AttachedActor no longer valid, removing widget"));
		this->RemoveFromParent();
		return;
	}

	// Project the UI from actor location to screen location, considering dpi scale, utilizing the parent size box
	if(FVector2d ScreenPosition; UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation() + WorldOffset,
	                                                                    ScreenPosition, false))
	{
		float Scale = UWidgetLayoutLibrary::GetViewportScale(this);

		ScreenPosition /= Scale;

		ParentSizeBox->SetRenderTranslation(ScreenPosition);
	}
}
