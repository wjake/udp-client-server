#include "MysticGesturesCharacter.h"
#include "Sockets.h"
#include "Common/UdpSocketBuilder.h"

// Sets default values
AMysticGesturesCharacter::AMysticGesturesCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMysticGesturesCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMysticGesturesCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMysticGesturesCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMysticGesturesCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    StopReceiver();
    Super::EndPlay(EndPlayReason);
}

void AMysticGesturesCharacter::InitializeReceiver(int32 Port)
{
    // Create and configure the UDP socket
    UdpSocket = FUdpSocketBuilder(TEXT("UdpSocket"))
        .AsNonBlocking()
        .AsReusable()
        .BoundToAddress(FIPv4Address::Any)
        .BoundToPort(Port)
        .WithReceiveBufferSize(2 * 1024 * 1024);

    if (UdpSocket)
    {
        // Initialise the UDP socket receiver
        UdpSocketReceiver = MakeShareable(new FUdpSocketReceiver(UdpSocket, FTimespan::FromMilliseconds(100), TEXT("UdpSocketReceiver")));
        UdpSocketReceiver->OnDataReceived().BindUObject(this, &AMysticGesturesCharacter::OnDataReceived);
        UdpSocketReceiver->Start();
		UE_LOG(LogTemp, Warning, TEXT("Mystic Guestures server started."));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create UDP socket."));
    }

    OnSpellEvent.Broadcast(Port * 2); // Example output
}

void AMysticGesturesCharacter::StopReceiver()
{
    if (UdpSocketReceiver.IsValid())
    {
        UdpSocketReceiver->Stop();
        UdpSocketReceiver = nullptr;
    }
    if (UdpSocket)
    {
        UdpSocket->Close();
        ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(UdpSocket);
        UdpSocket = nullptr;
    }
}

void AMysticGesturesCharacter::OnDataReceived(const TSharedPtr<FArrayReader, ESPMode::ThreadSafe>& Data, const FIPv4Endpoint& Endpoint)
{
    // Handle received gesture data
	FString ReceivedData = FString(ANSI_TO_TCHAR(reinterpret_cast<const char*>(Data->GetData())));
    UE_LOG(LogTemp, Warning, TEXT("Received data: %s from %s"), *ReceivedData, *Endpoint.ToString());

    // do work with the received data
    // ...
	// OnSpellEvent.Broadcast(1234); // Example trigger

    // Prepare response
    FString ResponseData = TEXT("Data received & processed");
    TArray<uint8> ResponseBytes;
    FTCHARToUTF8 Convert(*ResponseData);
    ResponseBytes.Append((uint8*)Convert.Get(), Convert.Length());

    // Send the response to the mystic client
    int32 BytesSent;
    UdpSocket->SendTo(ResponseBytes.GetData(), ResponseBytes.Num(), BytesSent, *Endpoint.ToInternetAddr());
    UE_LOG(LogTemp, Warning, TEXT("Sent data: %s to %s"), *ResponseData, *Endpoint.ToString());
}