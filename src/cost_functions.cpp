#include <vector>
#include <cstdlib>
#include "cost_functions.h"

using namespace std;

long double CostFunctions::multiply4(vector<vector<long double>> &matrix){
	int n = matrix.size();
	int m = matrix[0].size();

	long double ret = 0.0;
	for(int i = 0; i < n; i++){
		for(int j = 0; j < m; j++){
			for(int k = -1; k <= 1; k++){
				for(int l = -1; l <= 1; l++){
					int ni = i + k;
					int nj = j + l;

					if(abs(k) + abs(l) == 1 && ni >= 0 && ni < n && nj >= 0 && nj < m){
						ret += (matrix[i][j] * matrix[ni][nj]);
					}
				}
			}
		}
	}

	return ret;
}


long double CostFunctions::multiply8(vector<vector<long double>> &matrix){
	int n = matrix.size();
	int m = matrix[0].size();

	long double ret = 0.0;
	for(int i = 0; i < n; i++){
		for(int j = 0; j < m; j++){
			for(int k = -1; k <= 1; k++){
				for(int l = -1; l <= 1; l++){
					int ni = i + k;
					int nj = j + l;

					if(ni >= 0 && ni < n && nj >= 0 && nj < m){
						ret += (matrix[i][j] * matrix[ni][nj]);
					}
				}
			}
		}
	}
	
	return ret;
}


long double CostFunctions::squared4(vector<vector<long double>> &matrix){
	int n = matrix.size();
	int m = matrix[0].size();

	long double ret = 0.0;
	for(int i = 0; i < n; i++){
		for(int j = 0; j < m; j++){
			for(int k = -1; k <= 1; k++){
				for(int l = -1; l <= 1; l++){
					int ni = i + k;
					int nj = j + l;

					if(abs(k) + abs(l) == 1 && ni >= 0 && ni < n && nj >= 0 && nj < m){
						ret += (matrix[i][j] - matrix[ni][nj]) * (matrix[i][j] - matrix[ni][nj]);
					}
				}
			}
		}
	}

	return ret;
}


long double CostFunctions::squared8(vector<vector<long double>> &matrix){
	int n = matrix.size();
	int m = matrix[0].size();

	long double ret = 0.0;
	for(int i = 0; i < n; i++){
		for(int j = 0; j < m; j++){
			for(int k = -1; k <= 1; k++){
				for(int l = -1; l <= 1; l++){
					int ni = i + k;
					int nj = j + l;

					if(ni >= 0 && ni < n && nj >= 0 && nj < m){
						ret += (matrix[i][j] - matrix[ni][nj]) * (matrix[i][j] - matrix[ni][nj]);
					}
				}
			}
		}
	}
	
	return ret;
}


long double CostFunctions::variance22(vector<vector<long double>> &matrix){
	int n = matrix.size();
	int m = matrix[0].size();

	long double ret = 0.0;
	for(int i = 0; i < n - 1; i++){
		for(int j = 0; j < m - 1; j++){
			long double mean = 0.0;
			for(int k = 0; k <= 1; k++){
				for(int l = 0; l <= 1; l++){
					int ni = i + k;
					int nj = j + l;

					if(ni >= 0 && ni < n && nj >= 0 && nj < m){
						mean += matrix[ni][nj];
					}
				}
			}

			mean = mean / 4.0;

			for(int k = 0; k <= 1; k++){
				for(int l = 0; l <= 1; l++){
					int ni = i + k;
					int nj = j + l;

					if(ni >= 0 && ni < n && nj >= 0 && nj < m){
						ret += (matrix[ni][nj] - mean) * (matrix[ni][nj] - mean);
					}
				}
			}
		}
	}
	
	return ret;
}