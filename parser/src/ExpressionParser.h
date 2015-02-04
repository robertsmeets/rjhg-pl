/*
 * ExpressionParser.h
 *
 *  Created on: Dec 18, 2014
 *      Author: robert
 */

#ifndef EXPRESSIONPARSER_H_
#define EXPRESSIONPARSER_H_

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <list>
#include <stack>
#include <map>
#include <string>
#include <vector>
#include <iterator>

#include "ExpressionNode.h"
#include "ExpressionThing.h"
#include "PException.h"

using namespace std;

const int LEFT_ASSOC = 0;
const int RIGHT_ASSOC = 1;
//
// Map the different operators: +, -, *, / etc
//
typedef map<string, pair<int, int> > OpMap;
typedef vector<string>::const_iterator cv_iter;
typedef string::iterator s_iter;

const OpMap::value_type assocs[] = { OpMap::value_type("+",
		make_pair<int, int>(0, (int) LEFT_ASSOC)), OpMap::value_type("-",
		make_pair<int, int>(0, (int) LEFT_ASSOC)), OpMap::value_type("*",
		make_pair<int, int>(5, (int) LEFT_ASSOC)), OpMap::value_type("/",
		make_pair<int, int>(5, (int) LEFT_ASSOC)) };

const OpMap opmap(assocs, assocs + sizeof(assocs) / sizeof(assocs[0]));

class ExpressionParser {

public:
	ExpressionParser();
	virtual ~ExpressionParser();

	bool isParenthesis(const string&);

	// Test if token is an operator
	bool isOperator(const string&);

	bool isInt(const string&);
	bool isFloat(const string&);
	bool isString(const string&);
	bool isBool(const string&);

	bool isComma(const string&);

	bool isFunction(const string&);

	// Test associativity of operator token
	bool isAssociative(const string&, const int&);

	// Compare precedence of operators.
	int cmpPrecedence(const string&, const string&);
	// Convert infix expression format into reverse Polish notation
	bool infixToRPN(const vector<string>&, const int&,
			vector<ExpressionThing>&);

	double RPNtoDouble(vector<string> tokens);
	vector<string> getExpressionTokens(const string& expression);

	// Print iterators in a generic way
	template<typename T, typename InputIterator> void Print(
			const string& message, const InputIterator& itbegin,
			const InputIterator& itend, const string& delimiter);
	ExpressionNode* parse(string);
	unsigned int figureType(string token);

};

#endif /* EXPRESSIONPARSER_H_ */
