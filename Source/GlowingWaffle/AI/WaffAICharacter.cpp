// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "WaffAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

// Sets default values
AWaffAICharacter::AWaffAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AttriComp = CreateDefaultSubobject<UWaffAttributeComponent>(TEXT("AttriComp"));
	TimeToHit = "TimeToHit";
}

// Post init
void AWaffAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttriComp->OnHealthChanged.AddDynamic(this, &AWaffAICharacter::OnHealthChanged);
}

// Called when the game starts or when spawned
void AWaffAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWaffAICharacter::OnHealthChanged(AActor* ChangeInstigator, UWaffAttributeComponent* OwingComp, float NewHealth,
	float Delta)
{
	if (Delta < 0.0)
	{
		AWaffAIController* AIC = Cast<AWaffAIController>(GetController());
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHit, GetWorld()->TimeSeconds);
		// Set the damage instigator to target when hit.
		if(ChangeInstigator != this)
		{
			AIC->SetTargetActor(ChangeInstigator);
		}


		// Handle death
		if(NewHealth <= 0.0)
		{
			// Stop BT
			if(AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
			}
			
			// Ragdoll
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");

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


