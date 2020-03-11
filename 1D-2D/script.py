import numpy as np
import matplotlib.pyplot as plt
import random

import linear_biclustering as opt
import greedy_spanning_tree as greedy
import tsp_2D as tsp



def random_matrix(n, m):
    matrix = np.random.rand(n, m)
    return matrix



def random_binary_matrix(n, m):
    matrix = np.array([[random.randint(0, 1) for j in range(m)] for i in range(n)])
    return matrix



def random_permutation_matrix(matrix):
    n, m = matrix.shape

    permutation_rows =  np.random.permutation(n)
    permutation_columns = np.random.permutation(m)

    matrix = matrix[:, permutation_columns]
    matrix = matrix[permutation_rows, :]

    return matrix



def write_matrix_file(file_path, matrix):
    f = open(file_path, "w")
    n, m = matrix.shape
    
    f.write(str(n) + " " + str(m) + "\n")
    for i in range(n):
        for j in range(m):
            f.write(str(matrix[i][j]) + " ")
        f.write("\n")
        
    f.close()



def read_matrix_file(file_path):
    f = open(file_path, "r")

    n, m = list(map(int, f.readline().split(" ")))

    matrix = list()
    for i in range(n):
        line = f.readline().split(" ")
        line.pop()                      # remove newline

        line = list(map(float, line))
        matrix.append(line)

    f.close()

    matrix = np.array(matrix)

    '''
    for i in range(n):
        for j in range(m):
            print(matrix[i][j], end=" ")
        print() 
    '''
    return matrix



def generate_random_matrices():
	matrix = random_matrix(10, 10)
	write_matrix_file("random_matrix_small_1.txt", matrix)

	matrix = random_matrix(10, 10)
	write_matrix_file("random_matrix_small_2.txt", matrix)

	matrix = random_matrix(100, 100)
	write_matrix_file("random_matrix_large_1.txt", matrix)

	matrix = random_matrix(100, 100)
	write_matrix_file("random_matrix_large_2.txt", matrix)


	matrix = random_binary_matrix(10, 10)
	write_matrix_file("random_binary_matrix_small_1.txt", matrix)

	matrix = random_binary_matrix(10, 10)
	write_matrix_file("random_binary_matrix_small_2.txt", matrix)

	matrix = random_binary_matrix(100, 100)
	write_matrix_file("random_binary_matrix_large_1.txt", matrix)

	matrix = random_binary_matrix(100, 100)
	write_matrix_file("random_binary_matrix_large_2.txt", matrix)







if (__name__ == "__main__"):
	"""
	file_paths = ["random_matrix_small_1.txt", "random_matrix_small_2.txt", "random_matrix_large_1.txt", "random_matrix_large_2.txt",
				  "random_binary_matrix_small_1.txt", "random_binary_matrix_small_2.txt", "random_binary_matrix_large_1.txt", "random_binary_matrix_large_2.txt",
				  "brown_selected_small.txt", "brown_selected_large.txt", "caribbean_matrix_small.txt", "caribbean_matrix_large.txt",
				  "flower_matrix_large.txt", "flower_matrix_large.txt"]
	
	"""

	file_paths = ["brown_selected_small.txt", "brown_selected_large.txt", "caribbean_matrix_small.txt", "caribbean_matrix_large.txt",
				  "flower_matrix_small.txt", "flower_matrix_large.txt"]
	

	for path in file_paths:
		matrix = read_matrix_file(path)
		matrix = random_permutation_matrix(matrix)
		
		tsp_scores = tsp.TSP_2D(matrix, 5)
		tsp_iterations = [i for i in range(len(tsp_scores))]

		greedy_scores = greedy.greedy_2D(matrix)
		greedy_iterations = [i for i in range(len(greedy_scores))]
		
		# Final scores
		optimal_score = -1.0
		greedy_score = min(greedy_scores)
		tsp_score = min(tsp_scores)


		# Create plots
		fig, ax = plt.subplots()
		ax.plot(tsp_iterations, tsp_scores, 'g', label='TSP')
		ax.plot(greedy_iterations, greedy_scores, 'r', label='Greedy')


		if(max(len(matrix), len(matrix[0])) <= 10):
			optimal_score = opt.optimal_2D(matrix)

			optimal_iterations = [i for i in range(max(len(greedy_scores), len(tsp_scores)))]
			optimal_scores = [optimal_score for i in range(len(optimal_iterations))]
			
			# Plot optimal solution
			ax.plot(optimal_iterations, optimal_scores, 'b', label='Optimal')



		legend = ax.legend(loc='upper center', shadow=True, fontsize='x-large')
		
		# Save plot
		plot_path = path[:-4] + "_plot.png"
		plt.savefig(plot_path)


		# Save solutions (if matrix is too big, optimal solution = -1.0)
		score_path = path[:-4] + "_scores.txt"
		f = open(score_path, "w+")
		f.write("{} {} {}".format(optimal_score, tsp_score, greedy_score))
		f.close()