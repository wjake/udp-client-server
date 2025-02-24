import socket

# Define local server address and port 
server_address = ('127.0.0.1', 1234)

# Create connection to UDP socket
udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

try:
    # Send message to socket data
    message = "message".encode('utf-8')
    print(f'Sent data: {message} to {server_address}')
    sent = udp_socket.sendto(message, server_address)

    # Receive response (optional)
    print('Waiting for response...')
    data, server = udp_socket.recvfrom(1024)
    print(f'Received: {data} from {server}')

finally:
    # Close the socket
    print('Closing socket.')
    udp_socket.close()
