#include "Head.h"

bool CheckUser(string UserName, string PassWord) // 检查该用户是否是初始化的用户
{
	for (int i = 0;i<USERNUM;i++)
	{
		if (myFileSystem.UserInfo[i].UserName == UserName && myFileSystem.UserInfo[i].PassWord == PassWord)
		{
			return true;
		}
	}
	cout << "您的用户名或密码错误！请重新输入！" << endl;
	return false;
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
		if (CheckUser(UserName, PassWord))
		{
			myFileSystem.CurrentUser.UserName = UserName;
			myFileSystem.CurrentUser.PassWord = PassWord;
			return true;
		}
	}
	return false;
}