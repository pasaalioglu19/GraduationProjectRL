// Ufuk Pasaalioglu All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/WarriorBaseAnimInstance.h"
#include "WarriorHeroLinkedAnimLayer.generated.h"

class UWarriorHeroAnimInstance;

/**
 * 
 */
UCLASS()
class GRADUATIONPROJECTT_API UWarriorHeroLinkedAnimLayer : public UWarriorBaseAnimInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	UWarriorHeroAnimInstance* GetHeroAnimInstance() const;
};
