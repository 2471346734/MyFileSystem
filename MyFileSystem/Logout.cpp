#include "Head.h"

void Logout()
{
	InitialMemory();
	while (1)
	{
		cout << "ע��[R]\t��¼[L]" << endl;
		char ch;
		cin >> ch;
		if (ch == 'L' || ch == 'l') {
			LogInFileSystem();
			LoadFileSystem();
			break;
		}
		else if (ch == 'R' || ch == 'r')
		{
			Regist();
			//InitialUser();
			LogInFileSystem();
			LoadFileSystem();
			break;
		}
	}
}

void Regist()
{
	int user_num = myFileSystem.reg_users.size()+1;
	fseek(fp, 0, SEEK_SET);
	fwrite(&user_num, 1, sizeof(int), fp);
	user_table user;
	user.p_mode = 01017;
	user.p_uid = myFileSystem.reg_users.size()+1;
	user.sudo = false;
	user.p_gid = 01;
	cout << "===========================" << endl;
	cout << "���������û�����";
	cin >> user.p_name;
	cout << "���������û����룺";
	cin >> user.password;
	fseek(fp, sizeof(int)+sizeof(user_table)*(user_num-1), SEEK_SET);
	fwrite(&user, sizeof(user_table), 1, fp);
	InitialUser();
	cout << "ע��ɹ���" << endl;

}

void ChangeUser()
{

}
