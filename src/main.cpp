#include <vector>
#include <iostream>
#include "matrix.h"
#include "cost_functions.h"
#include "fixed_rows.cpp"

using namespace std;


int main(){
	vector<vector<long double>> matrix = generate_random_matrix(10, 10);
	output_matrix(matrix);

	cout << "Initial cost: " << CostFunctions::variance22(matrix) << endl;
	
	cout << endl << "Random permutations: " << endl;
	random_permutations(10000, matrix, CostFunctions::variance22);

	cout << endl << "Random greedy path two ends: " << endl;
	random_greedy_path_two_ends(1, matrix, CostFunctions::variance22);

	cout << endl << "Spanning tree: " << endl;
	spanning_tree(matrix, CostFunctions::variance22);

	cout << endl << "All permutations " << endl;
	all_permutations(matrix, CostFunctions::variance22);


	cout << endl << "Lower bound: " << lower_bound(matrix, CostFunctions::variance22) << endl;
	return 0; 
}