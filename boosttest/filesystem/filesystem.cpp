#include <boost/filesystem.hpp>
#include <iostream>
#include <iomanip>
#include <cassert>

using namespace std;
namespace fs = boost::filesystem;

int main(){
	
	fs::resize_file("e:\\a.txt", 100);
}

// ����Ŀ¼ bool create_directory, 
// ����Ƕ�׵�Ŀ¼ bool create_directories ���Ѿ������򷵻�false

// �Ƴ��ļ����ļ��� bool remove  ��֮ǰ������ ����false�����ļ��в��գ�����false��
// �ݹ��ɾ��       uintmax_t remove_all  ɾ���ļ��м������������ݣ������Ƴ��ĸ���

// ������ void rename(const path &old_p, const path &new_p) //�����·���Ѿ����ڣ�������쳣

// ��ȡ��ǰ����Ŀ¼ void current_path()
// �ı䵱ǰ����Ŀ¼ void current_path(const path &p)


// �����ļ��ߴ� uintmax_t file_size(const path &p)  ��ʧ���򷵻�false
// �ı��ļ��ߴ� void resize_file(const path & p, uintmax_t new size)  // ���Աȵ�ǰ�ļ���

// const path &initial_path() // ���ص�һ�ε��ô˺���ʱ��current_path�ķ���ֵ

// file+status status(const path &p)

/////�ж�����

// �ж��ļ����� bool exists(file_status s)
//              bool exists(const path &p);

// �Ƿ����ļ��� �� is_directory

// is_empty  �����ļ��У��򷵻��Ƿ�Ϊ�գ������ļ��򷵻س����Ƿ�Ϊ0

// �Ƿ��ǳ����ļ� is_regular_file(file_status p)
//                is_regular_file(const path &p)

////// �ļ�ʱ������
// �����ļ�����޸�ʱ�� std::time_t last_write_time(const path &p)
// �����ļ�����޸�ʱ�� std::time_t last_write_time(const path &p, const std::time_t new_time)


// void copy(const path &from, const path &to);
// void copy(const path& from, const path& to, system::error_code& ec);

//void copy_directory(const path& from, const path& to);
//void copy_directory(const path& from, const path& to, system::error_code& ec);

// void copy_file(const path& from, const path& to);
// void copy_file(const path& from, const path& to, system::error_code& ec);

