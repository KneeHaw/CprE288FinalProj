import numpy as np
from matplotlib import pyplot as plt
import time


def get_data(file_name='client_log.txt', num_lines=180):
    array = np.genfromtxt(file_name, skip_header=1, max_rows=num_lines)  # If graph data, save data to another file, and clear the current data file
    array = np.array(((array[:, 0] * np.pi) / 180, array[:, 2]))  # [theta, r]
    r_max = np.max(np.abs(array[1:]))
    # Convert data to polar coordinates
    x_data = np.array((array[1, :] * np.cos(array[0, :])))
    y_data = np.array((array[1, :] * np.sin(array[0, :])))

    return x_data, y_data, r_max


def draw_plot(lines):
    while True:
        x_data, y_data, r_max = get_data(num_lines=lines)
        # Draw Semi Circle of Object Range Seen
        arranged_degrees = np.arange(0, np.pi, .001)
        semi_circle_x = np.array(np.cos(arranged_degrees) * r_max)
        semi_circle_y = np.array(np.sin(arranged_degrees) * r_max)

        # Create Fields at Theta Degrees Interval (i)
        i = 8
        i_rad = i * np.pi / 180
        arranged_field_degrees = np.arange(0, np.pi + i_rad, i_rad)
        r = np.arange(0, r_max, 0.01)
        for j in range(int(180 / i) + 1):
            field_lines_x = np.array(np.cos(arranged_field_degrees[j]) * r[:])
            field_lines_y = np.array(np.sin(arranged_field_degrees[j]) * r[:])
            plt.plot(field_lines_x, field_lines_y, 0.1, c='black', alpha=0.3)
        plt.plot(semi_circle_x, semi_circle_y, 0.2, c='black', alpha=0.3)
        plt.plot(x_data, y_data, linewidth=3)
        plt.fill_between(x_data, y_data)
        plt.draw()
        plt.pause(1)


if __name__ == '__main__':
    draw_plot(60)
