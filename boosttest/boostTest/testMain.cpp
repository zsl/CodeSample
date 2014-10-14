#include <boost/lambda/lambda.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <string>

using namespace std;
using namespace boost;

int main(){
	string str1(" hello world! ");
	to_upper(str1);
	trim(str1);

	cout << str1 << endl;

	string str2 = to_lower_copy(
		ireplace_first_copy(
		str1, "hello", "goodbye"));

	cout << str2 << endl;
}