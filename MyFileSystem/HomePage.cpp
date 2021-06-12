#include "Head.h"

void HomePage() {
	cout << "**********帮助*******************************手册**********" << endl;
	cout << "*           mkfile filename —— 新建文件                 *" << endl;
	cout << "*             mkdir dirname —— 新建目录                 *" << endl;
	cout << "*           rmfile filename —— 删除文件                 *" << endl;
	cout << "*             rmdir dirname —— 删除目录                 *" << endl;
	cout << "*    cpfile filename source —— 复制文件                 *" << endl;//绝对路径和相对路径
	cout << "*      cpdir dirname source —— 复制目录                 *" << endl;//绝对路径和相对路径
	cout << "*    mvfile filename source —— 移动文件                 *" << endl;//绝对路径和相对路径
	cout << "*      mvdir dirname source —— 移动目录                 *" << endl;//绝对路径和相对路径
	cout << "*    rename oldname newname —— 重命名文件               *" << endl;
	cout << "*    rename oldname newname —— 重命名目录               *" << endl;
	cout << "*             open filename —— 打开文件：               *" << endl;
	cout << "*                        -r —— 读模式                   *" << endl;
	cout << "*                        -w —— 写模式                   *" << endl;
	cout << "*                        -a —— 追加模式；               *" << endl;
	cout << "*             read filename —— 读文件                   *" << endl;
	cout << "*    write filename content —— 写文件                   *" << endl;
	cout << "*    chmod filename content —— 修改文件打开方式：       *" << endl;
	cout << "*                        -r —— 读模式                   *" << endl;
	cout << "*                        -w —— 写模式                   *" << endl;
	cout << "*                        -a —— 追加模式；               *" << endl;
	cout << "*            close filename —— 关闭文件                 *" << endl;
	cout << "*                        ls —— 显示文件夹目录           *" << endl;
	cout << "*                cd dirname —— 切换目录                 *" << endl;//绝对路径和相对路径
	cout << "*                      cd.. —— 返回上级目录             *" << endl;
	cout << "*                    format —— 格式化文件系统           *" << endl;
	cout << "*                    logout —— 登出当前用户             *" << endl;
	cout << "*                      halt —— 退出文件系统             *" << endl;
	cout << "***********************************************************" << endl;
}


