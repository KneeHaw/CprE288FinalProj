# Author: Ethan Rogers (kneehaw@iastate.edu)
# Date: 09/27/2023
# Description: Simple Threaded Client for CprE 288 Cybot

import socket
from msvcrt import getch
import threading
from multiprocessing import Process
import numpy as np
from matplotlib import pyplot as plt
import time
import actual_socket.database as db

# Define Client Variables
HOST = "192.168.1.1"
PORT = 288
# Establish TCP connection
conn = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# graph_thread = threading.Thread(target=graph_data, args=(), daemon=True)
global exit_
global draw_
deg_inc = 2
lines = int(360 / deg_inc)
global_draw = 0


def build_string(character, string):
    string = string + character
    return string


def get_data(file_name="client_log.txt"):
    # If graph data, save data to another file, and clear the current data file
    array = np.genfromtxt(file_name, invalid_raise=False)
    array = np.array(((array[:, 0] * np.pi) / 180, array[:, 1]))  # [theta, r]
    r_max = np.max(np.abs(array[1:]))
    # Convert data to polar coordinates
    x_data = np.array((array[1, :] * np.cos(array[0, :])))
    y_data = np.array((array[1, :] * np.sin(array[0, :])))

    return x_data, y_data, r_max


def draw_plot():
    while True:
        with open("client_log.txt", "r") as f:
            num_lines = len(f.readlines())
            if num_lines < 3:
                continue
        x_data, y_data, r_max = get_data()
        # Draw Semi Circle of Object Range Seen
        arranged_degrees = np.arange(0, 2 * np.pi, 0.001)
        semi_circle_x = np.array(np.cos(arranged_degrees) * r_max)
        semi_circle_y = np.array(np.sin(arranged_degrees) * r_max)

        # Create Fields at Theta Degrees Interval (i)
        i = 8
        i_rad = i * np.pi / 180
        arranged_field_degrees = np.arange(0, 2 * np.pi, i_rad)
        r = np.arange(0, r_max, 0.01)
        for j in range(int(360 / i)):
            field_lines_x = np.array(np.cos(arranged_field_degrees[j]) * r[:])
            field_lines_y = np.array(np.sin(arranged_field_degrees[j]) * r[:])
            plt.plot(field_lines_x, field_lines_y, 0.1, c="black", alpha=0.3)
        plt.plot(semi_circle_x, semi_circle_y, 0.2, c="black", alpha=0.3)
        plt.plot(x_data, y_data, linewidth=3)
        plt.fill_between(x_data, y_data)
        plt.draw()
        plt.pause(0.1)
        plt.clf()


def orderHandler():
    # Infinite loop to check for orders
    while True:
        orders = db.getOrders()
        if orders:
            # Send orders to robot
            for order in orders:
                # Send order house character (The house to deliver to)
                conn.send(order.house.character)
                time.sleep(1)
            conn.send("e".encode())
            print("Orders Sent")


def setOrderStatus(order_id: str, status: str):
    db.updateOrderStatus(order_id, status)


def messageHandler():
    while True:
        try:
            message = conn.recv(1024)
            if message:
                string = message.decode()
                print(string, end="", flush=True)
                with open("client_log.txt", "a") as f:
                    # print(f"Written {string}")
                    f.write(string)
        except Exception as E:
            print(f"Error getting message: {E}")
            globals()["exit_"] = 1
            pass


def keyPress():
    while True:
        char = getch()

        # CyBot Characters: w, a, s, d, q, o, p
        if char == "x".encode():
            conn.send(char)
            print("'x': Closing Client...")
            globals()["exit_"] = 1
        elif char == "q".encode() or char == "e".encode() or "l".encode:
            f = open("client_log.txt", "w")
            f.close()
            conn.send(char)
        else:
            conn.send(char)


def main():
    try:
        print("Attempting Connection...")
        conn.connect((HOST, PORT))
        print("Connection Successful!\n")
    except Exception as E:
        print(f"Error: {E}\nFailed to connect, exiting program.")
        exit()
    globals()["exit_"] = 0
    print(
        "|---Client Commands ---\n| Exit program 'x'\n| Graph Data 'g'\n| Clear data file 'c'\n| Send long string 'l'"
    )

    # Both message_thread and key_press_thread are blocking, create daemon threads (Don't need to finish for exit)
    message_thread = threading.Thread(target=messageHandler, args=(), daemon=True)
    order_thread = threading.Thread(target=orderHandler, args=(), daemon=True)
    key_press_thread = threading.Thread(target=keyPress, args=(), daemon=True)
    graph_process = Process(target=draw_plot, args=(), daemon=True)
    message_thread.start()
    key_press_thread.start()
    # graph_process.start()
    order_thread.start()

    # Hold this loop until global exit is set
    while True:
        if exit_:
            break
        else:
            pass


if __name__ == "__main__":
    main()
