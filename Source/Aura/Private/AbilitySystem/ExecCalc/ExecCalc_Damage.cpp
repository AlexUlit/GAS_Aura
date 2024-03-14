// Copyright AUGames


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Interaction/CombatInterface.h"


DEFINE_LOG_CATEGORY_STATIC(Log_ExecCalc, All, All);

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
	
	RelevantAttributesToCapture.Add(DamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
}


void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC->GetAvatarActor();
	AActor* TargetAvatar = TargetASC->GetAvatarActor();
	ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
	ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);
	UCharacterClassInfo* SourceClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	UCharacterClassInfo* TargetClassInfo =  UAuraAbilitySystemLibrary::GetCharacterClassInfo(TargetAvatar);
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	EvaluateParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();

	float Damage = 0.f;

	for (const TTuple<FGameplayTag, FGameplayTag> Pair : FAuraGameplayTags::Get().DamageTypesToResistance)
	{
		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag ResistanceTag = Pair.Value;

		checkf(AuraDamageStatics().TagsToCaptureDef.Contains(ResistanceTag), TEXT("TagsToCaptureDefs doesn't contain Tag: [%s] in ExecCalc_Damage"), *ResistanceTag.ToString());
		const FGameplayEffectAttributeCaptureDefinition CaptureDef = AuraDamageStatics().TagsToCaptureDef[ResistanceTag];

		float DamageTypeAmount = Spec.GetSetByCallerMagnitude(Pair.Key);
		
		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluateParameters, Resistance);
		Resistance = FMath::Clamp(Resistance, 0.f, 100.f);
		
		
		DamageTypeAmount *= (100.f - Resistance) / 100.f;  
		
		Damage += DamageTypeAmount;
	}
	
	float BlockChance = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluateParameters, BlockChance);
	BlockChance = FMath::Max<float>(0.f, BlockChance);
	
	if (BlockChance >= FMath::RandRange(0.f, 100.f))
	{
		Damage /= 2.f;
		UAuraAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, true);
	}

	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluateParameters, TargetArmor);
	TargetArmor = FMath::Max<float>(0.f, TargetArmor);
	
	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluateParameters, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(0.f, SourceArmorPenetration);

	//Get coefficients values from DT_DamageCalculationCoefficients 
	const float ArmorPenetrationCoefficient = SourceClassInfo->DamageCalculationCoefficients.Get()->FindCurve(FName("ArmorPenetration"), FString())->Eval(SourceCombatInterface->GetPlayerLevel());
	UE_LOG(Log_ExecCalc, Warning, TEXT("ArmorPenetrationCoefficient is: %f"), ArmorPenetrationCoefficient);
	const float EffectiveArmorCoefficient = TargetClassInfo->DamageCalculationCoefficients.Get()->FindCurve(FName("EffectiveArmor"), FString())->Eval(TargetCombatInterface->GetPlayerLevel());
	UE_LOG(Log_ExecCalc, Warning, TEXT("EffectiveArmorCoefficient is: %f"), EffectiveArmorCoefficient);
	
	//ArmorPenetration ignores a percentage of the Target's Armor
	const float EffectiveArmor = TargetArmor *= (100.f - SourceArmorPenetration * ArmorPenetrationCoefficient) / 100.f;
	//Armor ignores a percentage of incoming Damage
	Damage *= (100.f - EffectiveArmor * EffectiveArmorCoefficient) / 100.f;

	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluateParameters, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(0.f, SourceCriticalHitChance);

	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef, EvaluateParameters, TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(0.f, TargetCriticalHitResistance);

	const float CriticalHitResistanceCoefficient = TargetClassInfo->DamageCalculationCoefficients.Get()->FindCurve(FName("CriticalHitResistance"), FString())->Eval(TargetCombatInterface->GetPlayerLevel());
	
	SourceCriticalHitChance = FMath::Max(0.f, SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanceCoefficient);

	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluateParameters, SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(0.f, SourceCriticalHitDamage);
	
	if (SourceCriticalHitChance > 0.f && SourceCriticalHitChance >= FMath::RandRange(0.f, 100.f))
	{
		Damage = Damage * 2.f + SourceCriticalHitDamage;
		UAuraAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, true);
	}
	
	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}

/*EASY way to get value of attribute
 *const float EasyWayBlockChance = TargetASC->GetNumericAttribute(UAuraAttributeSet::GetBlockChanceAttribute());
 *UE_LOG(Log_ExecCalc, Warning, TEXT("ArmorPenet is: %f"), ArmorPenet);
 */
