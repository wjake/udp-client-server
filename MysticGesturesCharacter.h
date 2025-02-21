#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Common/UdpSocketReceiver.h"
#include "MysticGesturesCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpellEvent, int32, SpellID);

UCLASS(Blueprintable)
class MYSTICGESTURESGAME_API AMysticGesturesCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMysticGesturesCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	FSocket* UdpSocket;
	TSharedPtr<FUdpSocketReceiver> UdpSocketReceiver;

	void OnDataReceived(const TSharedPtr<FArrayReader, ESPMode::ThreadSafe>& Data, const FIPv4Endpoint& Endpoint);

	UFUNCTION(BlueprintCallable, Category = "MysticServer")
	void InitializeReceiver(int32 Port);

	UFUNCTION(BlueprintCallable, Category = "MysticServer")
	void StopReceiver();

	UPROPERTY(BlueprintAssignable, Category = "MysticServer")
    FSpellEvent OnSpellEvent;
};
