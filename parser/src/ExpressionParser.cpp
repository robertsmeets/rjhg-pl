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
	return token == "+" || token == "-" || token == "*" || token == "/";
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
// Test if token is a comma
//
bool ExpressionParser::isComma(const string& token) {
	return token == ",";
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
			//	the stack onto the output queue.
			//	Pop the left parenthesis from the stack, but not onto the output queue.

			//
			// Until the token at the top of the stack is a left parenthesis,
			// pop operators off the stack onto the output queue.
			//
			string topToken = stack.top();
			while (topToken != "(") {
				//
				// type 1: operator ----- must evaluate what this is
				//
				unsigned int atype;
				if (isOperator(topToken)) {
					atype = 1;
				} else if (isFunction(topToken)) {
					atype = 4;
				} else if (isInt(token)) {
					atype = 2;
				} else {
					atype = 3;
				}
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
				if (stack.empty()) {
					cout << "Stack empty when receiving function token, ignored"
							<< endl;
				} else {
					stack.pop();
				}
				unsigned int a = arg_count.top();
				arg_count.pop();
				bool w = were_values.top();
				were_values.pop();
				if (w) {
					a++;
//					f.argument_count = a;
					ExpressionThing et(4, f);
					out.push_back(et);
				}
			}
			//
			// If the stack runs out without finding a left parenthesis,
			// then there are mismatched parentheses.
			//
			if (!((topToken == "(") || isFunction(topToken))) {
				return false;
			}
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
				if ((value == "(") || isFunction(value)) {
					break;
				} else {

					int atype;
					if (isInt(token)) {
						atype = 2;
					} else {
						atype = 3;
					}
					//
					// type 2 is a number
					// type 3 is a variable name
					//
					ExpressionThing et(atype, token);
					out.push_back(et);
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
			int atype;
			if (isInt(token)) {
				atype = 2;
			} else {
				atype = 3;
			}
			//
			// type 2 is a number
			// type 3 is a variable name
			//
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
// do a calculation based on the tokens
//
double ExpressionParser::RPNtoDouble(vector<string> tokens) {
	stack<string> st;

	// For each token
	for (int i = 0; i < (int) tokens.size(); ++i) {
		const string token = tokens[i];

		// If the token is a value push it onto the stack
		if (!isOperator(token)) {
			st.push(token);
		} else {
			double result = 0.0;

			// Token is an operator: pop top two entries
			const string val2 = st.top();
			st.pop();
			const double d2 = strtod(val2.c_str(), NULL);

			if (!st.empty()) {
				const string val1 = st.top();
				st.pop();
				const double d1 = strtod(val1.c_str(), NULL);

				//Get the result
				result = token == "+" ? d1 + d2 : token == "-" ? d1 - d2 :
							token == "*" ? d1 * d2 : d1 / d2;
			} else {
				if (token == "-")
					result = d2 * -1;
				else
					result = d2;
			}

			// Push result onto stack
			ostringstream s;
			s << result;
			st.push(s.str());
		}
	}
	return strtod(st.top().c_str(), NULL);
}

/*
 vector<string> ExpressionParser::getExpressionTokensOld(const string& expression) {
 vector<string> tokens;
 string str = "";
 for (int i = 0; i < (int) expression.length(); ++i) {
 //
 // look at one character and see if it is an operator or paren
 //
 const string token(1, expression[i]);
 if (isOperator(token) || isParenthesis(token)) {
 if (!str.empty()) {
 tokens.push_back(str);
 }
 str = "";
 tokens.push_back(token);
 } else {
 //
 // Append the numbers/letters
 //
 if (!token.empty() && token != " ") {
 str.append(token);
 } else {
 if (str != "") {
 tokens.push_back(str);
 str = "";
 }
 }
 }
 }
 //
 // check if empty
 //
 if (!str.empty()) {
 tokens.push_back(str);
 }
 return tokens;
 }
 */

//
// get the tokens from the string
//
// There are 4 types of tokens:
//   - operator
//   - literal (integer or variable name)
//   - parenthesis
//   - function call
//
vector<string> ExpressionParser::getExpressionTokens(const string& expression) {
	vector<string> tokens;
	string str = "";
	for (int i = 0; i < (int) expression.length(); ++i) {
		//
		// look at one character and see if it is an operator or paren
		//
		const string token(1, expression[i]);
		if (isOperator(token) || isComma(token) || (token == ")")) {
			if (!str.empty()) {
				//cout << "PUSHING <" << str << ">" << endl;
				tokens.push_back(str);
			}
			str = "";
			//cout << "PUSHING <" << token << ">" << endl;
			tokens.push_back(token);
		} else {
			//
			// function call "myfunction(1,2,3)"
			// should be tokenized as "myfunction(", "(", "," , "1" , "," , "2" , "," , "3" , ")"
			//
			// Append the numbers/letters
			//
			// There are the following possibilities:
			//  - integer "1234" or "-1234" or variable name "myvar"
			//  - function call start (may include one or more spaces, example "myfunction ("
			//
			if (!token.empty()) {
				if (token == " ") {
					//
					// now we need to look ahead because this is either a new token or part of a function call with a space
					//
					for (unsigned int j = i + 1; j < expression.length(); j++) {
						const string token2(1, expression[j]);
						if (token2 == "(") {
							//
							// found a function call
							//
							str.append("(");
							//cout << "PUSHING <" << str << ">" << endl;
							tokens.push_back(str);
							str = "";
							tokens.push_back("(");
							break;
						} else if (token2 != " ") {
							//
							// found something else, push and leave loop
							//
							if (str != "") {
								// cout << "PUSHING <" << str << ">" << endl;
								tokens.push_back(str);
								str = "";
							}
							break;
						}
						//
						// found a space, keep looping
						//
					}
				} else if (token == "(") {
					str.append("(");
					// cout << "PUSHING <" << str << ">" << endl;
					tokens.push_back(str);
					str = "";
				} else {
					str.append(token);
				}
			}
		}

	}
	//
	// check if empty
	//
	if (!str.empty()) {
		// cout << "PUSHING <" << str << ">" << endl;
		tokens.push_back(str);
	}
	return tokens;
}

//
// Print iterators in a generic way
//
template<typename T, typename InputIterator>
void ExpressionParser::Print(const string& message,
		const InputIterator& itbegin, const InputIterator& itend,
		const string& delimiter) {
	cout << message << endl;
	copy(itbegin, itend, ostream_iterator<T>(cout, delimiter.c_str()));
	cout << endl;
}

//
// parse string
//
ExpressionNode* ExpressionParser::parse(string s) {
	//
	// Example: string s = "( 1 + 2) * ( 3 / 4 )-(5+6)";
	//
	//Print<char, s_iter>("Input expression:", s.begin(), s.end(), "");
	if (s.size() == 0) {
		throw PException("empty expression");
	}
	//
	// Tokenize input expression
	//
	vector<string> tokens = getExpressionTokens(s);
	//cout << "-----------here come some tokens-----------" << endl;
	//for (vector<string>::iterator it = tokens.begin(); it != tokens.end();
	//		++it) {
	//	cout << "---> TOKEN " << *it << endl;
	//}
	//cout << "-------------------------------------------" << endl;
	//
	// Evaluate feasible expressions
	//
	vector<ExpressionThing> rpn;
	if (!infixToRPN(tokens, tokens.size(), rpn)) {
		throw new PException("Mis-match in parentheses: " + s);
	}
	ExpressionNode* en = new ExpressionNode();
	en->setRpn(rpn);
	return en;
}
