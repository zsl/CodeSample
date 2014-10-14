#include <boost/filesystem.hpp>
#include <iostream>
#include <iomanip>
#include <cassert>

using namespace std;
namespace fs = boost::filesystem;

int main(){
	
	fs::resize_file("e:\\a.txt", 100);
}

// 创建目录 bool create_directory, 
// 创建嵌套的目录 bool create_directories 若已经存在则返回false

// 移除文件或文件夹 bool remove  若之前不存在 返回false；若文件夹不空，返回false。
// 递归地删除       uintmax_t remove_all  删除文件夹及其中所有内容，返回移除的个数

// 重命名 void rename(const path &old_p, const path &new_p) //如果新路径已经存在，则出现异常

// 获取当前工作目录 void current_path()
// 改变当前工作目录 void current_path(const path &p)


// 返回文件尺寸 uintmax_t file_size(const path &p)  若失败则返回false
// 改变文件尺寸 void resize_file(const path & p, uintmax_t new size)  // 可以比当前文件大

// const path &initial_path() // 返回第一次调用此函数时，current_path的返回值

// file+status status(const path &p)

/////判定函数

// 判断文件存在 bool exists(file_status s)
//              bool exists(const path &p);

// 是否是文件夹 ： is_directory

// is_empty  若是文件夹，则返回是否为空；若是文件则返回长度是否为0

// 是否是常规文件 is_regular_file(file_status p)
//                is_regular_file(const path &p)

////// 文件时间属性
// 返回文件最后修改时间 std::time_t last_write_time(const path &p)
// 设置文件最后修改时间 std::time_t last_write_time(const path &p, const std::time_t new_time)


// void copy(const path &from, const path &to);
// void copy(const path& from, const path& to, system::error_code& ec);

//void copy_directory(const path& from, const path& to);
//void copy_directory(const path& from, const path& to, system::error_code& ec);

// void copy_file(const path& from, const path& to);
// void copy_file(const path& from, const path& to, system::error_code& ec);

