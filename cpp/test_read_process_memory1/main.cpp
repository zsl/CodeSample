#include "ProcessUtil.h"
#include "CMemoryMD.h"
#include "m_util.h"
#include <Windows.h>
#include <string>
#include <cstring>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>
#include <boost/algorithm/string.hpp>

#include <memory>

#pragma warning( disable : 4996 )

using namespace std;

namespace{

	class CArgsError : public std::runtime_error{
	public:
		CArgsError(char *msg) : std::runtime_error(msg){}
		CArgsError(const std::string &msg) : std::runtime_error(msg){}
	};
	char s_szProcName[MAX_PATH];
	unsigned int s_baseAddr;
	unsigned int s_nMemorySize;

	shared_ptr<CMemoryMD> s_spMemoryMD;

	char *szUsage = "usage: MemoryCheck exe=base_name addr=[0x]addr size=[0x]size algo=algo_name.";

	/*
	 exe=
	 addr=
	 size=
	 alog=
	*/
    void parseArgs(int argc, char *argv[]){
		char key[10];
		char value[256];

		if (argc < 3 + 1){
			throw CArgsError(szUsage);
		}

		for (int i = 1; i < argc; ++i){
			memset(key, 0, sizeof key);
			memset(value, 0, sizeof value);

			int count = sscanf(argv[i], "%[^=]=%s", key, value);// ÓÐÒç³öµÄÎ£ÏÕ

			if (count < 2){
				string msg("error args format: ");
				msg += argv[i];
				throw CArgsError(msg);
			}

			if ( 0 == strcmp(key, "exe") ){
				strncpy(s_szProcName, value, MAX_PATH);
			}
			else if ( 0 == strcmp(key, "addr") ){
				char *szFormat = NULL;
				if ( boost::istarts_with(value, "0x") )
					szFormat = "0x%x";
				else
					szFormat = "%u";

				sscanf(value, szFormat, &s_baseAddr);
			}
			else if ( 0 == strcmp(key, "size") ){
				char *szFormat = NULL;
				if ( boost::istarts_with(value, "0x") )
					szFormat = "0x%x";
				else
					szFormat = "%u";

				sscanf(value, szFormat, &s_nMemorySize);
			}
			else if ( 0 == strcmp(key, "algo") ){
				if ( s_spMemoryMD.get() == NULL ){
					if ( boost::iequals(value, "crc32") )
						s_spMemoryMD = make_shared<CMemoryCrc32>() ;
					else if ( boost::iequals(value, "sha1") )
						s_spMemoryMD = make_shared<CMemorySha1>();
				}
			}
			else{
				string msg("not find args:");
				msg += argv[i];
				throw CArgsError(msg);
			}
		}// end for i

		if ( !s_spMemoryMD )
			s_spMemoryMD = make_shared<CMemorySha1>();
	}
}

int main(int argc, char *argv[]){

	try{
		parseArgs(argc, argv);


		HANDLE hInq70 = GetProcessHandleByBaseName(s_szProcName);
		if (hInq70 <= 0){
			cout << "cannot find inq.\n";
			exit(1);
		}

		s_spMemoryMD->caculateMD(hInq70, (void *)s_baseAddr, s_nMemorySize);

		CloseHandle(hInq70);

		cout << byteArrToHexString( (char *)s_spMemoryMD->getMD(), s_spMemoryMD->getLen() );
	}
	catch(runtime_error &e){
		cout << e.what() << endl;
		exit(1);
	}
}