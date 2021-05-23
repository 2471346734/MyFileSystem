#include "Head.h"

FileSystem myFileSystem;

void command()
{
	stack<int> command;
	director myDirector = myFileSystem.SystemDirector[myFileSystem.DirectorIndex];//保存当前目录号
	//遍历当前目录的父目录，压栈
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
	cout << "*****************欢迎使用文件系统****************" << endl;
	if (LogInFileSystem())
	{
		cout << "您已成功登录！" << endl<<endl;
	}
	while (1)
	{
		cout << "是否初始化文件系统 Y/N" << endl;
		char ch;
		cin >> ch;
		if (ch == 'Y' || ch == 'y')
		{
			InitialFileSystem();
			cout << "您已成功初始化文件系统" << endl<<endl;
			break;
		}
		else
		{
			if (ch == 'N' || ch == 'n')
			{
				LoadFileSystem();
				cout << "您已成功载入文件系统" << endl<<endl;
				break;
			}
			else
			{
				cout << "输入有误，请处输入Y/N" << endl;

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
				cout << "该目录存在于当前文件夹！" << endl;
			else
				cout << "该目录不存在于当前文件夹！" << endl;
			continue;
		}
		if (command == "fdfile")
		{
			cin >> operating;
			if (SearchFile(operating) != -1)
				cout << "该文件存在于当前文件夹！" << endl;
			else
				cout << "该文件不存在于当前文件夹！" << endl;
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
			cout << "是否保存文件系统 Y/N" << endl;
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
					cout << "输入有误，请处输入Y/N" << endl;
				}
			}
		}
		else
		{
			cout << "错误指令，请重新输入" << endl;
		}
		while (1)    // 过滤掉剩余指令
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