#include "Head.h"

FileSystem myFileSystem;
FILE* fp;
char read_buffer[10 * BLOCKSIZ+10] = { 0 };//文件读取缓冲区,10*BLOCKSIZ因为一个i节点最多指向10个数据块
void command()
{
	string cur_user = myFileSystem.users[myFileSystem.cur_userid].u_name;
	string cur_dir_name;
	if (!myFileSystem.users[myFileSystem.cur_userid].path.empty()) {
		for (string s : myFileSystem.users[myFileSystem.cur_userid].path) {
			cur_dir_name = cur_dir_name + "/";
			cur_dir_name = cur_dir_name + s;
		}
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
		if (command == "mkdir")//创建目录
		{
			cin >> operating;
			if (!NameAccess(operating)) {
				cout << "文件名含有数字、字母、下划线（_）以外的其他字符，请修改后重试！" << endl;
				continue;
			}
			mkdir(operating.c_str());
			continue;
		}
		if (command == "help")//帮助
		{
			HomePage();
			continue;
		}
		if (command == "mkfile")//创建文件
		{
			cin >> operating;
			if (!NameAccess(operating)) {
				cout << "文件名含有数字、字母、下划线（_）以外的其他字符，请修改后重试！" << endl;
				continue;
			}
			CreateFile(operating.c_str());
			continue;
		}
		if (command == "rmdir")//删除目录
		{
			cin >> operating;
			if (!NameAccess(operating)) {
				cout << "文件名含有数字、字母、下划线（_）以外的其他字符，请修改后重试！" << endl;
				continue;
			}
			DeleteDir(operating.c_str());
			continue;
		}
		if (command == "rmfile")//删除文件
		{
			cin >> operating;
			if (!NameAccess(operating)) {
				cout << "文件名含有数字、字母、下划线（_）以外的其他字符，请修改后重试！" << endl;
				continue;
			}
			DeleteFile(operating.c_str());
			continue;
		}
		if (command == "format") {//格式化
			while (1)
			{
				cout << "是否要格式化文件卷？[Y/N]" << endl;
				char ch;
				cin >> ch;
				if (ch == 'y' || ch == 'Y') {
					int i, j;
					if (!myFileSystem.Hinode.empty()) {
						map<int, Inode*>::iterator iter;
						iter = myFileSystem.Hinode.begin();
						while (iter != myFileSystem.Hinode.end()) {
							Inode* inode = iter->second;
							long addr;
							unsigned int block_num;
							if (inode->di_number != 0)
							{
								/*write back the inode */
								addr = DINODESTART + inode->i_ino * DINODESIZ;
								fseek(fp, addr, SEEK_SET);
								fwrite(&inode->di_number, DINODESIZ, 1, fp);
							}
							else
							{
								/*	rm the inode & the block of the file in the disk */
								block_num = (unsigned int)inode->di_size / BLOCKSIZ;
								for (i = 0; i < block_num; i++)
								{
									bfree(inode->di_addr[i]);
								}
								ifree(inode->i_ino);
							}
							/*	free the inode in the memory */
							//myFileSystem.Hinode.erase(inode->i_ino);
							iter++;
						}
					}
					if (InitialFileSystem() == OK) {
						cout << "您已成功格式化文件系统！" << endl;
						InitialUser();
						LogInFileSystem();
						LoadFileSystem();
						break;
					}
					else
					{
						cout << "格式化失败！" << endl;
						exit(1);
					}
				}
				else if (ch == 'n' || ch == 'N')
				{
					break;
				}
			}
			continue;
		}
		if (command == "cpfile")//复制文件
		{
			cin >> operating;
			cin >> operating2;
			CopyFile(operating, operating2);
			continue;
		}
		if (command == "mvfile")//移动文件
		{
			cin >> operating;
			cin >> operating2;
			MoveFile(operating, operating2);
			continue;
		}
		if (command == "cp")//复制引用
		{
			cin >> operating;
			cin >> operating2;
			Copy(operating, operating2);
			continue;
		}
		if (command == "rename")//重命名文件
		{
			cin >> operating;
			cin >> operating2;
			if (!NameAccess(operating) || !NameAccess(operating2)) {
				cout << "文件名含有数字、字母、下划线（_）以外的其他字符，请修改后重试！" << endl;
				continue;
			}
			Rename(operating, operating2);
			continue;
		}
		if (command == "cd")//切换目录
		{
			cin >> operating;
			if (operating == ".") {
				continue;
			}
			else if (operating == "..") {
				chdir("..");
				continue;
			}
			else {
				if (ChangeDir(operating) == 1) {
					cout << "切换目录成功！" << endl;
					continue;
				}
				else {
					cout << "切换目录失败！" << endl;
					continue;
				}
			}
			
			continue;
		}
		if (command == "chmod") {//修改文件打开模式
			cin >> operating;
			cin >> operating2;
			if (!NameAccess(operating)) {
				cout << "文件名含有数字、字母、下划线（_）以外的其他字符，请修改后重试！" << endl;
				continue;
			}
			int fid = search_uof(operating);
			if (fid != -1) {
				if (operating2 == "-r") {
					Chmod(fid, FREAD);
				}
				else if (operating2 == "-w") {
					Chmod(fid, FWRITE);
				}
				else if (operating2 == "-a") {
					Chmod(fid, FAPPEND);
				}
			}
			else {
				printf(">Cannot read a file that is not open or not exixted in current directory\n");
			}
			continue;
		}
		if (command == "open")//打开文件
		{
			int pos_in_uof = -1;
			cin >> operating;
			cin >> operating2;
			if (!NameAccess(operating)) {
				cout << "文件名含有数字、字母、下划线（_）以外的其他字符，请修改后重试！" << endl;
				continue;
			}
			if (operating2 == "-r") {//读模式
				pos_in_uof = open(operating.c_str(), FREAD);
			}
			else if (operating2 == "-w") {//写模式
				pos_in_uof = open(operating.c_str(), FWRITE);
			}
			else if (operating2 == "-a") {//追加模式
				pos_in_uof = open(operating.c_str(), FAPPEND);
			}
			if (pos_in_uof == NOT_FOUND) {
				cout << "file open unsuccess!" << endl;
				continue;
			}
			Inode *temp_inode = iget(myFileSystem.sys_ofile[pos_in_uof].f_inode->i_ino);
			if (temp_inode->i_ino != 0)printf(">file open success!\n");
			iput(temp_inode);
			continue;
		}
		if (command == "read")//读取文件
		{
			cin >> operating;
			if (!NameAccess(operating)) {
				cout << "文件名含有数字、字母、下划线（_）以外的其他字符，请修改后重试！" << endl;
				continue;
			}
			memset(read_buffer, 0, sizeof(read_buffer));
			int fid = search_uof(operating.c_str());
			if (fid != -1) {
				int sys_otpos = myFileSystem.users[myFileSystem.cur_userid].u_ofile[fid];					//获取当前文件在系统文件打开表中的位置
				int temp_file_size = myFileSystem.sys_ofile[sys_otpos].f_inode->di_size;	//获取当前文件大小
				read(fid, read_buffer, temp_file_size);
				int cnt = 0;
				while (((read_buffer[cnt] >= '0' && read_buffer[cnt] <= '9') || (read_buffer[cnt] >= 'a' && read_buffer[cnt] <= 'z') || (read_buffer[cnt] >= 'A' && read_buffer[cnt] <= 'Z')))
					cnt++;
				read_buffer[cnt] = '\0';
				printf("\n>Content of the file: %s\n", read_buffer);									//输出文件内容
			}
			else {
				printf(">Cannot read a file that is not open or not exixted in current directory\n");
			}
			continue;
		}
		if (command == "write")//写文件
		{
			cin >> operating;
			if (!NameAccess(operating)) {
				cout << "文件名含有数字、字母、下划线（_）以外的其他字符，请修改后重试！" << endl;
				continue;
			}
			char write_buffer[10 * BLOCKSIZ];//文件写入缓冲区,10*BLOCKSIZ因为一个i节点最多指向10个数据块
			memset(write_buffer, 0, sizeof(write_buffer));
			int fid = search_uof(operating.c_str());
			if (fid != -1)
			{//文件存在且被打开
				int sys_otpos = myFileSystem.users[myFileSystem.cur_userid].u_ofile[fid];					//获取当前文件在系统文件打开表中的位置
				int temp_file_size = myFileSystem.sys_ofile[sys_otpos].f_inode->di_size;	//获取当前文件大小
				printf(">Write file [%s],please input:\n", operating.c_str());			//输出提示信息
				scanf("%s", write_buffer);									//获取用户写入
				int ssize = 0;
				while (write_buffer[ssize] != '\0')
					ssize++;
				write(fid, write_buffer, ssize);				//将文件内容读入缓冲区
			}
			else
				printf(">Cannot write a file that is not open or not exixted in current directory\n");
			continue;
		}
		if (command == "close")//关闭文件
		{
			cin >> operating;
			if (!NameAccess(operating)) {
				cout << "文件名含有数字、字母、下划线（_）以外的其他字符，请修改后重试！" << endl;
				continue;
			}
			int fid = search_uof(operating.c_str());
			if (fid != -1)
			{//文件存在且被打开
				CloseFile(fid);				//将文件内容读入缓冲区
			}
			else
				printf(">Cannot close a file that is not open or not exixted in current directory\n");
			continue;
		}
		if (command == "ls")//显示当前目录
		{
			_dir();
			continue;
		}
		if (command == "halt")//关机
		{
			while (1)
			{
				cout << "是否要退出文件系统？[Y/N]" << endl;
				char ch;
				cin >> ch;
				if (ch == 'y' || ch == 'Y') {
					halt();
				}
				else if (ch == 'n' || ch == 'N')
				{
					break;
				}
			}
			
			continue;
		}
		
		if (command == "logout")//登出用户
		{
			while (1)
			{
				cout << "是否要注销当前用户？[Y/N]" << endl;
				char ch;
				cin >> ch;
				if (ch == 'y' || ch == 'Y') {
					Logout();
					break;
				}
				else if (ch == 'n' || ch == 'N')
				{
					break;
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