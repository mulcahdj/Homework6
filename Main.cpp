#include <iostream>
#include <string>
#include <algorithm>
#include <deque>
#include <cstddef>

using namespace std;


void clean(string* str) {
	str->erase(remove(str->begin(), str->end(), ' '), str->end());	//remove spaces from input
	str->erase(remove(str->begin(), str->end(), '\t'), str->end());	//remove tabs from input
	str->erase(remove(str->begin(), str->end(), '\n'), str->end());	//remove newlines from input
}


deque<string> makeSegments(const string& str) {

	int l = str.length();
	bool hasMinus = false;
	string temp="";
	deque<string> out;

	if (str.at(0)=='-') hasMinus=true;
	
	bool number=false;
	//break str into segments that are either a bracket, an operation, or a number (possibly several chars)
	for (int i=1;i<l;i++){

		//is this segment making a number string?
		

		//for numbers, need to accumulate the full number before making a segment
		if (str.at(i)>='0' && str.at(i) <='9') {
			number=true;
			temp+=str.at(i);
		}
		else number=false;

		//if not a number, do other chars
		if (!number) {
			if (temp.compare("")!=0) out.push_back(temp);
			temp="";

			//brackets and operations are their own segments
			if (str.at(i) == '(') out.push_back("(");
			if (str.at(i) == ')') out.push_back(")");
			if (str.at(i) == '+') out.push_back("+");
			if (str.at(i) == '-') out.push_back("-");
			if (str.at(i) == '*') out.push_back("*");
			if (str.at(i) == '/') out.push_back("/");

		}

		//if we're on the last char and it's part of a number, push its segment
		if (number && i==(l-1)) out.push_back(temp);
	}
	
	if (hasMinus) out[0]=str.at(0)+out[0];
	else {
		out.push_front("");
		out[0]=str.at(0)+out[0];
	}

	return out;
}


//checks if a string is an operator
bool isOperator(const string& str) {
	return (str.compare("+")==0 || str.compare("-")==0 || str.compare("*")==0 || str.compare("/")==0);
}

//checks if a string is just a sequence of number chars
bool isNumber(const string& str) {
	size_t found;
	found = str.find_first_not_of("0123456789");
	return (found == string::npos);
}

bool isValid(const deque<string>& segments) {

	int l = segments.size();
	int leftBracketCount=0; 
	int rightBracketCount=0;
	int temp=0;

	//first count brackets, they must be equal
	for (int i=0;i<l;i++) {
		if (segments[i] == "(") leftBracketCount++;
		if (segments[i] == ")") rightBracketCount++;
	}
	
	//simple bracket count check
	if (leftBracketCount != rightBracketCount) return false;


	size_t found;
	//if first seg is not a number or a left bracket or a minus sign, return false
	found = segments[0].find_first_not_of("0123456789(-");
	if (found!=string::npos) return false; 

	//if last seg is not a number or a right bracket, return false
	found = segments[l-1].find_first_not_of("0123456789)");
	if (found!=string::npos && segments.size()>1) return false; 

	//loop over middle segments and check for bad combinations of numbers/ops/brackets
	for (int i=1;i<l-1;i++) {
		
		//if the segment is a left bracket, it must have an operator to its left 
		//and a number or left bracket to the right, but not a right bracket
		if (segments[i].compare("(")==0) {
			if((!isOperator(segments[i-1]) && segments[i+1].compare("(")!=0 
				&& (!isNumber(segments[i+1]))) 
				|| segments[i+1].compare(")")==0) return false;
		}

		//if the segment is a right bracket, it must have an operator to its right 
		//and a number or right bracket to the left, but not a left bracket
		if (segments[i].compare(")")==0) {
			if(!isOperator(segments[i+1]) && segments[i-1].compare(")")!=0 
				&& (!isNumber(segments[i-1]))
				|| segments[i-1].compare("(")==0) return false;
		}

		//operator must have either a number or an away-facing bracket to either side
		if (isOperator(segments[i])){
			if (isOperator(segments[i-1]) || segments[i-1].compare("(")==0 || 
				segments[i+1].compare(")")==0 || isOperator(segments[i+1])) return false;
		}
	}

	//if it hasn't returned false, return true...
	return true;
}

int main() {

	string exp;		//the expression to be entered

	cout << "Please enter an expression: ";
	getline(cin,exp);						//get expression from user

	//remove all whitespace from input
	clean(&exp);

	//segments is a list of strings that are each either a bracket, an operation, or a number
	deque<string> segments = makeSegments(exp);
		
	//if input is valid, echo it back
	if (isValid(segments)) {
		int s = segments.size();
		for (int i=0;i<s;i++) cout << segments[i];
		cout << endl;
	}
	//otherwise, output error message
	else cout << "Invalid input";
	
	
	cin.get();
}
