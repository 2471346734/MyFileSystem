#include "Head.h"

FileSystem myFileSystem;
FILE* fp;
char read_buffer[10 * BLOCKSIZ+10] = { 0 };//�ļ���ȡ������,10*BLOCKSIZ��Ϊһ��i�ڵ����ָ��10�����ݿ�
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
		if (command == "mkdir")//����Ŀ¼
		{
			cin >> operating;
			if (!NameAccess(operating)) {
				cout << "�ļ����������֡���ĸ���»��ߣ�_������������ַ������޸ĺ����ԣ�" << endl;
				continue;
			}
			mkdir(operating.c_str());
			continue;
		}
		if (command == "help")//����
		{
			HomePage();
			continue;
		}
		if (command == "mkfile")//�����ļ�
		{
			cin >> operating;
			if (!NameAccess(operating)) {
				cout << "�ļ����������֡���ĸ���»��ߣ�_������������ַ������޸ĺ����ԣ�" << endl;
				continue;
			}
			CreateFile(operating.c_str());
			continue;
		}
		if (command == "rmdir")//ɾ��Ŀ¼
		{
			cin >> operating;
			if (!NameAccess(operating)) {
				cout << "�ļ����������֡���ĸ���»��ߣ�_������������ַ������޸ĺ����ԣ�" << endl;
				continue;
			}
			DeleteDir(operating.c_str());
			continue;
		}
		if (command == "rmfile")//ɾ���ļ�
		{
			cin >> operating;
			if (!NameAccess(operating)) {
				cout << "�ļ����������֡���ĸ���»��ߣ�_������������ַ������޸ĺ����ԣ�" << endl;
				continue;
			}
			DeleteFile(operating.c_str());
			continue;
		}
		if (command == "format") {//��ʽ��
			while (1)
			{
				cout << "�Ƿ�Ҫ��ʽ���ļ���[Y/N]" << endl;
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
						cout << "���ѳɹ���ʽ���ļ�ϵͳ��" << endl;
						InitialUser();
						LogInFileSystem();
						LoadFileSystem();
						break;
					}
					else
					{
						cout << "��ʽ��ʧ�ܣ�" << endl;
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
		if (command == "cpfile")//�����ļ�
		{
			cin >> operating;
			cin >> operating2;
			CopyFile(operating, operating2);
			continue;
		}
		if (command == "mvfile")//�ƶ��ļ�
		{
			cin >> operating;
			cin >> operating2;
			MoveFile(operating, operating2);
			continue;
		}
		if (command == "cp")//��������
		{
			cin >> operating;
			cin >> operating2;
			Copy(operating, operating2);
			continue;
		}
		if (command == "rename")//�������ļ�
		{
			cin >> operating;
			cin >> operating2;
			if (!NameAccess(operating) || !NameAccess(operating2)) {
				cout << "�ļ����������֡���ĸ���»��ߣ�_������������ַ������޸ĺ����ԣ�" << endl;
				continue;
			}
			Rename(operating, operating2);
			continue;
		}
		if (command == "cd")//�л�Ŀ¼
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
					cout << "�л�Ŀ¼�ɹ���" << endl;
					continue;
				}
				else {
					cout << "�л�Ŀ¼ʧ�ܣ�" << endl;
					continue;
				}
			}
			
			continue;
		}
		if (command == "chmod") {//�޸��ļ���ģʽ
			cin >> operating;
			cin >> operating2;
			if (!NameAccess(operating)) {
				cout << "�ļ����������֡���ĸ���»��ߣ�_������������ַ������޸ĺ����ԣ�" << endl;
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
		if (command == "open")//���ļ�
		{
			int pos_in_uof = -1;
			cin >> operating;
			cin >> operating2;
			if (!NameAccess(operating)) {
				cout << "�ļ����������֡���ĸ���»��ߣ�_������������ַ������޸ĺ����ԣ�" << endl;
				continue;
			}
			if (operating2 == "-r") {//��ģʽ
				pos_in_uof = open(operating.c_str(), FREAD);
			}
			else if (operating2 == "-w") {//дģʽ
				pos_in_uof = open(operating.c_str(), FWRITE);
			}
			else if (operating2 == "-a") {//׷��ģʽ
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
		if (command == "read")//��ȡ�ļ�
		{
			cin >> operating;
			if (!NameAccess(operating)) {
				cout << "�ļ����������֡���ĸ���»��ߣ�_������������ַ������޸ĺ����ԣ�" << endl;
				continue;
			}
			memset(read_buffer, 0, sizeof(read_buffer));
			int fid = search_uof(operating.c_str());
			if (fid != -1) {
				int sys_otpos = myFileSystem.users[myFileSystem.cur_userid].u_ofile[fid];					//��ȡ��ǰ�ļ���ϵͳ�ļ��򿪱��е�λ��
				int temp_file_size = myFileSystem.sys_ofile[sys_otpos].f_inode->di_size;	//��ȡ��ǰ�ļ���С
				read(fid, read_buffer, temp_file_size);
				int cnt = 0;
				while (((read_buffer[cnt] >= '0' && read_buffer[cnt] <= '9') || (read_buffer[cnt] >= 'a' && read_buffer[cnt] <= 'z') || (read_buffer[cnt] >= 'A' && read_buffer[cnt] <= 'Z')))
					cnt++;
				read_buffer[cnt] = '\0';
				printf("\n>Content of the file: %s\n", read_buffer);									//����ļ�����
			}
			else {
				printf(">Cannot read a file that is not open or not exixted in current directory\n");
			}
			continue;
		}
		if (command == "write")//д�ļ�
		{
			cin >> operating;
			if (!NameAccess(operating)) {
				cout << "�ļ����������֡���ĸ���»��ߣ�_������������ַ������޸ĺ����ԣ�" << endl;
				continue;
			}
			char write_buffer[10 * BLOCKSIZ];//�ļ�д�뻺����,10*BLOCKSIZ��Ϊһ��i�ڵ����ָ��10�����ݿ�
			memset(write_buffer, 0, sizeof(write_buffer));
			int fid = search_uof(operating.c_str());
			if (fid != -1)
			{//�ļ������ұ���
				int sys_otpos = myFileSystem.users[myFileSystem.cur_userid].u_ofile[fid];					//��ȡ��ǰ�ļ���ϵͳ�ļ��򿪱��е�λ��
				int temp_file_size = myFileSystem.sys_ofile[sys_otpos].f_inode->di_size;	//��ȡ��ǰ�ļ���С
				printf(">Write file [%s],please input:\n", operating.c_str());			//�����ʾ��Ϣ
				scanf("%s", write_buffer);									//��ȡ�û�д��
				int ssize = 0;
				while (write_buffer[ssize] != '\0')
					ssize++;
				write(fid, write_buffer, ssize);				//���ļ����ݶ��뻺����
			}
			else
				printf(">Cannot write a file that is not open or not exixted in current directory\n");
			continue;
		}
		if (command == "close")//�ر��ļ�
		{
			cin >> operating;
			if (!NameAccess(operating)) {
				cout << "�ļ����������֡���ĸ���»��ߣ�_������������ַ������޸ĺ����ԣ�" << endl;
				continue;
			}
			int fid = search_uof(operating.c_str());
			if (fid != -1)
			{//�ļ������ұ���
				CloseFile(fid);				//���ļ����ݶ��뻺����
			}
			else
				printf(">Cannot close a file that is not open or not exixted in current directory\n");
			continue;
		}
		if (command == "ls")//��ʾ��ǰĿ¼
		{
			_dir();
			continue;
		}
		if (command == "halt")//�ػ�
		{
			while (1)
			{
				cout << "�Ƿ�Ҫ�˳��ļ�ϵͳ��[Y/N]" << endl;
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
		
		if (command == "logout")//�ǳ��û�
		{
			while (1)
			{
				cout << "�Ƿ�Ҫע����ǰ�û���[Y/N]" << endl;
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