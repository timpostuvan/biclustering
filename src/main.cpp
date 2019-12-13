#include <vector>
#include <iostream>
#include <iomanip>
#include "matrix.h"
#include "cost_functions.h"
#include "fixed_rows.h"

using namespace std;


int main(){
	vector<vector<long double>> matrix = generate_random_matrix(100, 1000);
//	output_matrix(matrix);

	cout << endl << "Lower bound 1 closest: " << lower_bound_1_closest(matrix, CostFunctions::variance22) << endl;

	cout << endl << "Lower bound 2 closest: " << lower_bound_2_closest(matrix, CostFunctions::variance22) << endl;
	
	cout << endl << "Lower bound spanning tree: " << lower_bound_spanning_tree(matrix, CostFunctions::variance22) << endl;

	cout << endl << "Initial cost: " << CostFunctions::variance22(matrix) << endl;
	
	cout << endl << "Random permutations: " << endl;
	random_permutations(100000, matrix, CostFunctions::variance22);

	cout << endl << "Random greedy path two ends: " << endl;
	random_greedy_path_two_ends(1, matrix, CostFunctions::variance22);

	cout << endl << "Spanning tree: " << endl;
	spanning_tree(matrix, CostFunctions::variance22); 

	cout << endl << "2-opt heuristics: " << endl;
	tsp_2_opt(100000, 0.1, matrix, CostFunctions::variance22);

/*	cout << endl << "All permutations " << endl;
	all_permutations(matrix, CostFunctions::variance22); 
*/

 /*
	int cnt1 = 0;
	int cnt2 = 0;

	while(cnt1 + cnt2 < 50){
		vector<vector<long double>> matrix = generate_binary_matrix(200, 200);
		
		long double ans1 = random_greedy_path_two_ends(1, matrix, CostFunctions::variance22).cost;
		long double ans2 = spanning_tree(matrix, CostFunctions::variance22).cost;

		cout << cnt1 + cnt2 << endl;
		if(ans1 - ans2 > 1e-3){
			cnt2++;
		}
		if(ans2 - ans1 > 1e-3){
			cnt1++;
			cout << fixed << setprecision(5) << ans1 << " " << ans2 << "\n";
	//		cout << "Matrix:\n";
	//		print_matrix(matrix);
		}
	}
	cout << 1.0 * cnt1 / (cnt1 + cnt2) << "   " << 1.0 * cnt2 / (cnt1 + cnt2)  << "\n" << cnt1 << "  " << cnt2 << endl; 

	*/



	return 0; 
}