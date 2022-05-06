// Fill out your copyright notice in the Description page of Project Settings.


#include "WaffAICharacter.h"

// Sets default values
AWaffAICharacter::AWaffAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AttriComp = CreateDefaultSubobject<UWaffAttributeComponent>(TEXT("AttriComp"));

}

// Called when the game starts or when spawned
void AWaffAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWaffAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


