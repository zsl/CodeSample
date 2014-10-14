#include "Parsefilemodel.h"

#include <iostream>
#include <cassert>

int main(){
	using namespace std;

	vector<char> tokens;
	ParseFileModel *pf = ParseFileModel::GetInstance();
	pf->FilterComment("f:/code.txt", tokens);

	for (int i=0; i<tokens.size(); ++i)
		cout<<tokens[i]<<" ";
	cout<<endl;

	vector<LineNode> lines;
	pf->ReadLines(tokens, lines);
	for (int i=0; i<lines.size(); ++i){
		cout<<lines[i].strLine<<endl;
	}
}