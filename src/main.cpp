#include <vector>
#include <iostream>
#include "matrix.h"
#include "cost_functions.h"
#include "fixed_rows.cpp"

using namespace std;


int main(){
	vector<vector<long double>> matrix = generate_binary_matrix(6, 6);
	output_matrix(matrix);

	cout << CostFunctions::variance22(matrix) << "\n";
	all_permutations(matrix, CostFunctions::variance22);
	random_permutations(2000, matrix, CostFunctions::variance22);

	return 0; 
}