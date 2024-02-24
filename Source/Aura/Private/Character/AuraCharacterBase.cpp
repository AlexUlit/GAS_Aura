// Copyright AUGames

#include "Character/AuraCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Aura/Aura.h"
#include "Components/CapsuleComponent.h"

AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

FVector AAuraCharacterBase::GetCombatSocketLocation()
{
	check(Weapon);
	return Weapon->GetSocketLocation(WeaponTipSocketName);
}

void AAuraCharacterBase::Die()
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulticastHandleDeath();
}

void AAuraCharacterBase::MulticastHandleDeath_Implementation()
{
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	//Weapon->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Dissolve();
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

void AAuraCharacterBase::AddCharacterAbilities()
{
	UAuraAbilitySystemComponent* AuraASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority()) return;
	
	AuraASC->AddCharacterAbilities(StartupAbilities);
	
}

void AAuraCharacterBase::Dissolve()
{
	if (CharacterDissolveMaterial)
	{
		UMaterialInstanceDynamic* CharacterMaterialInstance = UMaterialInstanceDynamic::Create(CharacterDissolveMaterial, this);
		GetMesh()->SetMaterial(0, CharacterMaterialInstance);
		StartCharacterDissolveTimeLine(CharacterMaterialInstance);
	}

	if (WeaponDissolveMaterial)
	{
		UMaterialInstanceDynamic* WeaponMaterialInstance = UMaterialInstanceDynamic::Create(WeaponDissolveMaterial, this);
		Weapon->SetMaterial(0, WeaponMaterialInstance);
		StartWeaponDissolveTimeLine(WeaponMaterialInstance);
	}
}




