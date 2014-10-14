#include <boost/xpressive/xpressive_dynamic.hpp>

using namespace boost::xpressive;

int main(){
	char *str = "there is POWER-suit item";
	cregex rex = cregex::compile("(power)-(.{4})", icase);


}