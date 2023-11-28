#include "arithmeticExpression.h"
#include <stack>
#include <fstream>
#include <sstream>
using namespace std;



arithmeticExpression::arithmeticExpression(const string& value)
{
	infixExpression = value;
	root = nullptr;
}

void arithmeticExpression::buildTree()
{
	stack<TreeNode*> tree;
	string input;
	char key = 'a';
	TreeNode* ROOT;
	input = infix_to_postfix();
	cout << input << endl;
	for (unsigned i = 0; i < input.size(); i++) {
		if (input.at(i) == '*' || input.at(i) == '+' || input.at(i) == '-' || input.at(i) == '/') {
			ROOT = new TreeNode(input.at(i), key++);
			//stores and assignes top to right
			ROOT->right = tree.top();
			tree.pop();
			//stores and assigns NEW top to the left 
			ROOT->left = tree.top();
			tree.pop();
			//add this subexpression to the stack
			tree.push(ROOT);
		}
		else {
			ROOT = new TreeNode(input.at(i), key++);
			tree.push(ROOT);
		}
	}
	//only element will be the root now 
	root = tree.top();
	tree.pop();
}

void arithmeticExpression::infix()
{
	infix(root);
}

void arithmeticExpression::prefix()
{
	prefix(root);
}

void arithmeticExpression::postfix()
{
	postfix(root);
}

void arithmeticExpression::visualizeTree(const string& outputFilename) {
	ofstream outFS(outputFilename.c_str());
	if (!outFS.is_open()) {
		cout << "Error opening " << outputFilename << endl;
		return;
	}
	outFS << "digraph G {" << endl;
	visualizeTree(outFS, root);
	outFS << "}";
	outFS.close();
	string jpgFilename = outputFilename.substr(0, outputFilename.size() - 4) + ".jpg";
	string command = "dot -Tjpg " + outputFilename + " -o " + jpgFilename;
	system(command.c_str());
}

int arithmeticExpression::priority(char op) {
	int priority = 0;
	if (op == '(') {
		priority = 3;
	}
	else if (op == '*' || op == '/') {
		priority = 2;
	}
	else if (op == '+' || op == '-') {
		priority = 1;
	}
	return priority;
}

string arithmeticExpression::infix_to_postfix() {
	stack<char> s;
	ostringstream oss;
	char c;
	for (unsigned i = 0; i < infixExpression.size(); ++i) {
		c = infixExpression.at(i);
		if (c == ' ') {
			continue;
		}
		if (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')') { //c is an operator
			if (c == '(') {
				s.push(c);
			}
			else if (c == ')') {
				while (s.top() != '(') {
					oss << s.top();
					s.pop();
				}
				s.pop();
			}
			else {
				while (!s.empty() && priority(c) <= priority(s.top())) {
					if (s.top() == '(') {
						break;
					}
					oss << s.top();
					s.pop();
				}
				s.push(c);
			}
		}
		else { //c is an operand
			oss << c;
		}
	}
	while (!s.empty()) {
		oss << s.top();
		s.pop();
	}
	return oss.str();
}

void arithmeticExpression::infix(TreeNode* node)
{
	if (node->left == nullptr) {
		cout << node->data;
		return;
	}
		cout << '(';
		infix(node->left);
		cout << node->data ;
		infix(node->right);
		cout << ')';
}

void arithmeticExpression::prefix(TreeNode* node)
{
	if (node)
	{
		cout << node->data ;
		prefix(node->left);
		prefix(node->right);
	}
}

void arithmeticExpression::postfix(TreeNode* node)
{
	if(node) 
	{
		postfix(node->left);
		postfix(node->right);
		cout << node->data ;
	}
}

void arithmeticExpression::visualizeTree(ofstream& os, TreeNode *curr)
{
	cout << curr->key << "[label=\"" << curr->data << "\"];" << endl;
	if (curr->left != nullptr) {
		cout << curr->key << "-> " << curr->left->key << ";" << endl;
		visualizeTree(os, curr->left);
	}
	if (curr->right != nullptr) {
		cout << curr->key << "->" << curr->right->key << ";" << endl;
		visualizeTree(os, curr->right);
	}
}