#include <fstream>
#include <iostream>
#include <iomanip>

void printHex(std::ifstream& ifs, std::ostream& ostream);
void showInfo();
/**
* ������ʽ�磺ShowHex [filename] [destfilename]
* ��ʾ��filename��������16������ʽ�����destfilename
* 
* ���������������1�����ӡ��������Ϣ
* ���������������2�������������̨
* ���������������3����������ļ�
*/
int main(int argc, char *argv[]){
	using namespace std;
	
	if (argc == 1){
		showInfo();
		return 0;
	}
	
	ifstream ifs(argv[1], ios::in | ios::binary);
	if (!ifs){
		cerr<<"�ļ����ܴ�\n";
		cerr<<"��������˳�����";
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
	cout<<"\nִ�гɹ�\n";
	cout<<"��������˳�����";
	getchar();
	return 0;
}

// ע�⣬����ӦΪ��������
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
	cout<<"������ʽ��ShowHex [filename] [destfilename]\n";
	cout<<"��ʾ��filename��������16������ʽ�����destfilename\n";
	cout<<"\n";
	cout<<"���������������0�����ӡ��������Ϣ\n";
	cout<<"���������������1�������������̨\n";
	cout<<"���������������2����������ļ�\n";
	cout<<"��������˳�����";
	getchar();
}