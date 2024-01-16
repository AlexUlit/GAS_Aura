// Copyright AUGames

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "AuraEffectActor.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;
class USphereComponent;
struct FActiveGameplayEffectHandle;


UENUM(BlueprintType)
enum class EApplicationPolicy : uint8
{
	DoNotApply,
	ApplyOnOverlap,
	ApplyOnEndOverlap
};

UENUM(BlueprintType)
enum class ERemovePolicy : uint8
{
	RemoveOnEndOverlap,
	DoNotRemove
};

USTRUCT(BlueprintType)
struct FPolicy
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EApplicationPolicy ApplicationPolicy = EApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ERemovePolicy RemovePolicy = ERemovePolicy::RemoveOnEndOverlap;
};


UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraEffectActor();
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* Target);

	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* Target);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AppliedEffects")
	TMap<TSubclassOf<UGameplayEffect>, EApplicationPolicy> InstantGameplayEffects;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AppliedEffects")
	TMap<TSubclassOf<UGameplayEffect>, EApplicationPolicy> DurationGameplayEffects;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AppliedEffects")
	TMap<TSubclassOf<UGameplayEffect>, FPolicy> InfiniteGameplayEffects;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AppliedEffects")
	float EffectLevel = 1.f;

	TMap<FActiveGameplayEffectHandle, TSubclassOf<UGameplayEffect>> ActiveEffectsHandles;
};
