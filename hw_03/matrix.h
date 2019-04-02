#pragma once
#include <stdexcept>

class Row {
	size_t ncols;
	int* data;
public:
	Row(int* _data, size_t _ncols): ncols(_ncols), data(_data) {};

	int& operator[] (size_t n);
	int operator[] (size_t n) const;
};

int& Row::operator[] (size_t n) {
	if (n < ncols) {
		return data[n];
	}
	else throw std::out_of_range("");
}

int Row::operator[] (size_t n) const {
	if (n < ncols) {
		return data[n];
	}
	else throw std::out_of_range("");
}

class Matrix {
	size_t nrows;
	size_t ncols;
	int* data;

public:
	Matrix(size_t _nrows, size_t _ncols): nrows(_nrows), ncols(_ncols), data(new int[_nrows*_ncols]) { };
	~Matrix() {	delete[] data; };

	Row operator[] (size_t n);
	const Row operator[] (size_t n) const;
	Matrix& operator *= (int n);
	bool operator == (const Matrix& m) const;
	bool operator != (const Matrix& m) const;
	size_t getRows() const { return nrows; };
	size_t getColumns() const { return ncols; };
};

Row Matrix::operator[] (size_t n) {
	if (n < nrows) {
		return Row(data + n*ncols, ncols);
	}
	else throw std::out_of_range("");
}

const Row Matrix::operator[] (size_t n) const {
	if (n < nrows) {
		return Row(data+n*ncols, ncols);
	}
	else throw std::out_of_range("");
}

Matrix& Matrix::operator *= (int n) {
	for (size_t i = 0; i < nrows*ncols; ++i) {
		data[i]*=n;
	}
	return *this;
}

bool Matrix::operator == (const Matrix& m) const {
	if (ncols == m.ncols && nrows == m.nrows) {
		for (size_t i = 0; i < nrows*ncols; ++i) {
			if (data[i] != m.data[i]) return false;
		}
	}
	else return false;
	return true;
}

bool Matrix::operator != (const Matrix& m) const {
	return !(*this==m);
}
