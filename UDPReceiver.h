#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Common/UdpSocketReceiver.h"
#include "UDPReceiver.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageReceivedEvent, const FString&, ReceivedMessage);

UCLASS(ClassGroup = (Server), meta = (BlueprintSpawnableComponent))
class UUDPReceiver : public UActorComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "UDP")
	void StartUDPReceiver(const FString& SocketName = "Socket", const FString& IP = "0.0.0.0",
		const int32 Port = 65432, const int32 BufferSize = 1024);

	UPROPERTY(BlueprintAssignable, Category = "UDP")
    FMessageReceivedEvent OnMessageRecievedEvent;

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	FSocket* UdpSocket;
	TSharedPtr<FUdpSocketReceiver> UdpSocketReceiver;

	void StopReceiver();
	void OnDataReceived(const TSharedPtr<FArrayReader, ESPMode::ThreadSafe>& Data, const FIPv4Endpoint& Endpoint);
};