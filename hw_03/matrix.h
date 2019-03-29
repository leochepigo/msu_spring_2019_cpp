#pragma once
#include <stdexcept>

class Row {
	size_t ncols;
	int* data;

public:
	Row(): ncols(0), data(nullptr) {};
	Row(size_t _ncols): ncols(_ncols), data(new int[_ncols]) {};
	~Row() { delete[] data;	};

	int& operator [] (size_t n);
	const int& operator [] (size_t n) const;
	const Row& operator = (const Row& row);
};

int& Row::operator[] (size_t n) {
	if (n < ncols) {
		return data[n];
	}
	else throw std::out_of_range("");
}

const int& Row::operator[] (size_t n) const {
	if (n < ncols) {
		return data[n];
	}
	else throw std::out_of_range("");
}

const Row& Row::operator = (const Row& row) {
	ncols = row.ncols;
	delete[] data;
	data = new int [row.ncols];
	for (size_t i = 0; i<row.ncols; ++i) data[i] = row.data[i];
	return *this;
}

class Matrix {
	const size_t nrows;
	const size_t ncols;
	Row* rows;

public:
		Matrix(size_t _nrows, size_t _ncols): nrows(_nrows), ncols(_ncols), rows(new Row[_nrows]) {
			for (size_t i =0; i<_nrows; ++i){
				rows[i] = Row(_ncols);
			}
		};
		~Matrix() {	delete[] rows; }

		Row& operator[] (size_t n);
		const Row& operator[] (size_t n) const;
		Matrix& operator *= (int n);
		bool operator == (const Matrix& m) const;
		bool operator != (const Matrix& m) const;
		size_t getRows() const { return nrows; }
		size_t getColumns() const { return ncols; }
};

Row& Matrix::operator[] (size_t n) {
	if (n < nrows) {
		return rows[n];
	}
	else throw std::out_of_range("");
}

const Row& Matrix::operator[] (size_t n) const {
	if (n < nrows) {
		return rows[n];
	}
	else throw std::out_of_range("");
}

Matrix& Matrix::operator *= (int n) {
	for (size_t i = 0; i < nrows; ++i) {
		for (size_t j = 0; j < ncols; ++j) {
			rows[i][j] *= n;
		}
	}
	return *this;
}

bool Matrix::operator == (const Matrix& m) const {
	if (ncols == m.ncols && nrows == m.nrows) {
		for (size_t i = 0; i < nrows; ++i) {
			for (size_t j = 0; j < ncols; ++j) {
				if (rows[i][j] != m[i][j]) return false;
			}
		}
	}
	else return false;
	return true;
}

bool Matrix::operator != (const Matrix& m) const {
	return !(*this==m);
}
