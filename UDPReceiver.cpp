#include "UDPReceiver.h"
#include "Sockets.h"
#include "Common/UdpSocketBuilder.h"

void UUDPReceiver::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    StopReceiver();
}

void UUDPReceiver::StartUDPReceiver(const FString& SocketName, const FString& IP, const int32 Port, const int32 BufferSize)
{
	FIPv4Address Addr;
    FIPv4Address::Parse(IP, Addr);
	FIPv4Endpoint Endpoint(Addr, Port);

    // Create and configure the UDP socket
    UdpSocket = FUdpSocketBuilder(SocketName)
        .AsNonBlocking()
        .AsReusable()
        .BoundToEndpoint(Endpoint)
        .WithReceiveBufferSize(BufferSize)
        .Build();

    if (UdpSocket)
    {
        // Initialise the UDP socket receiver
        UdpSocketReceiver = MakeShareable(new FUdpSocketReceiver(UdpSocket, FTimespan::FromMilliseconds(100), TEXT("UdpSocketReceiver")));
        UdpSocketReceiver->OnDataReceived().BindUObject(this, &UUDPReceiver::OnDataReceived);
        UdpSocketReceiver->Start();
        UE_LOG(LogTemp, Warning, TEXT("UDP receiver started."));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create UDP receiver."));
    }
}

void UUDPReceiver::StopReceiver()
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

void UUDPReceiver::OnDataReceived(const TSharedPtr<FArrayReader, ESPMode::ThreadSafe>& Data, const FIPv4Endpoint& Endpoint)
{
    // Null-terminate the received data
    TArray<uint8> ReceivedDataArray = *Data;
    ReceivedDataArray.Add(0);

    // Handle received gesture data
    FString ReceivedDataString = FString(ANSI_TO_TCHAR(reinterpret_cast<const char*>(ReceivedDataArray.GetData())));
    UE_LOG(LogTemp, Warning, TEXT("Received data: %s from %s"), *ReceivedDataString, *Endpoint.ToString());

    // Broadcast received data to delegate
    OnMessageRecievedEvent.Broadcast(ReceivedDataString);

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