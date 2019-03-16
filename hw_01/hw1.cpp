#include <stdlib.h>
#include <iostream>
#include <cmath>

using namespace std;

int isPrime(int n) {
	if (n==1) {
		return false;
	}

	for (int i = 2; i<=sqrt(n); ++i) {
		if (n%i==0) return false;
	}

	return true;
}

int how_much_prime(int left, int right, const int* Data, int Size) {
	int bgn, end, count;
	bgn = end = count = 0;

	while((Data[bgn] < left) && (bgn < Size)) ++bgn;
	while((Data[end] < right)&& (bgn < Size)) ++end;

	if (Data[bgn]!=left || Data[end] != right || bgn == Size || end ==Size) {
		return 0;
	}

	for (; bgn <= end; ++bgn) {
		count+= isPrime(Data[bgn]);
	}

	return count;
}

int main(int argc, char* argv[]) {
	#include "numbers.dat"

	if (!(argc % 2) || (argc == 1)) {
			return -1;
	}

	for (int i = 1; i<argc; ++i) {
		int left = atoi(argv[i]);
		++i;
		int right = atoi(argv[i]);
		cout << how_much_prime(left,right,Data,Size) << endl;
	}

	return 0;
}