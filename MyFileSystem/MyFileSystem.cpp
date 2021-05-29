#include "Head.h"

FileSystem myFileSystem;
FILE* fp;
void command()
{
	/*stack<int> command;
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
	if (!access("FileSystem", 0)) {//文件卷存在
		if ((fp = fopen("FileSystem", "r+b")) != nullptr) {
			cout << "文件卷打开成功！" << endl;
		}
		else
		{
			cout << "文件卷打开失败！" << endl;
			exit(1);
		}
	}
	else//文件卷不存在
	{
		
		while (1)
		{
			cout << "文件卷不存在，是否创建文件卷并格式化？[Y/N]" << endl;
			char ch;
			cin >> ch;
			if (ch == 'y' || ch == 'Y') {
				if (InitialFileSystem() == OK) {
					cout << "您已成功初始化文件系统！" << endl;
					break;
				}
				else
				{
					cout << "初始化失败！" << endl;
					exit(1);
				}
			}
			else if(ch == 'n' || ch == 'N')
			{
				cout << "已退出系统！" << endl;
				exit(0);
			}
		}
	}
	InitialUser();
	cout << "*****************欢迎使用文件系统****************" << endl;
	if (LogInFileSystem())
	{
		cout << "您已成功登录！" << endl<<endl;
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
				cout << "该目录存在于当前文件夹！" << endl;
			else
				cout << "该目录不存在于当前文件夹！" << endl;
			continue;
		}*/
		/*if (command == "fdfile")
		{
			cin >> operating;
			if (SearchFile(operating) != -1)
				cout << "该文件存在于当前文件夹！" << endl;
			else
				cout << "该文件不存在于当前文件夹！" << endl;
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
		}*/
	}
	system("pause");
	return 0;
}