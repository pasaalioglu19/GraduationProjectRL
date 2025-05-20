// Ufuk Pasaalioglu All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "DataAsset_HeroAIStartUpData.generated.h"

class UWarriorHeroGameplayAbility;

/**
 * 
 */
UCLASS()
class GRADUATIONPROJECTT_API UDataAsset_HeroAIStartUpData : public UDataAsset_StartUpDataBase
{
	GENERATED_BODY()
public:
	virtual void GiveToAbilitySystemComponent(UWarriorAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf <UWarriorHeroGameplayAbility> > HeroAICombatAbilities;
};