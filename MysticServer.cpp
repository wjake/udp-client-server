#include "MysticServer.h"
#include "Sockets.h"
#include "Common/UdpSocketBuilder.h"

AMysticServer::AMysticServer()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMysticServer::BeginPlay()
{
	Super::BeginPlay();
	InitializeReceiver();
}

// Called when the game ends or when destroyed
void AMysticServer::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    StopReceiver();
    Super::EndPlay(EndPlayReason);
}

void AMysticServer::InitializeReceiver()
{
    // Create and configure the UDP socket
    UdpSocket = FUdpSocketBuilder(TEXT("UdpSocket"))
        .AsNonBlocking()
        .AsReusable()
        .BoundToAddress(FIPv4Address::Any)
        .BoundToPort(65432)
        .WithReceiveBufferSize(2 * 1024 * 1024);

    if (UdpSocket)
    {
        // Initialise the UDP socket receiver
        UdpSocketReceiver = MakeShareable(new FUdpSocketReceiver(UdpSocket, FTimespan::FromMilliseconds(100), TEXT("UdpSocketReceiver")));
        UdpSocketReceiver->OnDataReceived().BindUObject(this, &AMysticServer::OnDataReceived);
        UdpSocketReceiver->Start();
		UE_LOG(LogTemp, Warning, TEXT("Mystic Guestures server started."));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create UDP socket."));
    }
}

void AMysticServer::StopReceiver()
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

void AMysticServer::OnDataReceived(const TSharedPtr<FArrayReader, ESPMode::ThreadSafe>& Data, const FIPv4Endpoint& Endpoint)
{
    // Handle received gesture data
	FString ReceivedData = FString(ANSI_TO_TCHAR(reinterpret_cast<const char*>(Data->GetData())));
    UE_LOG(LogTemp, Warning, TEXT("Received data: %s from %s"), *ReceivedData, *Endpoint.ToString());

    // make calls to the other assets in-game
}
