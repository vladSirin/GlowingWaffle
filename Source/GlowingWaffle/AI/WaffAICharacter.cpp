// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "WaffAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AWaffAICharacter::AWaffAICharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AttriComp = CreateDefaultSubobject<UWaffAttributeComponent>(TEXT("AttriComp"));
	ActionComponent = CreateDefaultSubobject<UWaffActionComponent>(TEXT("ActionComp"));
	TimeToHit = "TimeToHit";

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
}

// Post init
void AWaffAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttriComp->OnHealthChanged.AddDynamic(this, &AWaffAICharacter::OnHealthChanged);
	AWaffAIController* AIC = Cast<AWaffAIController>(GetController());
	if(AIC)
	{
		AIC->OnTargetFirstSight.AddDynamic(this, &AWaffAICharacter::OnTargetFirstSight);
	}
}

void AWaffAICharacter::OnTargetFirstSight(AActor* TargetActor)
{
	// UI for alert
	if(AlertMark == nullptr)
	{
		AlertMark = CreateWidget<UWaffWorldUserWidget>(GetWorld(), AlertMarkWidgetClass);
		if(AlertMark)
		{
			AlertMark->AttachedActor = this;
			AlertMark->AddToViewport();
		}
	}
}

// Called when the game starts or when spawned
void AWaffAICharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AWaffAICharacter::OnHealthChanged(AActor* ChangeInstigator, UWaffAttributeComponent* OwingComp, float NewHealth,
                                       float Delta)
{
	// UI
	if (ActiveHealthBar == nullptr)
	{
		ActiveHealthBar = CreateWidget<UWaffWorldUserWidget>(GetWorld(), HealthWidgetClass);
		if (ActiveHealthBar)
		{
			ActiveHealthBar->AttachedActor = this;
			ActiveHealthBar->AddToViewport();
		}
	}

	if (Delta < 0.0)
	{
		AWaffAIController* AIC = Cast<AWaffAIController>(GetController());
		if(AIC == nullptr)
		{
			return;
		}
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHit, GetWorld()->TimeSeconds);
		
		//@Todo, Set the damage instigator to target when hit. this should be normally done by a middle system or send message.
		if (!AIC->GetTargetActor())
		{
			AIC->SetTargetActor(ChangeInstigator);
		}

		// Handle death
		if (NewHealth <= 0.0)
		{
			
			// Stop BT
			if (AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
			}

			// Ragdoll
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();

			// set lifespan
			SetLifeSpan(10.0f);
		}
	}
}


// Called every frame
void AWaffAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
