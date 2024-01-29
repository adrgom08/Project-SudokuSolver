// Adrian Gomez Lamuedra 

// Sequential solution for the problem

#include <iostream>
#include <vector>
#include <unordered_set>
#include <chrono>

using namespace std;
using tMatrix = vector<vector<int>>;

const int N = 9;

bool rowVal(tMatrix& const m) { // Checks that every row has the numbers 1 to 9
	unordered_set<int> s;
	for (int i = 0; i < N; i++) {
		s.clear(); // Numbers can be repeated but in another row
		for (int j = 0; j < N; j++) {
			int k = m[i][j];
			if (k < 1 || s.count(k) > 0 || k > 9) return false; // There is a number repeated
			else s.insert(k); // No rule is violated, we continue the search
		}
	}
	return true;
}

bool colVal(tMatrix& const m) { // Checks that every column has the numbers 1 to 9
	unordered_set<int> s;
	for (int i = 0; i < N; i++) {
		s.clear(); // Numbers can be repeated but in another column
		for (int j = 0; j < N; j++) {
			int k = m[j][i];
			if (k < 1 || s.count(k) > 0 || k > 9) return false; // There is a number repeated
			else s.insert(k); // No rule is violated, we continue the search
		}
	}
	return true;
}

bool boxVal(tMatrix& const m, int x, int y) { // Checks that every small box has the 9 numbers 
	unordered_set<int> s;
	for (int i = x; i < x + 3; i++) {
		for (int j = y; j < y + 3; j++) {
			int k = m[i][j];
			if (k < 1 || s.count(k) > 0 || k > 9) return false; // There is a number repeated
			else s.insert(k); // No rule is violated, we continue the search
		}
	}
	return true;
}


bool validation(tMatrix& const m) { // Checks that every condition for the sudoku to be valid is true
	if (!rowVal(m)) return false;
	if (!colVal(m)) return false;
	for (int i = 0; i < N; i = i + 3) {
		for (int j = 0; j < N; j = j + 3) { // We have to check for the 9 boxes 
			if (!boxVal(m, i, j)) return false;
		}
	}
	return true;
}

void fillSudoku(tMatrix& m) { // Receives a full matrix 9x9 to be checked
	int k = 0;
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			cin >> k;
			m[i][j] = k;
		}
	}
}

int main() { // Program for 
	tMatrix m(N, vector<int>(N));
	fillSudoku(m);

	auto timeStart = chrono::high_resolution_clock::now();

	bool val = validation(m);

	auto timeEnd = chrono::high_resolution_clock::now();
	chrono::duration<double, std::milli> elapsed = timeEnd - timeStart;
	cout << "\nElapsed time: " << elapsed.count() << " ms\n";

	if (val) cout << "The sudoku is correct\n";
	else cout << "The sudoku is incorrect\n";

	return 0;
}