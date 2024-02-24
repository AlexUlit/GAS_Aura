// Copyright AUGames

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class UAttributeSet;
class UAbilitySystemComponent;

UCLASS()
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const {return AttributeSet;}
	virtual FVector GetCombatSocketLocation() override;
	virtual void Die() override;

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();
	
protected:
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo();

	void ApplyGEtoSelf(const TSubclassOf<UGameplayEffect> GameplayEffect) const;
	virtual void InitializeDefaultAttributes() const;
	void AddCharacterAbilities();

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponTipSocketName;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	UFUNCTION(BlueprintImplementableEvent)
	void StartCharacterDissolveTimeLine(UMaterialInstanceDynamic* DissolveMaterial);

	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeLine(UMaterialInstanceDynamic* DissolveMaterial);

private:
	UPROPERTY(EditDefaultsOnly, Category = "GAS")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	void Dissolve();

	UPROPERTY(EditDefaultsOnly, Category = "Materials")
	TObjectPtr<UMaterialInstance> CharacterDissolveMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Materials")
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterial;
};
