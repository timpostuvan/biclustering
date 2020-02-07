#include <vector>
#include <iostream>
#include <unistd.h>
#include <iomanip>
#include "matrix.h"
#include "cost_functions.h"
#include "fixed_rows.h"

using namespace std;

int main(int argc, char *argv[]){

	vector<vector<long double>> matrix;
	if(argv[1][0] == '0'){	// random matrix [-1, 1]
		matrix = generate_random_matrix(100, 100, true);
	}

	if(argv[1][0] == '1'){	// random positive matrix [0, 1]
		matrix = generate_random_matrix(100, 100, false);
	}

	if(argv[1][0] == '2'){	// random binary matrix
		matrix = generate_binary_matrix(100, 100);
	}

	if(argv[1][0] == '3'){	// random matrix within image
		sleep(1);
	//	string image_path = "images-data/flower_matrix.txt";
		string image_path = "images-data/caribbean_matrix.txt";
		matrix = read_matrix_file(image_path);
		matrix = apply_random_permutation_rows(matrix);
		matrix = apply_random_permutation_columns(matrix);
	}

	if(argv[1][0] == '4'){	// biological data
		sleep(1);
//		string data_path = "biology-data/brown-selected/brown_selected.txt";
//		string data_path = "biology-data/gastroparesis/gastroparesis_smaller.txt";
		string data_path = "biology-data/radiation/radiation.csv";
		matrix = read_matrix_file(data_path);
		matrix = apply_random_permutation_rows(matrix);
		matrix = apply_random_permutation_columns(matrix);
	}

	output_matrix_file("matrix.txt", matrix);

	vector<vector<long double>> distance_matrix = distance_matrix_columns(matrix, CostFunctions::variance22);
	output_matrix_file("distance_matrix.txt", distance_matrix);

	return 0; 
}