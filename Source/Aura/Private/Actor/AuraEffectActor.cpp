// Copyright AUGames

#include "Actor/AuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneComponent"));
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!TargetASC) return;

	check(GameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, EffectLevel, EffectContextHandle);
	const FActiveGameplayEffectHandle GameplayEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def->DurationPolicy == EGameplayEffectDurationType::Infinite;
	
	if (bIsInfinite)
	{
		ActiveEffectsHandles.Add(GameplayEffectHandle, GameplayEffectClass);
	}
	if (DestroyOnEffectApplication && !bIsInfinite)
	{
		Destroy();
	}
}

void AAuraEffectActor::OnOverlap(AActor* Target)
{
	if (Target->ActorHasTag(FName("Enemy")) && !ApplyEffectsToEnemies)
	{
		return;
	}
	
	if (!InstantGameplayEffects.IsEmpty())
	{
		for (auto const EffectPars : InstantGameplayEffects)
		{
			if (EffectPars.Value == EApplicationPolicy::ApplyOnOverlap)
			{
				ApplyEffectToTarget(Target, EffectPars.Key);
			}
		}
	}
	
	if (!DurationGameplayEffects.IsEmpty())
	{
		for (auto const EffectPars : DurationGameplayEffects)
		{
			if (EffectPars.Value == EApplicationPolicy::ApplyOnOverlap)
			{
				ApplyEffectToTarget(Target, EffectPars.Key);
			}
		}
	}
	
	if (!InfiniteGameplayEffects.IsEmpty())
	{
		for (auto const EffectPars : InfiniteGameplayEffects)
		{
			if (EffectPars.Value.ApplicationPolicy == EApplicationPolicy::ApplyOnOverlap)
			{
				ApplyEffectToTarget(Target, EffectPars.Key);
			}
		}
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* Target)
{
	if (Target->ActorHasTag(FName("Enemy")) && !ApplyEffectsToEnemies)
	{
		return;
	}
	
	if (!InstantGameplayEffects.IsEmpty())
	{
		for (auto const EffectPars : InstantGameplayEffects)
		{
			if (EffectPars.Value == EApplicationPolicy::ApplyOnEndOverlap)
			{
				ApplyEffectToTarget(Target, EffectPars.Key);
			}
		}
	}
	
	if (!DurationGameplayEffects.IsEmpty())
	{
		for (auto const EffectPars : DurationGameplayEffects)
		{
			if (EffectPars.Value == EApplicationPolicy::ApplyOnEndOverlap)
			{
				ApplyEffectToTarget(Target, EffectPars.Key);
			}
		}
	}

	if (!InfiniteGameplayEffects.IsEmpty())
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
		if (!TargetASC) return;

		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		
		for (auto const EffectPars : InfiniteGameplayEffects)
		{
			if (EffectPars.Value.RemovePolicy == ERemovePolicy::RemoveOnEndOverlap)
			{
				for (auto HandlePair : ActiveEffectsHandles)
				{
					if (HandlePair.Key.GetOwningAbilitySystemComponent() == TargetASC && HandlePair.Value == EffectPars.Key)
					{
						TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
						HandlesToRemove.Add(HandlePair.Key);
					}
				}
			}
		}
		
		for (auto &Handle : HandlesToRemove)
		{
			ActiveEffectsHandles.FindAndRemoveChecked(Handle);
		}
	}
}


