import serial
import time
import socket

# TCP Socket BEGIN (See Echo Client example): https://realpython.com/python-sockets/#echo-client-and-server
HOST = "127.0.0.1"  # The server's hostname or IP address
PORT = 65432  # The port used by the server
cybot_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  # Create a socket object
cybot_socket.connect((HOST, PORT))  # Connect to the socket  (Note: Server must first be running)

cybot = cybot_socket.makefile("rbw", buffering=0)  # makefile creates a file object out of a socket:  https://pythontic.com/modules/socket/makefile
# TCP Socket END

send_message = "Hello\n"  # 1) Hard code message to "Hello", or

cybot.write(send_message.encode())  # Convert String to bytes (i.e., encode), and send data to the server

print("Sent to server: " + send_message)

# Send messges to server until user sends "quit"
while send_message != 'quit\n':
    print("wait for server reply\n")
    rx_message = cybot.readline()  # Wait for a message, readline expects message to end with "\n"
    print("Got a message from server: " + rx_message.decode() + "\n")  # Convert message from bytes to String (i.e., decode)
    send_message = input("Enter a message (enter quit to exit):") + '\n'  # Enter next message to send to server
    cybot.write(send_message.encode())  # Convert String to bytes (i.e., encode), and send data to the server

print("Client exiting, and closing file descriptor, and/or network socket\n")
time.sleep(2)  # Sleep for 2 seconds
cybot.close()  # Close file object associated with the socket or UART
cybot_socket.close()  # Close the socket (NOTE: comment out if using UART interface, only use for network socket option)