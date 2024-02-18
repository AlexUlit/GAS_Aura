// Copyright AUGames


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"


UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* NewObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return NewObj;
}

void UTargetDataUnderMouse::Activate()
{
	UE_LOG(LogTemp, Warning, TEXT("ASC owner is: %s"), *AbilitySystemComponent.Get()->GetOwner()->GetName());
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		SendMouseCursorData();
	}
	else //We are on the server, so listen for target data
	{
		//Chached this vars because they use in AbilityTargetDataSetDelegate & CallReplicatedTargetDataDelegatesIfSet
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey PredictionKey = GetActivationPredictionKey();
		
		//Listen when TargetData comes to the server
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, PredictionKey).AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallBack);
		
		//if we're too late and the target data has already been sent and the delegate has already been broadcast
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, PredictionKey);
		if (bCalledDelegate)
		{
			//Tell the server, look, we're still waiting for remote player data
			SetWaitingOnRemotePlayerData();
		}
	}
	
}


/*If locally controller, call this function and broadcast delegate
 *If we're on the SERVER and we're locally controlled, broadcasting delegate
 *If we're on the CLIENT and we're locally controlled, we're broadcasting the delegate, but we're also sending data to the server.
 */
void UTargetDataUnderMouse::SendMouseCursorData()
{
	/*So what we're doing is we're saying that everything within this scope should be predicted.
	 *So server, please allow us to do what we're doing here locally, no problem. 
	 *And then the server will do it when the server knows about it.
     */  
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());

	//Get cursor hit result
	const APlayerController* PC =  Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult UnderCursorHit;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, UnderCursorHit);

	//Write UnderCursorHit to DataHandle(FGameplayAbilityTargetDataHandle)
	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit;
	Data->HitResult = UnderCursorHit;
	DataHandle.Add(Data);

	//Replicates(Send) targeting data to the server
	AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey);
	
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		//Send this data from AbilityTask
		ValidData.Broadcast(DataHandle);
	}
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallBack(const FGameplayAbilityTargetDataHandle& DataHandle,
	FGameplayTag ActivationTag)
{
	//Telling to ASC - You don't need to store TargetData, you don't need to keep it cached anymore.
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		//Send this data from AbilityTask
		ValidData.Broadcast(DataHandle);
	}
}
