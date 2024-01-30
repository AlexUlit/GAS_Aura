// Copyright AUGames

#include "Character/AuraCharacterBase.h"

#include "AbilitySystemComponent.h"

AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraCharacterBase::InitAbilityActorInfo()
{
}

void AAuraCharacterBase::ApplyGEtoSelf(const TSubclassOf<UGameplayEffect> GameplayEffect) const
{
	check(AbilitySystemComponent);
	
	FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle  EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1.f, ContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
}

void AAuraCharacterBase::InitializeDefaultAttributes() const
{
	check(DefaultPrimaryAttributes);
	check(DefaultSecondaryAttributes);
	check(DefaultVitalAttributes);

	ApplyGEtoSelf(DefaultPrimaryAttributes);
	ApplyGEtoSelf(DefaultSecondaryAttributes);
	ApplyGEtoSelf(DefaultVitalAttributes);
}




