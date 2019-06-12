#include <thread>
#include <mutex>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>
#include <algorithm>
#include <iostream>

using namespace std;

constexpr uint64_t SIZE = 1000000;
mutex m;

void merge(string& prefix, int count) {
	for (int i = 2; i <= count; ++i) {
		string f1_name = prefix+'1';
		string f2_name = prefix+to_string(i);
		string tmp_name = prefix+'t';

		ifstream f1(f1_name, ios::binary);
		ifstream f2(f2_name, ios::binary);
		ofstream tmp(tmp_name, ios::binary);

		uint64_t n1, n2;

		f1.read((char*)(&n1), sizeof(n1));
		f2.read((char*)(&n2), sizeof(n2));

		while(!f1.eof() && !f2.eof()) {
			if (n1 < n2) {
				tmp.write((char*)(&n1), sizeof(n1));
				f1.read((char*)(&n1), sizeof(n1));
			}
			else {
				tmp.write((char*)(&n2), sizeof(n2));
				f2.read((char*)(&n2), sizeof(n2));
			}
		}

		if (f1.eof()) {
			while (!f2.eof()) {
				tmp.write((char*)(&n2), sizeof(n2));
				f2.read((char*)(&n2), sizeof(n2));
			}
		}
		else {
			while (!f1.eof()) {
				tmp.write((char*)(&n1), sizeof(n1));
				f1.read((char*)(&n1), sizeof(n1));
			}
		}

		f1.close();
		f2.close();
		tmp.close();
		remove(f1_name.c_str());
		remove(f2_name.c_str());
		rename(tmp_name.c_str(), f1_name.c_str());
	}
}

void merge_sort(ifstream &file, int thread_id) {
	int count = 0;
	string prefix = to_string(thread_id) + '_';

	while(!file.eof()) {
		m.lock();
		uint64_t x;
		vector<uint64_t> v;

		for(size_t i = 0; i < SIZE; ++i) {
			file.read((char*)(&x), sizeof(x));
			if(file.eof()) {
				break;
			}
			v.push_back(x);
		}

		if (v.empty()) {
			break;
		}

		sort(v.begin(), v.end());
		++count;
		ofstream out(prefix + to_string(count), ios::binary);
		out.write((char*)v.data(), v.size() * sizeof(uint64_t));

		m.unlock();
		this_thread::yield();
	}
	merge(prefix, count);
}

int main() {
	ifstream fin("input", ios::binary);
	thread th_1(merge_sort, ref(fin), 1);
	thread th_2(merge_sort, ref(fin), 2);

	th_1.join();
	th_2.join();

	fin.close();

	rename("2_1", "1_2");
	string prefix = "1_";
	merge(prefix,2);

	return 0;
}

