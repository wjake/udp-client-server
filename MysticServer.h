#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Common/UdpSocketReceiver.h"
#include "MysticServer.generated.h"

UCLASS()
class TCPTEST_API AMysticServer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMysticServer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	FSocket* UdpSocket;
	TSharedPtr<FUdpSocketReceiver> UdpSocketReceiver;

	void OnDataReceived(const TSharedPtr<FArrayReader, ESPMode::ThreadSafe>& Data, const FIPv4Endpoint& Endpoint);
	void InitializeReceiver();
	void StopReceiver();
};
