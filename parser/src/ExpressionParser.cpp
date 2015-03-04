/*
 * ExpressionParser.cpp
 *
 *  Created on: Dec 18, 2014
 *      Author: robert
 */

#include "ExpressionParser.h"

using namespace std;

//
// shunting yard algorithm to turn expressions into RPN
//
ExpressionParser::ExpressionParser() {

}

ExpressionParser::~ExpressionParser() {

}
//
// Test if token is a parenthesis
//
bool ExpressionParser::isParenthesis(const string& token) {
	return token == "(" || token == ")";
}

//
// Test if token is an operator
//
bool ExpressionParser::isOperator(const string& token) {
	return token == "+" || token == "-" || token == "*" || token == "/"
			|| token == "<" || token == "<=" || token == "==" || token == ">"
			|| token == ">=" || token == "!=";
}

//
// Test if token is a function
//
bool ExpressionParser::isFunction(const string& token) {
	unsigned int len = token.size();
	if (len == 0) {
		return false;
	}
	return (token[len - 1] == '(');
}

//
// Test if token is an integer
//
bool ExpressionParser::isInt(const string& token) {
	for (unsigned int i = 0; i < token.length(); i++) //for each char in string,
			{
		if (!(token[i] >= '0' && token[i] <= '9')) {
			return false;
		}
	}
	return true;

}
//
// Test if token is a float
//
bool ExpressionParser::isFloat(const string& token) {
	for (unsigned int i = 0; i < token.length(); i++) //for each char in string,
			{
		if (!((token[i] >= '0' && token[i] <= '9') || token[i] == '.'
				|| (token[i] == 'e' && i>0) || token[i] == '-')) {
			return false;
		}
	}
	return true;
}

//
// Test if token is a string
//
bool ExpressionParser::isString(const string& token) {
	unsigned int sz = token.length();
	if (sz < 2) {
		return false;
	}
	return ((token[0] == '"') && token[sz - 1] == '"');
}

//
// Test if token is a boolean
//
bool ExpressionParser::isBool(const string& token) {
	return ((token == "true") || (token == "false"));
}

//
// Test if token is a comma
//
bool ExpressionParser::isComma(const string& token) {
	return token == ",";
}

bool ExpressionParser::isStartToken(char a_char) {
	return a_char == '+' || a_char == '-' || a_char == '*' || a_char == '/'
			|| a_char == '<' || a_char == '=' || a_char == '>' || a_char == '!';

}

//
// Test associativity of operator token
//
bool ExpressionParser::isAssociative(const string& token, const int& type) {
	const pair<int, int> p = opmap.find(token)->second;
	return p.second == type;
}

//
// Compare precedence of operators
//
int ExpressionParser::cmpPrecedence(const string& token1,
		const string& token2) {
	const pair<int, int> p1 = opmap.find(token1)->second;
	const pair<int, int> p2 = opmap.find(token2)->second;
	return p1.first - p2.first;
}

//
// Convert infix expression format into reverse Polish notation
// using the shunting yard algorithm
//
bool ExpressionParser::infixToRPN(const vector<string>& inputTokens,
		const int& size, vector<ExpressionThing>& strArray) {
	bool success = true;
	list<ExpressionThing> out;
	stack<bool> were_values;
	stack<unsigned int> arg_count;
	stack<string> stack;
	//
	// While there are tokens to be read
	//
	for (int i = 0; i < size; i++) {
		//
		// Read the token
		//
		const string token = inputTokens[i];
#ifdef DEBUG
		cout << "examining token [" << token << "]" << endl;
#endif
		//
		// If token is an operator
		//
		if (isOperator(token)) {
			//
			// While there is an operator token, o2, at the top of the stack AND
			// either o1 is left-associative AND its precedence is equal to that of o2,
			// OR o1 has precedence less than that of o2,
			//
			const string o1 = token;
			if (!stack.empty()) {
				string o2 = stack.top();
				while (isOperator(o2)
						&& ((isAssociative(o1, LEFT_ASSOC)
								&& cmpPrecedence(o1, o2) == 0)
								|| (cmpPrecedence(o1, o2) < 0))) {
					//
					// pop o2 off the stack, onto the output queue;
					//
					stack.pop();
					//
					// type 1
					//
					ExpressionThing et(1, o2);
					out.push_back(et);
					if (!stack.empty())
						o2 = stack.top();
					else
						break;
				}
			}
			//
			// push o1 onto the stack.
			//
			stack.push(o1);
		}
		//
		// If the token is a left parenthesis, then push it onto the stack.
		//
		else if (token == "(") {
			//
			// Push token to top of the stack
			//
			stack.push(token);
		} else if (token == ")") {
			//
			// If the token is a right parenthesis:
			//
			// Until the token at the top of the stack is a left parenthesis, pop operators off
			// the stack onto the output queue.
			// Pop the left parenthesis from the stack, but not onto the output queue.
			//
			// Until the token at the top of the stack is a left parenthesis,
			// pop operators off the stack onto the output queue.
			//
			if (stack.empty())
			{
				break;
			}
			string topToken =  stack.top();
			while (topToken != "(") {
				//
				// type 1: operator ----- must evaluate what this is
				//
				// 1: operation
				// 2: literal integer
				// 3: variable name
				// 4: procedure call
				// 5: float
				// 6: boolean
				// 7: string
				//
				unsigned int atype = figureType(topToken);
				ExpressionThing et(atype, topToken);
				out.push_back(et);
				stack.pop();
				if (stack.empty()) {
					break;
				}
				topToken = stack.top();
			}
			//
			// Pop the left parenthesis from the stack, but not onto the output queue.
			//
			if (!stack.empty()) {
				stack.pop();
			}
			//
			//	If the token at the top of the stack is a function token
			//	Pop stack into f
			//	Pop arg count into a
			//	Pop were values into w
			//	If w is true, increment a
			//	Set the argument count of f to a
			//	Push f onto output queue
			//
			if (!stack.empty() && isFunction(stack.top())) {
				string f = stack.top();
				if (!stack.empty()) {
					stack.pop();
				}
				unsigned int a = arg_count.top();
				arg_count.pop();
				bool w = were_values.top();
				were_values.pop();
				if (w) {
					a++;
					ExpressionThing et(4, f);
					out.push_back(et);
				}
			}
			//
			// If the stack runs out without finding a left parenthesis,
			// then there are mismatched parentheses.
			//
			/* if (!((topToken == "(") || isFunction(topToken))) {
				return true;
			} */
		} else if (isFunction(token)) {
			//
			// found a function
			//
			// If the token is a function token, then push it onto the stack. Push 0 onto the arg
			// count stack. If the were values stack has a value on it, pop it and push true. Push
			// false onto were values
			//
			stack.push(token);
			arg_count.push(0);
			if (!were_values.empty()) {
				were_values.pop();
				were_values.push(true);
			}
			were_values.push(false);
		} else if (token == ",") {
			//
			// If the token is a function argument separator (e.g., a comma):
			// Until the topmost element of the stack is a left parenthesis, pop the element
			// onto the output queue. If no left parentheses are encountered, either the
			// separator was misplaced or parentheses were mismatched. Pop were values
			// into w. If w is true, pop arg count into a, increment a and push back into arg
			// count. Push false into were values.
			//
			for (;;) {
				if (stack.empty()) {
					throw PException(
							"stack was empty when looking for '(' after a function call");
				}
				string value = stack.top();
#ifdef DEBUG
				cout << "VALUE is [" << value << "]" << endl;
#endif
				if ((value == "(") || isFunction(value)) {
					break;
				} else {
					unsigned int atype = figureType(value);
					ExpressionThing et(atype, value);
					out.push_back(et);
					stack.pop();
				}
			}
			bool w = were_values.top();
			were_values.pop();
			if (w) {
				unsigned int a = arg_count.top();
				arg_count.pop();
				a++;
				arg_count.push(a);
			}
			were_values.push(false);
		}
		//
		// If the token is a number, then add it to the output queue. If the were values stack
		// has a value on it, pop it and push true.
		//
		else {
			unsigned int atype = figureType(token);
			ExpressionThing et(atype, token);
			out.push_back(et);
			if (!were_values.empty()) {
				were_values.pop();
				were_values.push(true);
			}
		}
	}
	//
	// While there are still operator tokens in the stack:
	//
	while (!stack.empty()) {
		const string stackToken = stack.top();
		//
		// If the operator token on the top of the stack is a parenthesis,
		// then there are mismatched parentheses.
		//
		if (isParenthesis(stackToken)) {
			return false;
		}
		//
		// Pop the operator onto the output queue.
		// type 1: operator
		//
		ExpressionThing et(1, stackToken);
		out.push_back(et);
		stack.pop();
	}
	strArray.assign(out.begin(), out.end());
	return success;
}

//
// get the tokens from the string
//
// There are 4 types of tokens:
//   - operator
//   - literal (integer or variable name or string)
//   - parenthesis
//   - function call
//

vector<string> ExpressionParser::getExpressionTokens(string expression) {
	//
	// numerical literal 1234, or 5.12e-12 -> recognizable by numeral or by "-" + numeral
	// string literal "a_string"            -> recognizable by quote
	// boolean literal true or false        -> recognizable by full word
	// function call my_function(a,b,c)     -> recognizable by word + paren
	// operator + / - * < > <= >= != == %   -> recognizable by full word
	// variable name abc
	//
	tokens.clear();
	string str = "";
	for (unsigned int i = 0; i < expression.length(); ++i) {
		char a_char = expression[i];
		if (a_char == ' ' || a_char == '\t')
		{
			continue;
		}
		//
		// handle the strings
		//
		if (a_char == ')' || a_char == '(' || a_char == ',')
		{
			str += a_char;
			tokens.push_back(str);
			str = "";
		}
		else if (a_char == '"') {
			i = getString(expression, i);
		} else if (a_char >= '0' && a_char <= '9') {
			i = getNumeric(expression, i);
		} else if (isStartToken(a_char)) {
			i = getOperator(expression, i);
		} else {
			string f = getFunction(expression, i);
			if (f != "") {
				i += f.length() - 1;
			} else {
				f = getBoolean(expression, i);
				if (f != "") {
					i += f.length();
				} else {
					i = getVariableName(expression, i);
				}
			}
		}
	}

#ifdef DEBUG
cout << "-------------------------" << endl;
	for (vector<string>::iterator it=tokens.begin();it< tokens.end();++it)
	{
		cout << "TOKEN = " << *it << endl;
	}
	cout << "-------------------------" << endl;

#endif


	return tokens;
}

unsigned int ExpressionParser::getVariableName(string expression,
		unsigned int i) {
	string str = "";
	for (unsigned int j = i; j < expression.length(); j++) {
		char a_char = expression[j];
		if ((a_char >= 'a' && a_char <= 'z') ||( a_char >= '0' && a_char <= '9') ){
			str += a_char;
		} else {
			tokens.push_back(str);
			return j-1;
		}
	}
	tokens.push_back(str);
	return expression.length();
}

unsigned int ExpressionParser::getOperator(string expression, unsigned int i) {
	string str = "";
	for (unsigned int j = i; j < expression.length(); j++) {
		char a_char = expression[j];
		if (isStartToken(a_char)) {
			str += a_char;
		} else {
			tokens.push_back(str);
			return j-1;
		}
	}
	return expression.length();
}

string ExpressionParser::getBoolean(string expression, unsigned int i) {

	if (expression.substr(i, i + 3) == "true") {
		return "true";
	} else if (expression.substr(i, i + 4) == "false") {
		return "false";
	}
	return "";
}

string ExpressionParser::getFunction(string expression, unsigned int i) {
	string str = "";
	bool whitespace = false;
	for (unsigned int j = i; j < expression.length(); j++) {
		char a_char = expression[j];
		if ((a_char >= 'a') && (a_char <= 'z')) {
			if (whitespace) {
				return "";
			}
			str += a_char;
		} else if (a_char == '(') {
			str += a_char;
			tokens.push_back(str);
			return str;
		} else if ((a_char == ' ') || (a_char == '\t')) {
			whitespace = true;
		}
	}
	return "";
}

unsigned int ExpressionParser::getNumeric(string expression, unsigned int i) {
	string str = "";
	for (unsigned int j = i; j < expression.length(); j++) {
		char a_char = expression[j];
		if ((a_char >= '0' && a_char <= '9') || a_char == '.'
				|| (a_char == 'e' && j>i)) {
			str += a_char;
		} else {
			tokens.push_back(str);
			return j -1;
		}
	}
	if (str != "")
		{
		tokens.push_back(str);
		}
	return expression.length();
}

unsigned int ExpressionParser::getString(string expression, unsigned int i) {
	string str = "\"";
	for (unsigned int j = i+1; j < expression.length(); j++) {
		char a_char = expression[j];
		str += a_char;
		if (a_char == '"') {
			tokens.push_back(str);
			return j + 1;
		}
	}
	throw new PException("found unmatched double quote in " + expression);
}


//
// parse string
//
ExpressionNode* ExpressionParser::parse(string s) {
//
// Example: string s = "( 1 + 2) * ( 3 / 4 )-(5+6)";
//
#ifdef DEBUG
	cout << "EXPRESSIONPARSER::PARSE " << s << endl;
#endif
	if (s.size() == 0) {
		throw PException("empty expression");
	}
//
// Tokenize input expression
//
	vector<string> tokens = getExpressionTokens(s);
//
// Evaluate feasible expressions
//
	vector<ExpressionThing> rpn;
	if (!infixToRPN(tokens, tokens.size(), rpn)) {
		throw PException("Mis-match in parentheses: " + s);
	}
	ExpressionNode* en = new ExpressionNode();
	en->setRpn(rpn);
	return en;
}

unsigned int ExpressionParser::figureType(string token) {
	unsigned int atype;
	if (isOperator(token)) {
		atype = 1;
	} else if (isFunction(token)) {
		atype = 4;
	} else if (isInt(token)) {
		atype = 2;
	} else if (isFloat(token)) {
		atype = 5;
	} else if (isString(token)) {
		atype = 7;
	} else if (isBool(token)) {
		atype = 6;
	} else {
		atype = 3;
	}
	return atype;
}
