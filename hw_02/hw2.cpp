#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <limits>
#define INF numeric_limits<int>::max()

using namespace std;

struct Expression {
	Expression(const string& token) : token(token) {}
	Expression(const string& token, const Expression& a) : token(token), args{a} {}
	Expression(const string& token, const Expression& a, const Expression& b) : token(token), args{a, b} {}

	string token;
	vector<Expression> args;
};

class Parser {
public:
	Parser(const char* smbl) : current_symbol(smbl) {}
	Expression parse();
private:
	string parse_token();
	Expression parse_simple_expression();
	Expression parse_binary_expression(int min_priority);

	const char* current_symbol;
};

string Parser::parse_token() {
	while (isspace(*current_symbol)) ++current_symbol;

	if (isdigit(*current_symbol)) {
		string number;
		while (isdigit(*current_symbol)) number.push_back(*current_symbol++);
		return number;
	}

	constexpr const char* tokens[] =	{ "+", "-", "*", "/"};
	for (auto& t : tokens) {
		if (strncmp(current_symbol, t, strlen(t)) == 0) {
			current_symbol += 1;
			return t;
		}
	}

	return "";
}

Expression Parser::parse_simple_expression() {
	string token = parse_token();
	if (token.empty())  {
		return to_string(INF);
	}

	if (isdigit(token[0]))
		return Expression(token);

	return Expression(token, parse_simple_expression());
}

int get_priority(const string& binary_op) {
	if (binary_op == "+") return 1;
	if (binary_op == "-") return 1;
	if (binary_op == "*") return 2;
	if (binary_op == "/") return 2;
	return 0;
}

Expression Parser::parse_binary_expression(int min_priority) {

	Expression left_expr = parse_simple_expression();

	for (;;) {
		string op = parse_token();
		int priority = get_priority(op);
		if (priority <= min_priority) {
			current_symbol -= op.size();
			return left_expr;
		}

		Expression right_expr = parse_binary_expression(priority);
		left_expr = Expression(op, left_expr, right_expr);
	}
	return {""};
}

Expression Parser::parse() {
	return parse_binary_expression(0);
}

int calculate(const Expression& e) {
	switch (e.args.size()) {
	case 2: {

		int a = calculate(e.args[0]);
		int b = calculate(e.args[1]);
		if (a!= INF && b != INF) {
		if (e.token == "+") return a + b;
		if (e.token == "-") return a - b;
		if (e.token == "*") return a * b;
		if (e.token == "/" && b != 0) return a / b;
		}
		return INF;
	}

	case 1: {
		int a = calculate(e.args[0]);
		if (e.token == "-") return -a;
		return INF;
	}

	case 0:
		return atoi(e.token.c_str());
	}


	return INF;
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
		cout << "error";
		return 1;
	}

	Parser p(argv[1]);
	int result = calculate(p.parse());
	if (result == INF) {
		cout << "error";
		return 1;
	}

	cout << result;
	return 0;

}
