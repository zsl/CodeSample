#include <stack>
#include <cassert>

#include <iostream>

using namespace std;

// ��ȡ���ȼ�
int priority(char c)
{
    switch (c){
        case '+': 
        case '-':
            return 0;
        case '*':
        case '/':
            return 1;
    }
}

int calculate(char op, int left, int right)
{
    switch(op){
        case '+':return left + right;
        case '-':return left - right;
        case '*':return left * right;
        case '/':return left / right; // if left < right ,�᷵��0����ע��
    }
}

int findMatch(const string &expr, int index)
{
    assert(expr[index] == '('); // ȷ��indexλ����'('
    int left = 1;
    int right = 0;
    for (int i = index + 1; i < expr.size(); ++i){
        if (expr[i] == '(')
            ++left;
        else if (expr[i] == ')'){
            ++right;
            if (left == right) // find
                return i;
        }
    }

    return -1; // cannot find
}


int evaluate(const string &expr)
{
    stack<char> opcode;  // �����
    stack<int> oprand;  // ������

    const size_t exprSize = expr.size();
    for (int i = 0; i < exprSize;){
        switch(expr[i]){
            case '+':
            case '-':
            case '*':
            case '/':{
                char opCur = expr[i];
                if ( !opcode.empty() ){
                    if ( priority(opcode.top() ) >= priority(opCur) ){ // ���ջ�� ��priority > ��ǰ�� ����ֵ
                        char op = opcode.top();
						opcode.pop();
                        int right = oprand.top();
                        oprand.pop();
                        if (oprand.empty() ) {
                            cerr << "illegal expr";
                            exit(1);
                        }

                        int left = oprand.top();
                        oprand.pop();

                        oprand.push( calculate(op, left, right) );
                    }
                }
                opcode.push(expr[i]);
                break;
             }
            case '(':
                 {
                    int pos = findMatch(expr, i);
                    if (pos == -1){
                        cerr << "cannot match ()";
                        exit(1);
                    }

                    oprand.push( evaluate(expr.substr(i + 1, pos - i - 1) ) );

                    i = pos + 1;
                    continue;
                }
            default: // ����
                 oprand.push(expr[i] - '0');
                 break;

        }

        ++i;
    }

	if (!opcode.empty()){
		while (!opcode.empty()){
			char op = opcode.top();
			opcode.pop();
			int right = oprand.top();
			oprand.pop();
			if (oprand.empty() ) {
				cerr << "illegal expr";
				exit(1);
			}

			int left = oprand.top();
			oprand.pop();

			oprand.push( calculate(op, left, right) );
		}
	}

    return oprand.top();
}

int main(int argc, char *argv[]){
    cout << evaluate(argv[1]);
}
