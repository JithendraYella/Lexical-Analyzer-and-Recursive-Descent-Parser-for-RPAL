// rdt-parser.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stack>
#include <vector>
#include <string>

using namespace std;

struct Node {
	string value;
	Node *leftChild = NULL;
	Node *rightSibling = NULL;
};

void fn_E(); void fn_Ew(); void fn_T(); void fn_Ta();
void fn_Tc(); void fn_B(); void fn_Bt(); void fn_Bs();
void fn_Bp(); void fn_A(); void fn_At(); void fn_Af();
void fn_Ap(); void fn_R(); void fn_Rn(); void fn_D();
void fn_Da(); void fn_Dr(); void fn_Db(); void fn_Vb();
void fn_Vl();

stack<Node*> myStack;
int counter = 0;
fstream fs;
string NextToken;
//int N = 0;

string keywords[] = { "let", "in", "fn", "where", "aug", "or", "not", "gr", "ge", "ls", "le", "eq", "ne",
"true", "false", "dummy", "within", "and", "rec", "neg", "tau", "function_form", "lambda", "gamma", "nil", "","<true>", "<false>", "<dummy>","<nil>" };

string opers[] = { "->", ">=",  "<=", "**", ".", ",", "|", ">", "<", "+", "-", "*", "/", "@", "(", ")", "=" , "'", "&", "()" };

string allNodes[] = { "tau", "function_form", ",", "and", "lambda" };

string three[] = { "@", "->" };

bool present_three(string Token) {
	for (int i = 0; i<(sizeof(three) / sizeof(three[0])); ++i) {
		if (Token.compare(three[i]) == 0)
			return 1;
	}
	return 0;
}

bool present_all(string Token) {
	for (int i = 0; i<(sizeof(allNodes) / sizeof(allNodes[0])); ++i) {
		if (Token.compare(allNodes[i]) == 0)
			return 1;
	}
	return 0;
}

bool present(char c, int pl) {
	if (pl == 0) {
		for (int i = 0; i < 20; ++i) {
			if (opers[i].at(pl) == c)
				return 1;
		}
		return 0;
	}
	else if (pl == 1) {
		for (int i = 0; i < 4; ++i) {
			if (opers[i].at(pl) == c)
				return 1;
		}
		return 0;
	}
}

bool isoper(string Token) {
	for (int i = 0; i < 20; ++i) {
		if (Token.compare(opers[i]) == 0)
			return 1;
	}
	return 0;
}

bool isKeyword(string Token) {
	for (int i = 0; i < 30; ++i) {
		if (Token.compare(keywords[i]) == 0)
			return 1;
	}
	return 0;
}

bool integ(string Token) {
	if (Token.length() == 0)
		return 0;
	for (size_t i = 0; i<Token.length(); ++i) {
		if (!isdigit(Token.at(i)))
			return 0;
	}
	return 1;
}

bool ident(string Token) {
	if (Token.length() == 0)
		return 0;
	for (size_t i = 0; i < Token.length(); ++i) {
		if (!(isalnum(Token.at(i)) || Token.at(i)=='_'))
			return 0;
	}
	if (isKeyword(Token) || isoper(Token))
		return 0;
	return 1;
}

bool str(string Token) {
	if (Token.length() == 0)
		return 0;
	if (!(Token.at(0) == '\'' && Token.at(Token.length() - 1) == '\''))
		return 0;
	else
		return 1;
}

bool strEq(const char *arg0, const char *arg1) {		//compare two arguments
	string var0 = (string)arg0;	//converting both the arguments to strings and then using compare
	string var1 = (string)arg1;
	if (var0 == var1)
		return true;
	else
		return false;
}

void printTree(Node *root) {
	if (root != NULL) {
		for (int j = 0; j<counter; ++j) { cout << "."; }
		if (integ(root->value))
			cout << "<INT:" << root->value << ">" << endl;
		else if (str(root->value))
			cout << "<STR:" << root->value << ">" << endl;
		else if (ident(root->value))
			cout << "<ID:" << root->value << ">" << endl;
		else
			cout << root->value << endl;
		counter++;
		printTree(root->leftChild); counter--;
		printTree(root->rightSibling);
	}
}

void buildTree(string s, int i) {
	if (i == 0) {
		Node *temp = new Node;
		temp->value = s;
		myStack.push(temp);
		//if(temp->value=="")
		//	myStack.pop();
	}
	else if (i == 1) {
		Node *temp = myStack.top();
		myStack.pop();
		Node *newHead = new Node;
		newHead->leftChild = temp;
		newHead->value = s;
		myStack.push(newHead);
	}
	else if (i == 2) {
		Node *temp = myStack.top();
		myStack.pop();
		myStack.top()->rightSibling = temp;
		temp = myStack.top();
		myStack.pop();
		Node *newRoot = new Node;
		newRoot->value = s;
		newRoot->leftChild = temp;
		myStack.push(newRoot);
	}
	else {
		if (present_all(s)) {
			vector<Node *> allNodes;
			for (int p = 1; p<i; ++p) {
				allNodes.push_back(myStack.top());
				myStack.pop();
			}
			buildTree(s, 1);
			for (int p = 0; p<allNodes.size(); ++p) {
				Node *trv = myStack.top();
				trv = trv->leftChild;
				for (int p = 0; p<allNodes.size(); ++p) {
					trv->rightSibling = allNodes[allNodes.size() - p - 1];
					trv = trv->rightSibling;
				}
			}
		}
		else if (present_three(s)) {
			vector<Node *> threeNodes;
			for (int p = 1; p<i; ++p) {
				threeNodes.push_back(myStack.top());
				myStack.pop();
			}
			buildTree(s, 1);
			for (int p = 0; p<2; ++p) {
				Node *trv = myStack.top();
				trv = trv->leftChild;
				for (int p = 0; p<2; ++p) {
					trv->rightSibling = threeNodes[threeNodes.size() - p - 1];
					trv = trv->rightSibling;
				}
			}
		}
		else {
			vector<Node*> myNodes;
			for (int p = 2; p<i; ++p) {
				myNodes.push_back(myStack.top());
				myStack.pop();
			}
			buildTree(s, 2);
			for (int p = 0; p<myNodes.size(); ++p) {
				myStack.push(myNodes[myNodes.size() - p - 1]);
				buildTree(s, 2);
			}
		}
	}
	//printTree(myStack.top());
	// cout << "...................." << endl;
}

string scanNextToken() {
	char x;
	x = fs.peek();
	char c = x;
	int i = 0;
	char *myToken = NULL;
	string Token;
	while (!fs.eof()) {
		fs.get(c);
		if (isblank(c) || (c == '\n')) {
			if (myToken == NULL)
				continue;
			else
				break;
		}
		else if (c == '/') {
			if (fs.peek() == '/') {
				fs.get(c);
				while (!(c == '\n')) {
					fs.get(c);
					if (fs.eof()) {
						return "";
					}
					//cout << c << endl;
				}
			}
else{
				if (myToken == NULL) {
					myToken = new char[2];
					i = 0;
					myToken[i] = c;
					++i;
				}
				break;
}
			if (myToken == NULL)
				continue;
			else
				break;
		}

		else if (c == '\'') {
			if (myToken == NULL) {
				myToken = new char[100];
				i = 0;
			}
			do {
				myToken[i] = c;
				fs.get(c);
				++i;
				if (c == '\\') {
				//	myToken[i] = c;
				//	++i;
					if (fs.peek() == '\'') {
						fs.get(c);
						myToken[i] = c;
						fs.get(c);
						++i;
					}
				}
			} while (c != '\'');
			myToken[i] = c;
			++i;
			break;
		}

		else if (present(c, 0)) {
			if (myToken == NULL) {
				myToken = new char[3];
				i = 0;
			}
			myToken[i] = c;
			++i;
			if (c == '-' || c == '>' || c == '<' || c == '*') {
				if (present(fs.peek(), 1)) {
					fs.get(c);
					myToken[i] = c;
					++i;
				}
			}
			break;
		}
		else if (isalnum(c) || (c == '_')) {
			if (myToken == NULL) {
				myToken = new char[50];
				i = 0;
			}
			myToken[i] = c;
			++i;
			if (!(isalnum(fs.peek()) || (fs.peek() == '_')))
				break;
			else
				continue;
		}

	}
	if (myToken != NULL)
		myToken[i] = '\0';

	if (myToken == NULL)
		Token = "";
	else
		Token = string(myToken);

	return Token;
}

void Read(string Token) {
	if (Token.compare(NextToken)) {
		cout << "Expected " << Token << " but found " << NextToken << endl;
		exit(1);
	}
	if (integ(Token) || ident(Token) || str(Token)) {
		buildTree(Token, 0);
	}
	NextToken = scanNextToken();
}

void printAST() {
	NextToken = scanNextToken();
	fn_E();
	printTree(myStack.top());
}

void fn_E() {
	//cout << "fn_E";
	if (NextToken.compare("let") == 0) {
		Read("let");
		fn_D();
		Read("in");
		fn_E();
		buildTree("let", 2);
		//cout << "LET" << endl;
	}
	else if (NextToken.compare("fn") == 0) {
		Read("fn");
		int N = 0;
		do {
			fn_Vb();
			++N;
		} while (NextToken.compare("(") == 0 || ident(NextToken));
		Read(".");
		fn_E();
		buildTree("lambda", N + 1);
		//cout << "LAMBDA" << endl;
	}
	else
		fn_Ew();
}

void fn_Ew() {
	fn_T();
	if (NextToken.compare("where") == 0) {
		Read("where");
		fn_Dr();
		buildTree("where", 2);
		//cout << "WHERE" << endl;
	}
}

void fn_T() {
	fn_Ta();
	if (NextToken.compare(",") == 0) {
		int N = 0;
		do {
			Read(",");
			fn_Ta();
			++N;
		} while (NextToken.compare(",") == 0);
		buildTree("tau", N + 1);
		//cout << "TAU" << endl;
	}
}

void fn_Ta() {
	fn_Tc();
	if (NextToken.compare("aug") == 0) {
		while (NextToken.compare("aug") == 0) {
			Read("aug");
			fn_Tc();
			buildTree("aug", 2);
		}

		//cout << "AUG" << endl;
	}
}

void fn_Tc() {
	fn_B();
	if (NextToken.compare("->") == 0) {
		Read("->");
		fn_Tc();
		Read("|");
		fn_Tc();
		buildTree("->", 3);
		//cout << "->" << endl;
	}
}

void fn_B() {
	fn_Bt();
	if (NextToken.compare("or") == 0) {
		while (NextToken.compare("or") == 0) {
			Read("or");
			fn_Bt();
			buildTree("or", 2);
		}

		//cout << "OR" << endl;
	}
}

void fn_Bt() {
	fn_Bs();
	if (NextToken.compare("&") == 0) {
		while (NextToken.compare("&") == 0) {
			Read("&");
			fn_Bs();
			buildTree("&", 2);
		}

		// cout << "&" << endl;
	}
}

void fn_Bs() {
	if (NextToken.compare("not") == 0) {
		Read("not");
		fn_Bp();
		buildTree("not", 1);
		// cout << "NOT" << endl;
	}
	else
		fn_Bp();
}

void fn_Bp() {
	fn_A();
	if (NextToken.compare("gr") == 0 || NextToken.compare(">") == 0) {
		Read(NextToken);
		fn_A();
		buildTree("gr", 2);
		// cout << "GR" << endl;
	}
	else if (NextToken.compare("ge") == 0 || NextToken.compare(">=") == 0) {
		Read(NextToken);
		fn_A();
		buildTree("ge", 2);
		// cout << "GE" << endl;
	}
	else if (NextToken.compare("ls") == 0 || NextToken.compare("<") == 0) {
		Read(NextToken);
		fn_A();
		buildTree("ls", 2);
		// cout << "LS" << endl;
	}
	else if (NextToken.compare("le") == 0 || NextToken.compare("<=") == 0) {
		Read(NextToken);
		fn_A();
		buildTree("le", 2);
	}
	else if (NextToken.compare("eq") == 0) {
		Read(NextToken);
		fn_A();
		buildTree("eq", 2);
	}
	else if (NextToken.compare("ne") == 0) {
		Read(NextToken);
		fn_A();
		buildTree("ne", 2);
	}
}

void fn_A() {
	if (NextToken.compare("-") == 0) {
		Read("-");
		fn_At();
		buildTree("neg", 1);
		// cout << "NEG" << endl;
	}
	else if (NextToken.compare("+") == 0) {
		Read("+");
		fn_At();
	}
	else
		fn_At();
	if (NextToken.compare("+") == 0 || NextToken.compare("-") == 0) {
		while (NextToken.compare("+") == 0 || NextToken.compare("-") == 0) {
			string Temp = NextToken;
			Read(NextToken);
			fn_At();
			buildTree(Temp, 2);
			// cout << Temp << endl;
		}
	}
}

void fn_At() {
	fn_Af();
	if (NextToken.compare("*") == 0 || NextToken.compare("/")==0) {

		while (NextToken.compare("*") == 0 || NextToken.compare("/")==0) {
			string Temp = NextToken;
			Read(Temp);
			fn_Af();
			buildTree(Temp, 2);
		}
	}
}

void fn_Af() {
	fn_Ap();
	if (NextToken.compare("**") == 0) {
		Read("**");
		fn_Af();
		buildTree("**", 2);
	}
}

void fn_Ap() {
	fn_R();
	if (NextToken.compare("@") == 0) {
		while (NextToken.compare("@") == 0) {
			Read("@");
			if (ident(NextToken)) {
				Read(NextToken);
			}
			fn_R();
			buildTree("@", 3);
			// cout << "@" << endl;
		}
	}
}

void fn_R() {
	fn_Rn();
	if (NextToken.compare("true") == 0 || NextToken.compare("false") == 0 || NextToken.compare("nil") == 0 ||
		NextToken.compare("(") == 0 || NextToken.compare("dummy") == 0 || ident(NextToken)
		|| integ(NextToken) || str(NextToken)) {

		while (NextToken.compare("true") == 0 || NextToken.compare("false") == 0 || NextToken.compare("nil") == 0 ||
			NextToken.compare("(") == 0 || NextToken.compare("dummy") == 0 || ident(NextToken)
			|| integ(NextToken) || str(NextToken)) {
			fn_Rn();
			buildTree("gamma", 2);
		}

		// cout << "GAMMA" << endl;
	}
}

void fn_Rn() {
	if (ident(NextToken) || integ(NextToken) || str(NextToken))
		Read(NextToken);
	else if(NextToken.compare("") == 0){}
	else if (NextToken.compare("true") == 0) {
		Read(NextToken);
		buildTree("<true>", 0);
	}
	else if (NextToken.compare("false") == 0) {
		Read(NextToken);
		buildTree("<false>", 0);
	}
	else if (NextToken.compare("nil") == 0) {
		Read(NextToken);
		buildTree("<nil>", 0);
	}
	else if (NextToken.compare("dummy") == 0) {
		Read(NextToken);
		buildTree("<dummy>", 0);
	}
	else if (NextToken.compare("(") == 0) {
		Read("(");
		fn_E();
		Read(")");
	}
	else {
		 cout << "Error\n" << endl;
		exit(1);
	}
}

void fn_D() {
	fn_Da();
	if (NextToken.compare("within") == 0) {
		Read("within");
		fn_D();
		buildTree("within", 2);
		// cout << "WITHIN" << endl;
	}
}

void fn_Da() {
	fn_Dr();
	if (NextToken.compare("and") == 0) {
		int N = 0;
		do {
			Read("and");
			fn_Dr();
			++N;
		} while (NextToken.compare("and") == 0);
		buildTree("and", N + 1);
		// cout << "AND" << endl;
	}
}

void fn_Dr() {
	if (NextToken.compare("rec") == 0) {
		Read("rec");
		fn_Db();
		buildTree("rec", 1);
	}
	else
		fn_Db();
}

void fn_Db() {
	if (NextToken.compare("(") == 0) {
		Read("(");
		fn_D();
		Read(")");
	}
	else if (ident(NextToken)) {
		Read(NextToken);
		if (ident(NextToken) || NextToken.compare("(") == 0) {
			int N = 0;
			while (ident(NextToken) || NextToken.compare("(") == 0) {
				fn_Vb();
				++N;
			}
			Read("=");
			fn_E();
			buildTree("function_form", N + 2);
			// cout << "FUNCTION_FORM" << endl;
		}
		else {
			if (NextToken.compare(",") == 0) {
				int N = 0;
				while (NextToken.compare(",") == 0) {
					Read(",");
					if (ident(NextToken))
						Read(NextToken);
					else {
						cout << "Error: Expected " << NextToken << endl;
						exit(1);
					}
					++N;
				}
				buildTree(",", N + 1);
			}
			Read("=");
			fn_E();
			buildTree("=", 2);
			// cout << "=" << endl;
		}
	}
}

void fn_Vb() {
	if (ident(NextToken)) {
		Read(NextToken);
	}
	else if (NextToken.compare("(") == 0) {
		Read("(");
		if (NextToken.compare(")") == 0) {
			Read(")");
			buildTree("()", 0);
		}
		else if (ident(NextToken)) {
			//Read(NextToken);
			fn_Vl();
			Read(")");
		}
	}
}

void fn_Vl() {
	if (ident(NextToken))
		Read(NextToken);
	if (NextToken.compare(",") == 0) {
		int N = 0;
		while (NextToken.compare(",") == 0) {
			Read(",");
			if (ident(NextToken))
				Read(NextToken);
			++N;
		}
		buildTree(",", N + 1);
		// cout << "," << endl;
	}
}

void printUsage() {	//If the user enters the command incorrectly or gives the arguments incorrectly etc., this is printed out
	cout << endl;
	cout << "This is how you use it: <ExecutableName> -ast <FileName>";
	cout << "\n" << endl;
	cout << "Name of the executable followed by '-ast' argument followed by the name of the file\n";
}

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printUsage();
	}
	else {
		string s = "-ast";
		if (s.compare(argv[1]) == 0) {
			fs.open(argv[2], fstream::in);

			printAST();
		}
		else {
			printUsage();
		}
	}
	return 0;
}

