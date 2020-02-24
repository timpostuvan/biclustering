#include <vector>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include "matrix.h"
#include "cost_functions.h"
#include "fixed_rows.h"
#include "solution.h"

using namespace std;


int main(int argc, char* argv[]){
	int n = atoi(argv[1]);
	int m = atoi(argv[2]);


	vector<vector<long double>> matrix = generate_random_matrix(n, m, 1);
	output_matrix_file("matrix.txt", matrix);

	matrix = read_matrix_file("matrix.txt");
	n = matrix.size();
	m = matrix[0].size();

	output_matrix(matrix);

	vector<int> rows, columns;
	for(int i = 0; i < n; i++){
		rows.push_back(i);
	}
	for(int i = 0; i < m; i++){
		columns.push_back(i);
	}

	long double best_score = 1.0 * (1LL << 40);
	do{
		do{
			vector<vector<long double>> new_matrix = matrix;
			new_matrix = apply_permutation_rows(new_matrix, rows);
			new_matrix = apply_permutation_columns(new_matrix, columns);

			long double new_score = CostFunctions::variance22(new_matrix);
			best_score = min(best_score, new_score);

		} while(next_permutation(columns.begin(), columns.end()));
	} while(next_permutation(rows.begin(), rows.end()));

	cout << best_score << endl;
	return 0; 
}