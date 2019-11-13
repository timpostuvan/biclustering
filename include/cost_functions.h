#ifndef COST_FUNCTIONS
#define COST_FUNCTIONS

#include <vector>

using namespace std;

class CostFunctions{
public:	
	static long double multiply4(vector<vector<long double>> &matrix);		// množiš trenutno polje s seštevkom sosednjih 4, max
	
	static long double multiply8(vector<vector<long double>> &matrix);		// množiš trenutno polje s seštevkom sosednjih 8, max
	
	static long double squared4(vector<vector<long double>> &matrix);		// kvadriraš razliko in sešteješ sosednje 4, min
	
	static long double squared8(vector<vector<long double>> &matrix);		// kvadriraš razliko in sešteješ sosednjih 8, min
	
	static long double variance22(vector<vector<long double>> &matrix);		// varianca za 2 * 2 matrike, min
};

#endif