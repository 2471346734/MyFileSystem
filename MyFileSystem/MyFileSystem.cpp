#include "Head.h"

FileSystem myFileSystem;

void command()
{
	stack<int> command;
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
	}
}

int main()
{
	InitialUser();
	cout << "*****************��ӭʹ���ļ�ϵͳ****************" << endl;
	if (LogInFileSystem())
	{
		cout << "���ѳɹ���¼��" << endl<<endl;
	}
	while (1)
	{
		cout << "�Ƿ��ʼ���ļ�ϵͳ Y/N" << endl;
		char ch;
		cin >> ch;
		if (ch == 'Y' || ch == 'y')
		{
			InitialFileSystem();
			cout << "���ѳɹ���ʼ���ļ�ϵͳ" << endl<<endl;
			break;
		}
		else
		{
			if (ch == 'N' || ch == 'n')
			{
				LoadFileSystem();
				cout << "���ѳɹ������ļ�ϵͳ" << endl<<endl;
				break;
			}
			else
			{
				cout << "���������봦����Y/N" << endl;

			}
		}
	}
	HomePage();
	while (1)
	{
		command();
		string command;
		cin >> command;
		string operating, operating2;
		if (command == "mkdir")
		{
			cin >> operating;
			MakeDirector(operating);
			continue;
		}
		if (command == "mkfile")
		{
			cin >> operating;
			CreateFile(operating);
			continue;
		}
		if (command == "rmdir")
		{
			cin >> operating;
			DeleteDirector(operating);
			continue;
		}
		if (command == "rmfile")
		{
			cin >> operating;
			DeleteFile(operating);
			continue;
		}
		if (command == "cpdir")
		{
			cin >> operating;
			cin >> operating2;
			CopyDirector(operating, operating2);
			continue;
		}
		if (command == "cpfile")
		{
			cin >> operating;
			cin >> operating2;
			CopyFile(operating, operating2);
			continue;
		}
		if (command == "mvdir")
		{
			cin >> operating;
			cin >> operating2;
			CutDirector(operating, operating2);
			continue;
		}
		if (command == "mvfile")
		{
			cin >> operating;
			cin >> operating2;
			CutFile(operating, operating2);
			continue;
		}
		if (command == "rndir")
		{
			cin >> operating;
			cin >> operating2;
			RenameDirector(operating, operating2);
			continue;
		}
		if (command == "rnfile")
		{
			cin >> operating;
			cin >> operating2;
			RenameFile(operating, operating2);
			continue;
		}
		if (command == "fddir")
		{
			cin >> operating;
			if (SearchDirector(operating) != -1)
				cout << "��Ŀ¼�����ڵ�ǰ�ļ��У�" << endl;
			else
				cout << "��Ŀ¼�������ڵ�ǰ�ļ��У�" << endl;
			continue;
		}
		if (command == "fdfile")
		{
			cin >> operating;
			if (SearchFile(operating) != -1)
				cout << "���ļ������ڵ�ǰ�ļ��У�" << endl;
			else
				cout << "���ļ��������ڵ�ǰ�ļ��У�" << endl;
			continue;
		}
		if (command == "cd")
		{
			cin >> operating;
			EnterLowDir(operating);
			continue;
		}
		if (command == "cd..")
		{
			ReturnToParentDir();
			continue;
		}
		if (command == "rndir")
		{
			cin >> operating;
			cin >> operating2;
			RenameDirector(operating, operating2);
			continue;
		}
		if (command == "rnfile")
		{
			cin >> operating;
			cin >> operating2;
			RenameFile(operating, operating2);
			continue;
		}
		if (command == "ofile")
		{
			cin >> operating;
			OpenFileFunc(operating);
			continue;
		}
		if (command == "rfile")
		{
			cin >> operating;
			ReadFile(operating);
			continue;
		}
		if (command == "wfile")
		{
			cin >> operating;
			cin >> operating2;
			WriteFile(operating, operating2);
			continue;
		}
		if (command == "cfile")
		{
			cin >> operating;
			CloseFile(operating);
			continue;
		}
		if (command == "ls")
		{
			ShowDirector();
			continue;
		}
		if (command == "save")
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
		}
	}
	system("pause");
	return 0;
}