#include <vector>
#include <string>
#include <fstream>
#include <random>
#include <iostream>
#include "matrix.h"

using namespace std;

mt19937 gen(time(NULL));

long double my_rand(){
	long long r = gen() % 100000001;
	long double ret = 1.0 * r / 100000000.0;
	return ret;
}


vector<vector<long double>> generate_binary_matrix(int n, int m){
	vector<vector<long double>> matrix;
	matrix.resize(n);
	for(int i = 0; i < n; i++){
		matrix[i].resize(m);
		for(int j = 0; j < m; j++){
			matrix[i][j] = gen() % 2;
		}
	}

	return matrix;
}


vector<vector<long double>> generate_random_matrix(int n, int m){
	vector<vector<long double>> matrix;
	matrix.resize(n);
	for(int i = 0; i < n; i++){
		matrix[i].resize(m);
		for(int j = 0; j < m; j++){
			matrix[i][j] = my_rand();
		}
	}

	return matrix;
}


vector<vector<long double>> read_matrix(){
	int n, m;
	cin >> n >> m;
	
	vector<vector<long double>> matrix;
	matrix.resize(n);
	for(int i = 0; i < n; i++){
		matrix[i].resize(m);
		for(int j = 0; j < m; j++){
			cin >> matrix[i][j];
		}
	}

	return matrix;
}


vector<vector<long double>> read_matrix_file(string path){
	ifstream input(path);
	int n, m;
	input >> n >> m;
	
	vector<vector<long double>> matrix;
	matrix.resize(n);
	for(int i = 0; i < n; i++){
		matrix[i].resize(m);
		for(int j = 0; j < m; j++){
			input >> matrix[i][j];
		}
	}
	input.close();

	return matrix;
}


void output_matrix(vector<vector<long double>> &matrix){
	int n = matrix.size();
	int m = matrix[0].size();
	cout << n << " " << m << "\n";

	for(int i = 0; i < n; i++){
		for(int j = 0; j < m; j++){
			cout << matrix[i][j] << " ";
		}
		cout << "\n";
	}
}


void output_matrix_file(string path, vector<vector<long double>> &matrix){
	ofstream output(path);
	int n = matrix.size();
	int m = matrix[0].size();
	output << n << " " << m << "\n";

	for(int i = 0; i < n; i++){
		for(int j = 0; j < m; j++){
			output << matrix[i][j] << " ";
		}
		output << "\n";
	}

	output.flush();
	output.close();
}


void print_matrix(vector<vector<long double>> &matrix){
	for(int i = 0; i < matrix.size(); i++){
		for(int j = 0; j < matrix[i].size(); j++){
			cout << matrix[i][j] << " ";
		}
		cout << "\n";
	}
}


vector<vector<long double>> apply_permutation_columns(vector<vector<long double>> &matrix, vector<int> &permutation){
	int n = matrix.size();
	int m = matrix[0].size();
	vector<vector<long double>> new_matrix = matrix;
	for(int j = 0; j < m; j++){
		for(int i = 0; i < n; i++){
			new_matrix[i][j] = matrix[i][permutation[j]];
		}
	}
	return new_matrix;
}


vector<vector<long double>> apply_permutation_rows(vector<vector<long double>> &matrix, vector<int> &permutation){
	int n = matrix.size();
	int m = matrix[0].size();
	vector<vector<long double>> new_matrix = matrix;
	for(int i = 0; i < n; i++){
		for(int j = 0; j < m; j++){
			new_matrix[i][j] = matrix[permutation[i]][j];
		}
	}
	return new_matrix;
}