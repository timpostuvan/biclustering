#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include "matrix.h"
#include "cost_functions.h"
#include "fixed_rows.h"

using namespace std;


int main(int argc, char *argv[]){

	vector<vector<long double>> matrix = read_matrix_file("matrix.txt");

	string path = "";
	long double approximation = 0.0;

	if(argv[1][0] == '0'){	// lower_bound_spanning_tree
		approximation = lower_bound_spanning_tree(matrix, CostFunctions::variance22);
		path = "result_l_spanning_tree.txt";
	}
	if(argv[1][0] == '1'){	// lower_bound_2_closest
		approximation = lower_bound_2_closest(matrix, CostFunctions::variance22);
		path = "result_l_2_closest.txt";
	}
	if(argv[1][0] == '2'){	// lower_bound_1_closest
		approximation = lower_bound_1_closest(matrix, CostFunctions::variance22);
		path = "result_l_1_closest.txt";
	}
	if(argv[1][0] == '3'){	// spanning_tree
		approximation = spanning_tree(matrix, CostFunctions::variance22).cost;
		path = "result_u_spanning_tree.txt";
	}
	if(argv[1][0] == '4'){	// greedy_path
		approximation = random_greedy_path_two_ends(1, matrix, CostFunctions::variance22).cost;
		path = "result_u_greedy_path.txt";
	}
	if(argv[1][0] == '5'){	// (spanning_tree + lower_bound_2_closest) / 2
		approximation = spanning_tree(matrix, CostFunctions::variance22).cost;
		approximation += lower_bound_2_closest(matrix, CostFunctions::variance22);
		approximation /= 2.0;
		path = "result_combined.txt";
	}


	ofstream output(path);
	output << fixed << setprecision(4) << approximation << endl;
	output.close();

	return 0; 
}