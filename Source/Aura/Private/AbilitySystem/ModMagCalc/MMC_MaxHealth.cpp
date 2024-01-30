// Copyright AUGames


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	//Specify what attribute should be captured
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	//From whose side capture this attribute
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorDef.bSnapshot = false;
	//Add this attribute to list of captured attributes
	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	float Vigor = 0.f;
	
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluateParameters,Vigor);
	
	Vigor = FMath::Max(0.f, Vigor);
	const float PlayerLevel = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject())->GetPlayerLevel();
	
	return 80.f + 2.5f * Vigor + 10.f * PlayerLevel;
}
