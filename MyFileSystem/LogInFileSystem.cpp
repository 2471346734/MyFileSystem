#include "Head.h"

bool CheckUser(string UserName, string PassWord) // �����û��Ƿ��ǳ�ʼ�����û�
{
	for (int i = 0;i<USERNUM;i++)
	{
		if (myFileSystem.UserInfo[i].UserName == UserName && myFileSystem.UserInfo[i].PassWord == PassWord)
		{
			return true;
		}
	}
	cout << "�����û���������������������룡" << endl;
	return false;
}

bool LogInFileSystem()
{
	string UserName, PassWord;
	while (1)
	{
		cout << "�������û���:";
		cin >> UserName;
		cout << "���������룺";
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