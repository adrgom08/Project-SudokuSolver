// Adrian Gomez Lamuedra

// Sudoku solver after optimization 1

#include <iostream>
#include <vector>
#include <unordered_set>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;
using tMatrix = vector<vector<int>>;

// Global variables

bool wrong = false;
mutex mu;

const int N = 9;

void rowVal(tMatrix& const m) { // Checks that every row has the numbers 1 to 9
	unordered_set<int> s;
	int i = 0, j = 0;
	while (i < N && !wrong) {
		s.clear(); // Numbers can be repeated but in another row
		j = 0;
		while (j < N && !wrong) { // We check at every moment this thread or the others have found a mistake
			int k = m[i][j];
			if (k < 1 || s.count(k) > 0 || k > 9) {
				mu.lock(); // Acquire the lock to change the global variable
				wrong = true; // There is a number repeated
				mu.unlock(); // Release the lock just after the variable is modified
			}
			else s.insert(k); // No rule is violated, we continue the search
			++j;
		}
		++i;
	}
}


void colVal(tMatrix& const m) { // Checks that every column has the numbers 1 to 9
	unordered_set<int> s;
	int i = 0, j = 0;
	while (i < N && !wrong) {
		s.clear(); // Numbers can be repeated but in another column
		j = 0;
		while (j < N && !wrong) { // We check at every moment this thread or the others have found a mistake
			int k = m[j][i];
			if (k < 1 || s.count(k) > 0 || k > 9) {
				mu.lock(); // Acquire the lock to change the global variable
				wrong = true; // There is a number repeated
				mu.unlock(); // Release the lock just after the variable is modified
			}
			else s.insert(k); // No rule is violated, we continue the search
			++j;
		}
		++i;
	}
}

void boxVal(tMatrix& const m, int x, int y) { // Checks that every small box has the 9 numbers 
	unordered_set<int> s;
	int i = x, j = y;
	while (i < x + 3 && !wrong) { // We check at every moment this thread or the others have found a mistake
		j = y;
		while (j < y + 3 && !wrong) {
			int k = m[i][j];
			if (k < 1 || s.count(k) > 0 || k > 9) {
				mu.lock(); // Acquire the lock to change the global variable
				wrong = true; // There is a number repeated
				mu.unlock(); // Release the lock just after the variable is modified
			}
			else s.insert(k); // No rule is violated, we continue the search
			++j;
		}
		++i;
	}
}

void genBoxVal(tMatrix& const m) { // Checks that the 9 boxes have numbers from 1 to 9
	int i = 0, j = 0;
	while (i < N && !wrong) {
		j = 0;
		while (j < N && !wrong) {
			boxVal(m, i, j);
			j += 3;
		}
		i += 3;
	}
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

int main() {
	tMatrix m(N, vector<int>(N));
	fillSudoku(m);

	auto timeStart = chrono::high_resolution_clock::now();

	thread first(rowVal, ref(m)); // Pass the reference of m and not m itself (the value)
	thread second(colVal, ref(m));
	thread third(genBoxVal, ref(m));

	first.join();
	second.join();
	third.join();

	auto timeEnd = chrono::high_resolution_clock::now();
	chrono::duration<double, std::milli> elapsed = timeEnd - timeStart;
	cout << "\nElapsed time: " << elapsed.count() << " ms\n";

	if (!wrong) cout << "The sudoku is correct\n";
	else cout << "The sudoku is incorrect\n";

	return 0;
}