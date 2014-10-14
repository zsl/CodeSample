#include <boost/date_time/gregorian/gregorian.hpp>
#include <iostream>

using namespace std;
using namespace boost::gregorian;

int main()
{
	string s;
	std::cout << "Enter birth day YYYY-MM-DD (eg: 2002-02-01): ";
	std::cin >> s;
	try {
		date birthday(from_simple_string(s));
		date today = day_clock::local_day();
		days days_alive = today - birthday;
		days one_day(1);
		if (days_alive == one_day) {
			std::cout << "Born yesterday, very funny" << std::endl;
		}
		else if (days_alive < days(0)) {
			std::cout << "Not born yet, hmm: " << days_alive.days() 
				<< " days" <<std::endl;
		}
		else {
			std::cout << "Days alive: " << days_alive.days() << std::endl;
		}

	}
	catch(...) {
		std::cout << "Bad date entered: " << s << std::endl;
	}
	return 0;
}