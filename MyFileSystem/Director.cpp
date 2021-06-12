#include "Head.h"

void _dir()	/* _dir */
{
	int i;
	Inode * temp_inode;
	cout << "空闲磁盘i节点数：" << myFileSystem.superblock.s_ninode << "/" << DINODEBLK*BLOCKSIZ/ DINODESIZ << endl;
	cout << "空闲数据块数：" << myFileSystem.superblock.s_nfree << "/" << FILEBLK << endl;
	printf("CURRENT DIRECTORY\n");
	Dir cur_dir = myFileSystem.users[myFileSystem.cur_userid].cur_dir;
	for (i = 0; i < cur_dir.size; i++)
	{
		if (cur_dir.direct[i].d_ino != DIEMPTY)//目录项非空
		{
			printf("%.*s\t", DIRSIZ, cur_dir.direct[i].d_name);
			temp_inode = iget(cur_dir.direct[i].d_ino);
			if (temp_inode->di_mode & DIFILE)//目录项为文件
				printf("f\t");
			else							//目录项为目录
				printf("d\t");
			//显示权限
			/*int mode = temp_inode->di_mode;
			for (int i = 0; i < 3; ++i) {
				for (int j = 0; j < 3; ++j) {
					if()
				}
			}*/
			if (temp_inode->di_mode & DIFILE)
			{
				printf("%d\t", temp_inode->di_size);
				printf("inode number:%d\t", cur_dir.direct[i].d_ino);
				printf("block chain:");
				for (int i = 0; i < NADDR; i++)
				{
					if (temp_inode->di_addr[i] > 0) {
						printf("%d ", temp_inode->di_addr[i]);
					}
					
				}
				printf("\n");
			}
			else
				printf("<dir>\tinode number:%d\tblock chain:%d\n", cur_dir.direct[i].d_ino,temp_inode->di_addr[0]);
			iput(temp_inode);
		}
	}
}

void mkdir(const char *dirname)	/* mkdir */
{

	int dirid, dirpos;
	Inode * inode;
	Dir* cur_dir = &myFileSystem.users[myFileSystem.cur_userid].cur_dir;
	Direct buf[BLOCKSIZ / DIRITEM];
	unsigned int block;
	memset(buf, 0, (BLOCKSIZ / DIRITEM) * sizeof(Direct));
	dirid = namei(dirname);
	if (dirid != NOT_FOUND)
	{
		inode = iget(dirid);
		if (inode->di_mode & DIDIR)
			printf("\n%s directory already existed! ! \n", dirname);
		else
			printf("\n%s is a file name, &can't create a dir the same name\n", dirname);
		iput(inode);
		return;
	}
	dirpos = iname(dirname);
	if (dirpos == NOT_FOUND) {
		return;
	}
	if (myFileSystem.superblock.s_nfree == 0 || myFileSystem.superblock.s_ninode == 0) {
		cout << "i节点或数据块为空！" << endl;
		return;
	}
	inode = ialloc();
	cur_dir->direct[dirpos].d_ino = inode->i_ino;
	cur_dir->size++;
	/*	fill the new dir buf */
	strcpy(buf[0].d_name, "..");
	buf[0].d_ino = myFileSystem.cur_path_inode->i_ino;
	strcpy(buf[1].d_name, ".");
	buf[1].d_ino = inode->i_ino;
	buf[2].d_ino = 0;
	block = balloc();

	fseek(fp, DATASTART + block * BLOCKSIZ, SEEK_SET);
	fwrite(buf, 1, BLOCKSIZ, fp);

	inode->di_size = 2 * DIRITEM;
	inode->di_number = 1;
	inode->di_mode = DEFAULTMODE | DIDIR;
	inode->di_uid = myFileSystem.users[myFileSystem.cur_userid].u_uid;
	inode->di_gid = myFileSystem.users[myFileSystem.cur_userid].u_gid;
	inode->di_addr[0] = block;
	iput(inode);
	cout << "目录创建成功！" << endl;
	return;
}

bool DeleteDir(const char *filename)
{
	unsigned int dinodeid;
	Inode *inode;
	int i;
	dinodeid = namei(filename);
	if (dinodeid != 0)
		inode = iget(dinodeid);
	else
	{
		cout << "no such a dir!" << endl;
		return false;
	}
	if ((inode->di_mode & DIFILE) != 0) {
		cout << "no such a dir!" << endl;
		return false;
	}
	if (!access(inode, WRITE))    /* access denied */
	{
		printf(">failed to remove dir because of no authority!\n");
		return false;
	}
	if (inode->di_size > (2 * DIRITEM)) {
		cout << "dir is not empty!" << endl;
		return false;
	}
	inode->di_number--;
	for (i = 0; i < myFileSystem.users[myFileSystem.cur_userid].cur_dir.size; i++)
	{
		if (myFileSystem.users[myFileSystem.cur_userid].cur_dir.direct[i].d_ino == dinodeid)
			break;
	}
	i++;
	while (myFileSystem.users[myFileSystem.cur_userid].cur_dir.direct[i].d_ino != 0)
	{
		strcpy(myFileSystem.users[myFileSystem.cur_userid].cur_dir.direct[i - 1].d_name, myFileSystem.users[myFileSystem.cur_userid].cur_dir.direct[0].d_name);
		myFileSystem.users[myFileSystem.cur_userid].cur_dir.direct[i - 1].d_ino = myFileSystem.users[myFileSystem.cur_userid].cur_dir.direct[i].d_ino;
		i++;
	}
	myFileSystem.users[myFileSystem.cur_userid].cur_dir.direct[i - 1].d_ino = 0;
	myFileSystem.users[myFileSystem.cur_userid].cur_dir.size = i - 1;
	iput(inode);
	return true;
}

void save_cur_dir()
{
	int i, block;
	//	write back the current directory 
	myFileSystem.cur_path_inode->di_size = myFileSystem.users[myFileSystem.cur_userid].cur_dir.size*DIRITEM;
	for (i = 0; i < myFileSystem.cur_path_inode->di_size / BLOCKSIZ + 1; i++)
	{
		bfree(myFileSystem.cur_path_inode->di_addr[i]);
	}
	for (i = 0; i < myFileSystem.users[myFileSystem.cur_userid].cur_dir.size; i += BLOCKSIZ / DIRITEM)
	{
		block = balloc();
		myFileSystem.cur_path_inode->di_addr[i] = block;
		fseek(fp, DATASTART + block * BLOCKSIZ, SEEK_SET);
		fwrite(&myFileSystem.users[myFileSystem.cur_userid].cur_dir.direct[i], 1, BLOCKSIZ, fp);
	}
	iput(myFileSystem.cur_path_inode);
}


bool ChangeDir(string path)
{
	if (path.size() == 0)return 0;
	if (path[0] == '/') {			//绝对路径
		vector<string> tmp_path = myFileSystem.users[myFileSystem.cur_userid].path;
		Dir tmp_dir = myFileSystem.users[myFileSystem.cur_userid].cur_dir;
		myFileSystem.users[myFileSystem.cur_userid].path.clear();
		if (!chdir("/")) {
			myFileSystem.users[myFileSystem.cur_userid].cur_dir = tmp_dir;
			myFileSystem.users[myFileSystem.cur_userid].path = tmp_path;
			return 0;
		}
		queue<string> path_q;
		path = path.substr(1, path.size() - 1);
		int l = 0;
		for (int i = 0; i < (int)path.size()-1; ++i) {
			if (path[i] == '/') {
				path_q.push(path.substr(l, i - l));
				l = i + 1;
			}
		}
		if (path.size() > 0) {
			if (path[path.size() - 1] == '/') {
				path_q.push(path.substr(l, path.size() - 1 - l));
			}
			else {
				path_q.push(path.substr(l, path.size() - l));
			}
		}
		

		while (!path_q.empty()) {
			string next_p = path_q.front();
			path_q.pop();
			if (!chdir(next_p.c_str())) {
				myFileSystem.users[myFileSystem.cur_userid].cur_dir = tmp_dir;
				myFileSystem.users[myFileSystem.cur_userid].path = tmp_path;
				return 0;
			}
		}
		return 1;

	}
	else if (path[0] == '.') {//相对路径
		vector<string> tmp_path = myFileSystem.users[myFileSystem.cur_userid].path;
		Dir tmp_dir = myFileSystem.users[myFileSystem.cur_userid].cur_dir;
		//myFileSystem.users[myFileSystem.cur_userid].path.clear();
		queue<string> path_q;
		path = path.substr(2, path.size() - 2);
		int l = 0;
		for (int i = 0; i < (int)path.size() - 1; ++i) {
			if (path[i] == '/') {
				path_q.push(path.substr(l, i - l));
				l = i + 1;
			}
		}
		if (path.size() > 0) {
			if (path[path.size() - 1] == '/') {
				path_q.push(path.substr(l, path.size() - 1 - l));
			}
			else {
				path_q.push(path.substr(l, path.size() - l));
			}
		}


		while (!path_q.empty()) {
			string next_p = path_q.front();
			path_q.pop();
			if (!chdir(next_p.c_str())) {
				myFileSystem.users[myFileSystem.cur_userid].cur_dir = tmp_dir;
				myFileSystem.users[myFileSystem.cur_userid].path = tmp_path;
				return 0;
			}
		}
		return 1;
	}
	else {						//当前目录
		return chdir(path.c_str());
	}
}



bool chdir(const char * dirname) 
{
	
	int dirid, i, j;
	Inode * inode;
	unsigned short block;
	
		
	if (!strcmp(dirname, "/"))
	{
		dirid = 1;
	}
	else
	{
		dirid = namei(dirname);

		if (dirid == NOT_FOUND)
		{
			printf("\n%s does not existed\n", dirname);
			return false;
		}
	}

	inode = iget(dirid);
	if (inode->di_mode & DIFILE)
	{//找到的是文件
		printf("\ncannot use the command on file!\n");
		iput(inode);
		return false;
	}
	if (!access(inode, myFileSystem.users[myFileSystem.cur_userid].u_default_mode))
	{
		printf("\nhas not access to the directory %s", dirname);
		iput(inode);
		return false;
	}
	//对.和..特殊处理
	if (myFileSystem.cur_path_inode->i_ino == 1) {
		if (strcmp(dirname, ".") && strcmp(dirname, "..") && strcmp(dirname, "/")) {
			myFileSystem.users[myFileSystem.cur_userid].path.push_back(dirname);
		}
	}
	else
	{
		if (!strcmp(dirname, "..")) {
			myFileSystem.users[myFileSystem.cur_userid].path.pop_back();
		}
		else if (strcmp(dirname, ".") && strcmp(dirname, "/"))
		{
			myFileSystem.users[myFileSystem.cur_userid].path.push_back(dirname);
		}
	}

	//	write back the current directory 
	save_cur_dir();
	iput(myFileSystem.cur_path_inode);
	myFileSystem.cur_path_inode = inode;

	//	read the change dir from disk 
	j = 0;

	for (i = 0; i < inode->di_size / BLOCKSIZ + 1; i++)
	{
		//cout<<"read block "<<i<<":"<<inode->di_addr[i]<<endl;
		fseek(fp, DATASTART + inode->di_addr[i] * BLOCKSIZ, SEEK_SET);
		fread(&myFileSystem.users[myFileSystem.cur_userid].cur_dir.direct[j], 1, BLOCKSIZ, fp);
		j += BLOCKSIZ / DIRITEM;
	}
	i = 0;
	myFileSystem.users[myFileSystem.cur_userid].cur_dir.size = 0;
	while (myFileSystem.users[myFileSystem.cur_userid].cur_dir.direct[i++].d_ino != 0)
	{
		myFileSystem.users[myFileSystem.cur_userid].cur_dir.size++;
	}
	
	return true;
}

void Rename(string oldname, string newname)
{
	unsigned int dinodeid;
	Inode *inode;
	int di_ino,i;
	if (oldname == "." || oldname == "..") {
		cout << "该文件不可重命名！" << endl;
		return;
	}
	di_ino = namei(newname.c_str());
	if (di_ino != NOT_FOUND)	/* already existed */
	{
		printf("file already exists!\n");
		return;
	}
	for (i = 0; i < myFileSystem.users[myFileSystem.cur_userid].cur_dir.size; ++i) {
		if (strcmp(myFileSystem.users[myFileSystem.cur_userid].cur_dir.direct[i].d_name, oldname.c_str()) == 0) {
			strcpy(myFileSystem.users[myFileSystem.cur_userid].cur_dir.direct[i].d_name, newname.c_str());
			cout << oldname << "已重命名为" << newname << endl;
			return;
		}
	}
	cout << oldname << "文件不存在！" << endl;
	return;
}

