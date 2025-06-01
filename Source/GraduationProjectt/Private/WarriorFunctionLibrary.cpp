// Ufuk Pasaalioglu All Rights Reserved


#include "WarriorFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "Interfaces/PawnCombatInterface.h"
#include "GenericTeamAgentInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "WarriorGameplayTags.h"
#include "AbilitySystem/WarriorAttributeSet.h" // I added for reset attributes

#include "WarriorDebugHelper.h"

UWarriorAbilitySystemComponent* UWarriorFunctionLibrary::NativeGetWarriorASCFromActor(AActor* InActor)
{
    check(InActor);

    return Cast<UWarriorAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
}

void UWarriorFunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd)
{
    UWarriorAbilitySystemComponent* ASC = NativeGetWarriorASCFromActor(InActor);
    
    if (!ASC->HasMatchingGameplayTag(TagToAdd)) {
        ASC->AddLooseGameplayTag(TagToAdd);
    }
}

void UWarriorFunctionLibrary::RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove)
{
    UWarriorAbilitySystemComponent* ASC = NativeGetWarriorASCFromActor(InActor);

    if (ASC->HasMatchingGameplayTag(TagToRemove)) {
        ASC->RemoveLooseGameplayTag(TagToRemove);
    }
}

bool UWarriorFunctionLibrary::NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck)
{
    UWarriorAbilitySystemComponent* ASC = NativeGetWarriorASCFromActor(InActor);

    return ASC->HasMatchingGameplayTag(TagToCheck);
}

void UWarriorFunctionLibrary::BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EWarriorConfirmType& OutConfirmType)
{
    OutConfirmType = NativeDoesActorHaveTag(InActor, TagToCheck) ? EWarriorConfirmType::Yes : EWarriorConfirmType::No;
}

UPawnCombatComponent* UWarriorFunctionLibrary::NativeGetPawnCombatComponentFromActor(AActor* InActor)
{
    check(InActor);

    if (IPawnCombatInterface* PawnCombatInterface = Cast<IPawnCombatInterface>(InActor)) {
        return PawnCombatInterface->GetPawnCombatComponent();
    }
    return nullptr;
}

UPawnCombatComponent* UWarriorFunctionLibrary::BP_GetPawnCombatComponentFromActor(AActor* InActor, EWarriorValidType& OutValidType)
{
    UPawnCombatComponent* CombatComponent = NativeGetPawnCombatComponentFromActor(InActor);

    OutValidType = CombatComponent ? EWarriorValidType::Valid : EWarriorValidType::Invalid;

    return CombatComponent;
}

bool UWarriorFunctionLibrary::IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn)
{
    check(QueryPawn && TargetPawn);

    IGenericTeamAgentInterface* QueryTeamAgent = Cast<IGenericTeamAgentInterface>(QueryPawn->GetController());
    IGenericTeamAgentInterface* TargetTeamAgent = Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());

    if (QueryTeamAgent && TargetTeamAgent) {
        return QueryTeamAgent->GetGenericTeamId() != TargetTeamAgent->GetGenericTeamId();
    }

    return false;
}

float UWarriorFunctionLibrary::GetScalableFloatValueAtLevel(const FScalableFloat& InScalableFloat, float InLevel)
{
    return InScalableFloat.GetValueAtLevel(InLevel);
}

FGameplayTag UWarriorFunctionLibrary::ComputeHitReactDirectionTag(AActor* InAttacker, AActor* InVictim, float& OutAngleDifference)
{
    ensure(InAttacker && InVictim);

    const FVector VictimForward = InVictim->GetActorForwardVector();
    const FVector VictimToAttackerNormalized = (InAttacker->GetActorLocation() - InVictim->GetActorLocation()).GetSafeNormal();

    const float DotResult = FVector::DotProduct(VictimForward, VictimToAttackerNormalized);
    OutAngleDifference = UKismetMathLibrary::DegAcos(DotResult);

    const FVector CrossResult = FVector::CrossProduct(VictimForward, VictimToAttackerNormalized);

    if (CrossResult.Z < 0.f) {
        OutAngleDifference *= -1.f;
    }

    if (OutAngleDifference >= -45.f && OutAngleDifference <= 45.f) {
        return WarriorGameplayTags::Shared_Status_HitReact_Front;
    }
    else if (OutAngleDifference < -45.f && OutAngleDifference >= -135.f) {
        return WarriorGameplayTags::Shared_Status_HitReact_Left;
    }
    else if (OutAngleDifference < -135.f || OutAngleDifference >= 135.f) {
        return WarriorGameplayTags::Shared_Status_HitReact_Back;
    }
    else if (OutAngleDifference > 45.f && OutAngleDifference < 135.f) {
        return WarriorGameplayTags::Shared_Status_HitReact_Right;
    }

    return WarriorGameplayTags::Shared_Status_HitReact_Front;
}

bool UWarriorFunctionLibrary::IsValidBlock(AActor* InAttacker, AActor* InDefender)
{
    check(InAttacker && InDefender);

    const float DotResult = FVector::DotProduct(InAttacker->GetActorForwardVector(), InDefender->GetActorForwardVector());
    
 /*   const FString DebugString = FString::Printf(TEXT("Dot Result: %f %s"), DotResult, DotResult < -0.1f ? TEXT("Valid Block") : TEXT("InvalidBlock"));
    Debug::Print(DebugString, DotResult < -0.1f ? FColor::Green : FColor::Red);*/

    return DotResult<-0.1f;
}

void UWarriorFunctionLibrary::ResetCharacterAttributes(UWarriorAbilitySystemComponent* WarriorASC)
{
    if (!WarriorASC)
    {
        UE_LOG(LogTemp, Warning, TEXT("SetCurrentHealth: Invalid WarriorAbilitySystemComponent!"));
        return;
    }

    const UWarriorAttributeSet* ConstAttrSet = Cast<const UWarriorAttributeSet>(WarriorASC->GetAttributeSet(UWarriorAttributeSet::StaticClass()));
    if (!ConstAttrSet)
    {
        UE_LOG(LogTemp, Warning, TEXT("SetCurrentHealth: WarriorAttributeSet not found!"));
        return;
    }

    UWarriorAttributeSet* AttrSet = const_cast<UWarriorAttributeSet*>(ConstAttrSet);
    if (!AttrSet)
    {
        UE_LOG(LogTemp, Warning, TEXT("SetCurrentHealth: Could not cast to non-const UWarriorAttributeSet!"));
        return;
    }

    AttrSet->SetCurrentHealth(AttrSet->GetMaxHealth());
}

