import matplotlib.pyplot as plt
import random

def random_variance():
    arr = list()
    average = 0.0
    for i in range(4):
        arr.append(random.random())
        average += arr[i]

    average /= 4.0
    ret = 0.0
    for i in range(4):
        ret += (arr[i] - average) ** 2

    return ret



def create_sample_file(num_points):
    f = open("distribution_data.txt", "w+")
    for i in range(num_points):
        cur_val = random_variance()
        f.write(str(cur_val) + "\n")

    f.close()



def draw_graph(num_points):
    count = [0 for i in range(101)]
    value = [i / 100.0 for i in range(101)]

    for i in range(num_points):
        cur_val = random_variance()
        count[int(cur_val * 100.0)] += 1

    for i in range(len(count)):
        count[i] /= num_points

    plt.plot(value, count)
    plt.show()



def min_values(n):
    arr = list()
    for i in range(n):
        arr.append(random_variance())

    return min(arr)


def expected_min(num_points, n):
    sum = 0.0
    for i in range(num_points):
        sum += min_values(n)

    return sum / num_points



num_points = 100000

draw_graph(num_points)

print(expected_min(num_points, 100))