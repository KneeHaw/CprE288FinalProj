import numpy as np
from matplotlib import pyplot as plt
import math
from scipy.optimize import curve_fit
import time


def create_data_array(file_name='client_log.txt'):
    array = np.genfromtxt(file_name, skip_header=0)
    return array


def graph_data(array):
    start_t = time.time()

    def func2fit(x, a, b, c):
        return np.exp(a / (x ** b)) + c
        # Optimal = e ^ (3.6934023 / (x ** 0.21835123)) + -13.58386622

    def func3fit(x, a, b, c):
        return (x ** a) + c

    x_data = np.array((array[:, 1]))
    # rng = np.random.default_rng().normal(size=x_data.size)
    y_data = np.array((array[:, 0]))
    # sample_y = func2fit(x_data, 3.70192, 0.22462709, -11.14918499)

    # plt.plot(x_data, y_data, 'b-')
    # plt.plot(x_data, sample_y, 'r-')
    popt, pcov = curve_fit(func3fit, x_data, y_data)
    print(popt)
    plt.plot(x_data, func3fit(x_data, *popt), 'r-')

    print("Time to run: " + str(time.time()-start_t))
    plt.show()


def main():
    graph_data(create_data_array())


if __name__ == '__main__':
    main()

    # pred_y1 = np.array(math.e ** (297 / (sample ** .65)))  # x > 830
    # pred_y2 = np.array(math.e ** (3700 / (sample ** 1.03)))  # x <= 730
    # pred_y3 = np.array(math.e ** (813 / (sample ** 0.8)))  # 730 < x <= 830
    # format = e ^ (a / (x ^ b)) + c
    # pred_f = pred_y1 * (sample > 830) + pred_y3 * (sample <= 830) * (sample > 730) + pred_y2 * (sample < 730)
