#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

typedef unsigned char byte;
template<typename Tchar>
void toHexString(basic_string<Tchar> &result, const vector<byte> &bytes, const basic_string<Tchar> &splitter = sizeof(Tchar) == 1 ? (Tchar*)"" : (Tchar*)L"");

template<typename Tchar>
void toHexString(basic_string<Tchar> &result, const vector<byte> &bytes, const basic_string<Tchar> &splitter/* = ""*/){
	Tchar *hexString = sizeof(Tchar) == 1 ? (Tchar*)"0123456789ABCDEF" : (Tchar*)L"0123456789ABCDEF";
	const size_t sizeVec = bytes.size();

	result.clear();

	Tchar buf[3];
	for (size_t i = 0; i < sizeVec; ++i){
		memset(buf, 0, sizeof(buf));

		byte b = bytes[i];
		buf[1] = hexString[b & 0x0f];
		buf[0] = hexString[b >> 4];
		result.append(buf).append(splitter);
	}
	result.erase(result.size() - splitter.size());
}

void toHexString_sprintf(string &result, const vector<byte> &bytes, const string &splitter/* = ""*/){
	result.clear();

	char buf[3];
	for (size_t i = 0; i < bytes.size(); ++i){
		memset(buf, 0, sizeof(buf));

		sprintf(buf, "%02X", bytes[i]);
		result.append(buf).append(splitter);
	}
	result.erase(result.size() - splitter.size());
}

void toHexString_ostream(string &result, const vector<byte> &bytes, const string &splitter/* = ""*/){
	ostringstream oss;
	oss << setfill('0') << hex << uppercase;

	for (size_t i = 0; i < bytes.size(); ++i){
		oss << setw(2) << (int)bytes[i] << splitter;
	}

	result.swap(oss.str());
	result.erase(result.size() - splitter.size());
}

int byte2string_main(){
	byte barr[] = {15, 16, 17};
	vector<byte> vec(barr, barr + sizeof(barr));

	string hexString;
	toHexString(hexString, vec, string(" "));

	cout << hexString;

	return 0;
}