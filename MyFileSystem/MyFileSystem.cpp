#include "Head.h"

FileSystem myFileSystem;
FILE* fp;
void command()
{
	/*stack<int> command;
	director myDirector = myFileSystem.SystemDirector[myFileSystem.DirectorIndex];//���浱ǰĿ¼��
	//������ǰĿ¼�ĸ�Ŀ¼��ѹջ
	while (myDirector.ParentDirector != -1)
	{
		command.push(myDirector.DirectorId);
		myDirector = myFileSystem.SystemDirector[myDirector.ParentDirector];
	}
	command.push(myDirector.DirectorId);
	cout << myFileSystem.CurrentUser.UserName << "@FileSystem:~$ ";
	while (!command.empty())
	{
		cout << myFileSystem.SystemDirector[command.top()].DirectorName << ">";
		command.pop();
	}*/

	string cur_user = myFileSystem.users[myFileSystem.cur_userid].u_name;
	string cur_dir_name;
	for (string s : myFileSystem.users[myFileSystem.cur_userid].path) {
		cur_dir_name = cur_dir_name + "/";
		cur_dir_name = cur_dir_name + s;
	}
	string cmd = cur_user + "@/" + cur_dir_name + ">";
	cout << cmd;
}

int main()
{
	if (!access("FileSystem", 0)) {//�ļ������
		if ((fp = fopen("FileSystem", "r+b")) != nullptr) {
			cout << "�ļ���򿪳ɹ���" << endl;
		}
		else
		{
			cout << "�ļ����ʧ�ܣ�" << endl;
			exit(1);
		}
	}
	else//�ļ�������
	{
		
		while (1)
		{
			cout << "�ļ������ڣ��Ƿ񴴽��ļ�����ʽ����[Y/N]" << endl;
			char ch;
			cin >> ch;
			if (ch == 'y' || ch == 'Y') {
				if (InitialFileSystem() == OK) {
					cout << "���ѳɹ���ʼ���ļ�ϵͳ��" << endl;
					break;
				}
				else
				{
					cout << "��ʼ��ʧ�ܣ�" << endl;
					exit(1);
				}
			}
			else if(ch == 'n' || ch == 'N')
			{
				cout << "���˳�ϵͳ��" << endl;
				exit(0);
			}
		}
	}
	InitialUser();
	cout << "*****************��ӭʹ���ļ�ϵͳ****************" << endl;
	if (LogInFileSystem())
	{
		cout << "���ѳɹ���¼��" << endl<<endl;
	}
	LoadFileSystem();
	HomePage();
	//cin.ignore();
	while (1)
	{
		command();
		string command;
		cin >> command;
		string operating, operating2;
		if (command == "mkdir")
		{
			cin >> operating;
			mkdir(operating.c_str());
			continue;
		}
		if (command == "help")
		{
			HomePage();
			continue;
		}
		if (command == "mkfile")
		{
			cin >> operating;
			//CreateFile(operating);
			continue;
		}
		if (command == "rmdir")
		{
			cin >> operating;
			//DeleteDirector(operating);
			continue;
		}
		if (command == "rmfile")
		{
			cin >> operating;
			//DeleteFile(operating);
			continue;
		}
		/*if (command == "cpdir")
		{
			cin >> operating;
			cin >> operating2;
			CopyDirector(operating, operating2);
			continue;
		}*/
		/*if (command == "cpfile")
		{
			cin >> operating;
			cin >> operating2;
			CopyFile(operating, operating2);
			continue;
		}*/
		/*if (command == "mvdir")
		{
			cin >> operating;
			cin >> operating2;
			CutDirector(operating, operating2);
			continue;
		}*/
		/*if (command == "mvfile")
		{
			cin >> operating;
			cin >> operating2;
			CutFile(operating, operating2);
			continue;
		}*/
		/*if (command == "rndir")
		{
			cin >> operating;
			cin >> operating2;
			RenameDirector(operating, operating2);
			continue;
		}*/
		/*if (command == "rnfile")
		{
			cin >> operating;
			cin >> operating2;
			RenameFile(operating, operating2);
			continue;
		}*/
		/*if (command == "fddir")
		{
			cin >> operating;
			if (SearchDirector(operating) != -1)
				cout << "��Ŀ¼�����ڵ�ǰ�ļ��У�" << endl;
			else
				cout << "��Ŀ¼�������ڵ�ǰ�ļ��У�" << endl;
			continue;
		}*/
		/*if (command == "fdfile")
		{
			cin >> operating;
			if (SearchFile(operating) != -1)
				cout << "���ļ������ڵ�ǰ�ļ��У�" << endl;
			else
				cout << "���ļ��������ڵ�ǰ�ļ��У�" << endl;
			continue;
		}*/
		if (command == "cd")
		{
			cin >> operating;
			chdir(operating.c_str());
			continue;
		}
		/*if (command == "rndir")
		{
			cin >> operating;
			cin >> operating2;
			RenameDirector(operating, operating2);
			continue;
		}*/
		/*if (command == "rnfile")
		{
			cin >> operating;
			cin >> operating2;
			RenameFile(operating, operating2);
			continue;
		}*/
		/*if (command == "ofile")
		{
			cin >> operating;
			OpenFileFunc(operating);
			continue;
		}*/
		/*if (command == "rfile")
		{
			cin >> operating;
			ReadFile(operating);
			continue;
		}*/
		/*if (command == "wfile")
		{
			cin >> operating;
			cin >> operating2;
			WriteFile(operating, operating2);
			continue;
		}*/
		/*if (command == "cfile")
		{
			cin >> operating;
			CloseFile(operating);
			continue;
		}*/
		if (command == "ls")
		{
			_dir();
			continue;
		}
		/*if (command == "save")
		{
			SaveFileSystem();
			continue;
		}
		if (command == "format")
		{
			InitialFileSystem();
			continue;
		}
		if (command == "logout")
		{
			char ch;
			cout << "�Ƿ񱣴��ļ�ϵͳ Y/N" << endl;
			cin >> ch;
			if (ch == 'Y' || ch == 'y')
			{
				SaveFileSystem();
				break;
			}
			else
			{
				if (ch != 'N' || ch != 'n')
				{
					cout << "���������봦����Y/N" << endl;
				}
			}
		}
		else
		{
			cout << "����ָ�����������" << endl;
		}
		while (1)    // ���˵�ʣ��ָ��
		{
			char ch;
			ch = getchar();
			if (ch == '\n')
			{
				break;
			}
		}*/
	}
	system("pause");
	return 0;
}