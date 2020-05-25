#include "Coarse_Grained.h"
#include "Fine_Grained.h"
#include "Lock_free.h"
#include "Lock_free_mem.h"
#include "Optimistic.h"
#include "Optimistic_mem.h"
#include <omp.h>
//#include <stdint.h>
#include "stdio.h"
#include "termcolor.hpp"
#include <chrono>
#include <fstream>
#include <sstream>
#include <vector>
using namespace termcolor;
#include <set>

void runtest(string name, SetList<int> *list);
void check(string name, SetList<int> *list);
int main(int argc, char *argv[]) {

	SetList<int> *list;

	// /////////////////// CoarseList///////////////////////
	// cout << endl << endl << "CoarseList:" << endl;

	// list = new CoarseList<int>();
	// runtest("testcases/basic.txt", list);
	// runtest("testcases/remove.txt", list);

	// delete list;
	// list = new CoarseList<int>();
	// runtest("testcases/pre.txt", list);
	// runtest("testcases/main.txt", list);
	// check("testcases/main.txt", list);
	// delete list;

	// ////////////////////// FineList /////////////////////
	// cout << endl << endl << "FineList:" << endl;

	// list = new FineList<int>();
	// runtest("testcases/basic.txt", list);
	// runtest("testcases/remove.txt", list);
	// delete list;

	// list = new FineList<int>();
	// runtest("testcases/pre.txt", list);
	// runtest("testcases/main.txt", list);
	// check("testcases/main.txt", list);
	// delete list;

	// ///////////////////// Optimistic_mem /////////////////////
	// cout << endl << endl << "Optimistic_mem:" << endl;

	// 	list = new Optimistic_mem<int>();
	// 	runtest("testcases/basic.txt", list);
	// 	runtest("testcases/remove.txt", list);
	// 	delete list;
	// for(int i=0; i<100;i++){
	// list = new Optimistic_mem<int>();
	// runtest("testcases/pre.txt", list);
	// runtest("testcases/main.txt", list);
	// check("testcases/main.txt", list);
	// delete list;
	// }
	// ///////////////////// Optimistic /////////////////////
	// cout << endl << endl << "Optimistic:" << endl;

	// list = new Optimistic<int>();
	// runtest("testcases/basic.txt", list);
	// runtest("testcases/remove.txt", list);
	// delete list;

	// list = new Optimistic<int>();
	// runtest("testcases/pre.txt", list);
	// runtest("testcases/main.txt", list);
	// check("testcases/main.txt", list);
	// delete list;
	///////////////////// Lock Free /////////////////////
	cout << endl << endl << "Lock Free:" << endl;
	for (size_t i = 0; i < 200; i++) {

		list = new LockFree_mem<int>();
		runtest("testcases/basic.txt", list);
		runtest("testcases/remove.txt", list);
		delete list;

	}
		list = new LockFree_mem<int>();
		runtest("testcases/pre.txt", list);
		runtest("testcases/main.txt", list);
		check("testcases/main.txt", list);
		delete list;
	return 0;
}

void runtest(string name, SetList<int> *list) {
	ifstream file(name);
	string line;
	vector<vector<int>> cases;
	set<int> valid;

	// Load Test cases
	if (file.is_open()) {
		while (getline(file, line)) {
			vector<int> x;
			string segment;
			stringstream l(line);

			while (getline(l, segment, ' ')) {
				auto t = stoi(segment);
				x.push_back(t);

				if (t < 0)
					valid.erase(-t);
				else if (t > 0)
					valid.insert(t);
				else
					throw new invalid_argument("0 not allowed");
			}

			cases.push_back(x);
		}

		file.close();
	} else {
		cout << "Unable to open file";
		return;
	}

	cout << endl;
	mutex outl;

	auto start = chrono::high_resolution_clock::now();

// Run test Cases
#pragma omp parallel
#pragma omp for
	for (auto it = cases.begin(); it < cases.end(); it++) {
		for (const auto &j : *it) {
			if (j < 0) {
				list->remove(-j);
				if (list->contains(j)) {
					cout << red << "Error: " << reset << j << " in list" << endl;
				}
			} else if (j > 0)
				list->add(j);

			else
				throw new invalid_argument("0 not allowed");
		}
		{
			// const lock_guard<mutex> lock(outl);
			// cout<<"Added "<<it->size()<<" by "<<cyan<<"Thread "<<omp_get_thread_num()<<reset<<endl;
		}
	}

	auto finish = chrono::high_resolution_clock::now();
	chrono::duration<double> elapsed = finish - start;
	auto ms = chrono::duration_cast<chrono::milliseconds>(elapsed).count();

	// Compare to Valid
	bool correct = true;
	for (const auto &i : valid) {
		if (!list->contains(i)) {
			cout << red << "Error: " << reset << i << " not in list" << endl;
			correct = false;
		}
	}

	if (correct) {
		cout << green << "Test succeeded" << reset << " (" << name << ")"
			 << " in " << ms << "ms" << endl;
	}
}

void check(string name, SetList<int> *list) {
	ifstream file(name);
	string line;
	set<int> valid;
	set<int> invalid;

	// Load Test cases
	if (file.is_open()) {
		while (getline(file, line)) {
			vector<int> x;
			string segment;
			stringstream l(line);

			while (getline(l, segment, ' ')) {
				auto t = stoi(segment);

				if (t < 0) {
					invalid.insert(t);
				} else if (t > 0)
					valid.insert(t);
				else
					throw new invalid_argument("0 not allowed");
			}
		}

		file.close();
	} else {
		cout << "Unable to open file";
		return;
	}

	cout << endl;
	mutex outl;

	auto start = chrono::high_resolution_clock::now();

	// Compare to Valid
	bool correct = true;
	for (const auto &i : valid) {
		if (!list->contains(i)) {
			cout << red << "Error: " << reset << i << " not in list" << endl;
			correct = false;
		}
	}

	for (const auto &i : invalid) {
		if (list->contains(i)) {
			cout << red << "Error: " << reset << i << " in list" << endl;
			correct = false;
		}
	}

	auto finish = chrono::high_resolution_clock::now();
	chrono::duration<double> elapsed = finish - start;
	auto ms = chrono::duration_cast<chrono::milliseconds>(elapsed).count();
	if (correct) {
		cout << green << "Test succeeded" << reset << " (" << name << ")"
			 << " in " << ms << "ms" << endl;
	}
}