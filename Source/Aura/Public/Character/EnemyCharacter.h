// Copyright AUGames

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/UCharacterClassInfo.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "EnemyCharacter.generated.h"

class AAuraAIController;
class UWidgetComponent;
class UBehaviorTree;

UCLASS()
class AURA_API AEnemyCharacter : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

	virtual void PossessedBy(AController* NewController) override;
	
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	
	virtual int32 GetPlayerLevel() override;

	virtual void Die() override;

	virtual UAnimMontage* GetHitReactMontage_Implementation() override;

	//Include OverlayWidgetController to get this delegate and not create a new one
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	void HitReactTagChanged(const FGameplayTag, int32 NewCount);

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	float BaseWalkSpeed = 200.f;
	
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	bool bHitReacting = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	float LifeSpan = 5.f;
protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AAuraAIController> AuraAIController;
private:
	void ChangeCustomDepthState(bool bState, USkeletalMeshComponent* TargetMesh);

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;
};
