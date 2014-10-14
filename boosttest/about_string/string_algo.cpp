#include <boost/algorithm/string.hpp>
#include <algorithm>
#include <functional>
#include <string>
#include <cstdlib>
#include <cassert>

using namespace std;
using namespace boost;

class Isxdigit : public std::unary_function<char, bool>{
public:
	Isxdigit(){}
	bool operator()(char c) const{
		return isxdigit(c);
	}
};

int main(){
	// 1. 大小写转换
	string str("Hello World.");
	to_upper(str);
	assert(str == "HELLO WORLD.");

	to_lower(str);
	assert(str == "hello world.");

	// 2. 判定
	str = "filename.ExE";
	assert(iends_with(str, ".exe") );

	str = "123n";
	if (str.end() == find_if_not(str.begin(), str.end(), isxdigit ) )
		cout << "not find.\n";
	else
		cout << "find.\n";

	// 3. trim
	str = "  12hello world  ";
	trim(str); // trim_left, trim_right
	assert(str == "12hello world");
	trim_if(str, is_any_of("12 ")); // 不能移除中间的空格
	assert(str == "hello world");
	erase_all(str, " "); // str.erase(remove(str.begin(), str.end(), ' '), str.end() );
	assert(str == "helloworld");

	// 4. Replace
	str = "hello world, hello world";
	replace_first(str, "world", "hello");
	replace_last(str, "hello", "world");
	assert(str == "hello hello, world world");
	erase_head(str, 6); // str.erase(0, 6);
	erase_tail(str, 6); // str.erase(str.size() - 6);
	assert(str == "hello, world");


}

//Copy vs. Mutable: Many algorithms in the library are performing a transformation of the input. The transformation can be done in-place, mutating the input sequence, or a copy of the transformed input can be created, leaving the input intact. None of these possibilities is superior to the other one and both have different advantages and disadvantages. For this reason, both are provided with the library.

//	Algorithm stacking: Copy versions return a transformed input as a result, thus allow a simple chaining of transformations within one expression (i.e. one can write trim_copy(to_upper_copy(s))). Mutable versions have void return, to avoid misuse.

//Naming: Naming follows the conventions from the Standard C++ Library. If there is a copy and a mutable version of the same algorithm, the mutable version has no suffix and the copy version has the suffix _copy. Some algorithms have the prefix i (e.g. ifind_first()). This prefix identifies that the algorithm works in a case-insensitive manner.