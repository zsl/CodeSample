#include <fstream>
#include <iostream>
#include <iomanip>

void printHex(std::ifstream& ifs, std::ostream& ostream);
void showInfo();
/**
* 命令形式如：ShowHex [filename] [destfilename]
* 表示将filename的内容以16进制形式输出到destfilename
* 
* 如果参数个数等于1，则打印出帮助消息
* 如果参数个数等于2，则输出到控制台
* 如果参数个数等于3，则输出到文件
*/
int main(int argc, char *argv[]){
	using namespace std;
	
	if (argc == 1){
		showInfo();
		return 0;
	}
	
	ifstream ifs(argv[1], ios::in | ios::binary);
	if (!ifs){
		cerr<<"文件不能打开\n";
		cerr<<"按任意键退出……";
		getchar();
		exit(1);
	}

	if (argc == 2)
		printHex(ifs, cout);
	else if (argc == 3){
		ofstream ofs(argv[2], ios::out);
		printHex(ifs, ofs);
	}
	else{
		showInfo();
		exit(2);
	}
	cout<<"\n执行成功\n";
	cout<<"按任意键退出……";
	getchar();
	return 0;
}

// 注意，参数应为引用类型
void printHex(std::ifstream& ifs, std::ostream& ostream){
	using namespace std;
	ostream<<setfill('0')<<hex<<uppercase;

	unsigned char byte;
	unsigned long count = 0;
	while (true){
		ostream<<setw(8)<<count<<"    ";
		for (int i=0; i<8; ++i){
			if (ifs.read((char*)&byte, 1))
				ostream<<setw(2)<<(int)byte<<" ";
			else
				goto endfile;
		}
		ostream<<" ";
		for (int i=0; i<8; ++i){
			if (ifs.read((char*)&byte, 1))
				ostream<<setw(2)<<(int)byte<<" ";
			else
				goto endfile;
		}
		ostream<<endl;
		count += 16;
	}

endfile:
	ostream<<setfill(' ')<<dec;
}

void showInfo(){
	using namespace std;
	cout<<"命令形式：ShowHex [filename] [destfilename]\n";
	cout<<"表示将filename的内容以16进制形式输出到destfilename\n";
	cout<<"\n";
	cout<<"如果参数个数等于0，则打印出帮助消息\n";
	cout<<"如果参数个数等于1，则输出到控制台\n";
	cout<<"如果参数个数等于2，则输出到文件\n";
	cout<<"按任意键退出……";
	getchar();
}