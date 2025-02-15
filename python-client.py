import socket

# Define local server address and port 
server_address = ('127.0.0.1', 65432)

# Create connection to UDP socket
mystic_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

try:
    # Send gesture data
    message = b'Gesture detected: 1'
    print(f'Sent data: {message} to {server_address}')
    sent = mystic_socket.sendto(message, server_address)

    # Receive response (optional, but could be useful?)
    print('Waiting for response...')
    data, server = mystic_socket.recvfrom(4096)
    print(f'Received: {data} from {server}')

finally:
    # Close the socket
    print('Closing socket.')
    mystic_socket.close()
