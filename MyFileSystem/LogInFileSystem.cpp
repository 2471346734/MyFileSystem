#include "Head.h"

int CheckUser(string UserName, string PassWord) // �����û��Ƿ��ǳ�ʼ�����û�
{
	for (int i=0;i<myFileSystem.reg_users.size();++i)
	{
		if (myFileSystem.reg_users[i].p_name==UserName&& myFileSystem.reg_users[i].password==PassWord)
		{
			return i;
		}
	}
	cout << "�����û���������������������룡" << endl;
	return -1;
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