#include "Head.h"

int CheckUser(string UserName, string PassWord) // 检查该用户是否是初始化的用户
{
	for (int i=0;i<myFileSystem.reg_users.size();++i)
	{
		if (myFileSystem.reg_users[i].p_name==UserName&& myFileSystem.reg_users[i].password==PassWord)
		{
			return i;
		}
	}
	cout << "您的用户名或密码错误！请重新输入！" << endl;
	return -1;
}

bool LogInFileSystem()
{
	string UserName, PassWord;
	while (1)
	{
		cout << "请输入用户名:";
		cin >> UserName;
		cout << "请输入密码：";
		cin >> PassWord;
		int useri = 0;
		if ((useri = CheckUser(UserName, PassWord))!=-1)
		{
			myFileSystem.cur_userid = myFileSystem.reg_users[useri].p_uid;
			user tmp(myFileSystem.reg_users[useri]);
			myFileSystem.users[myFileSystem.cur_userid] = tmp;
			return true;
		}
	}
	return false;
}