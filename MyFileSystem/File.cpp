#include "Head.h"

int CreateFile(const char * filename, unsigned short mode)
{
	int di_ith, di_ino, i, j;
	Inode * inode;
	di_ino = namei(filename);
	if (di_ino != NOT_FOUND)	/* already existed */
	{
		printf("file already exists!\n");
		return ERR;
	}
	else /* not existed before */
	{
		if (myFileSystem.superblock.s_nfree == 0 || myFileSystem.superblock.s_ninode == 0) {
			cout << "i节点或数据块为空！" << endl;
			return -1;
		}
		di_ith = iname(filename);
		if (di_ith == NOT_FOUND)
		{
			printf("no empty directory items!\n");
			return -1;
		}
		inode = ialloc();
		myFileSystem.users[myFileSystem.cur_userid].cur_dir.size++;
		myFileSystem.users[myFileSystem.cur_userid].cur_dir.direct[di_ith].d_ino = inode->i_ino;
		if (di_ith + 1 < DIRNUM)myFileSystem.users[myFileSystem.cur_userid].cur_dir.direct[di_ith + 1].d_ino = 0;
		inode->di_mode = myFileSystem.users[myFileSystem.cur_userid].u_default_mode | DIFILE;
		inode->di_uid = myFileSystem.users[myFileSystem.cur_userid].u_uid;
		inode->di_gid = myFileSystem.users[myFileSystem.cur_userid].u_gid;
		inode->di_addr[0] = balloc();
		inode->di_size = 0;
		inode->di_number = 1;
		
		/*i = myFileSystem.sys_ofile.size();
		myFileSystem.users[myFileSystem.cur_userid].u_ofile.push_back(i);
		File tmp;
		tmp.f_flag = mode;
		tmp.f_count = 1;
		tmp.f_off = 0;
		tmp.f_inode = inode;
		myFileSystem.sys_ofile.push_back(tmp);*/
		//iput(inode);

		cout << "创建文件成功！" << endl;
		return OK;
	}
	return ERR;
}

void Chmod(int fid, unsigned short mode)
{
	unsigned int dinodeid;
	Inode * inode;
	int i, j;

	inode = myFileSystem.sys_ofile[myFileSystem.users[myFileSystem.cur_userid].u_ofile[fid]].f_inode;
	if (!access(inode, mode))    /* access denied */
	{
		printf("\nfile open has not access!!!");
		iput(inode);
		return;
	}
	myFileSystem.sys_ofile[myFileSystem.users[myFileSystem.cur_userid].u_ofile[fid]].f_flag = mode;

	if (mode & FAPPEND)
		myFileSystem.sys_ofile[myFileSystem.users[myFileSystem.cur_userid].u_ofile[fid]].f_off = inode->di_size;
	else
		myFileSystem.sys_ofile[myFileSystem.users[myFileSystem.cur_userid].u_ofile[fid]].f_off = 0;
	if (inode->di_size && (mode & FWRITE))
	{
		for (i = 0; i < inode->di_size / BLOCKSIZ + (inode->di_size % BLOCKSIZ != 0); i++)
			bfree(inode->di_addr[i]);
		inode->di_size = 0;
		inode->di_addr[0] = balloc();
	}
	if (myFileSystem.sys_ofile[myFileSystem.users[myFileSystem.cur_userid].u_ofile[fid]].f_flag & FAPPEND) {
		cout << "文件打开模式已修改为追加！" << endl;
	}
	else if (myFileSystem.sys_ofile[myFileSystem.users[myFileSystem.cur_userid].u_ofile[fid]].f_flag & FWRITE) {
		cout << "文件打开模式已修改为写入！" << endl;
	}
	else if (myFileSystem.sys_ofile[myFileSystem.users[myFileSystem.cur_userid].u_ofile[fid]].f_flag & FREAD) {
		cout << "文件打开模式已修改为读取！" << endl;
	}
}

bool DeleteFile(const char *filename)
{
	unsigned int dinodeid;
	Inode *inode;
	int i;
	dinodeid = namei(filename);
	if (dinodeid != 0)
		inode = iget(dinodeid);
	else
	{
		cout << "no such a file!" << endl;
		return false;
	}
	if ((inode->di_mode & DIDIR) != 0) {
		cout << "no such a file!" << endl;
		return false;
	}
	if (!access(inode, WRITE))    /* access denied */
	{
		printf(">failed to remove file because of no authority!\n");
		iput(inode);
		return false;
	}
	for (i = 0; i < myFileSystem.sys_ofile.size(); ++i) {
		if (myFileSystem.sys_ofile[i].f_inode->i_ino == inode->i_ino) {
			cout << "文件已被打开，请关闭后在删除！" << endl;
			iput(inode);
			return false;
		}
	}
	inode->di_number--;                       //压缩目录
	for (i = 0; i < myFileSystem.users[myFileSystem.cur_userid].cur_dir.size; i++)
	{
		if (myFileSystem.users[myFileSystem.cur_userid].cur_dir.direct[i].d_ino == dinodeid)
			break;
	}
	i++;
	while (i < DIRNUM && myFileSystem.users[myFileSystem.cur_userid].cur_dir.direct[i].d_ino != 0)
	{
		strcpy(myFileSystem.users[myFileSystem.cur_userid].cur_dir.direct[i - 1].d_name, myFileSystem.users[myFileSystem.cur_userid].cur_dir.direct[i].d_name);
		myFileSystem.users[myFileSystem.cur_userid].cur_dir.direct[i - 1].d_ino = myFileSystem.users[myFileSystem.cur_userid].cur_dir.direct[i].d_ino;
		i++;
	}
	myFileSystem.users[myFileSystem.cur_userid].cur_dir.direct[i - 1].d_ino = 0;
	myFileSystem.users[myFileSystem.cur_userid].cur_dir.size = i - 1;
	while (inode->i_count > 1) {
		iput(inode);
	}
	iput(inode);
	return true;
}

void CopyFile(string org, string des)
{
	vector<string> tmp_path = myFileSystem.users[myFileSystem.cur_userid].path;
	Dir tmp_dir = myFileSystem.users[myFileSystem.cur_userid].cur_dir;
	unsigned int dinodeid;
	Inode * old_inode, *new_inode;
	int di_ith;
	dinodeid = namei(org.c_str());
	if (dinodeid == NOT_FOUND)    /* no such file */
	{
		printf("\nfile does not existed!!\n");
		return;
	}
	if (ChangeDir(des) == 0) {
		cout << "目的路径不存在！" << endl;
		return;
	}
	int di_ino = namei(org.c_str());
	if (di_ino != NOT_FOUND)	/* already existed */
	{
		printf("file already exists!\n");
		myFileSystem.users[myFileSystem.cur_userid].cur_dir = tmp_dir;
		myFileSystem.users[myFileSystem.cur_userid].path = tmp_path;
		myFileSystem.cur_path_inode = iget(tmp_dir.direct[1].d_ino);
		return;
	}
	di_ith = iname(org.c_str());
	if (di_ith == NOT_FOUND)
	{
		printf("no empty directory items!\n");
		myFileSystem.users[myFileSystem.cur_userid].cur_dir = tmp_dir;
		myFileSystem.users[myFileSystem.cur_userid].path = tmp_path;
		myFileSystem.cur_path_inode = iget(tmp_dir.direct[1].d_ino);
		return;
	}
	old_inode = iget(dinodeid);
	new_inode = ialloc();
	new_inode->di_gid = old_inode->di_gid;
	new_inode->di_mode = old_inode->di_mode;
	new_inode->di_number = 1;
	new_inode->di_size = old_inode->di_size;
	new_inode->di_uid = old_inode->di_uid;
	new_inode->i_count = old_inode->i_count;
	new_inode->i_flag = old_inode->i_flag;
	for (int i = 0; old_inode->di_addr[i] != 0; ++i) {
		char buf[BLOCKSIZ];
		memset(buf, 0, sizeof(buf));
		fseek(fp, DATASTART + old_inode->di_addr[i] * BLOCKSIZ, SEEK_SET);
		fread(buf, 1, BLOCKSIZ, fp);
		int block = balloc();
		fseek(fp, DATASTART + block * BLOCKSIZ, SEEK_SET);
		fwrite(buf, 1, BLOCKSIZ, fp);
		new_inode->di_addr[i] = block;
	}
	strcpy(myFileSystem.users[myFileSystem.cur_userid].cur_dir.direct[di_ith].d_name, org.c_str());
	myFileSystem.users[myFileSystem.cur_userid].cur_dir.direct[di_ith].d_ino = new_inode->i_ino;
	if (di_ith + 1 < DIRNUM)myFileSystem.users[myFileSystem.cur_userid].cur_dir.direct[di_ith + 1].d_ino = 0;
	save_cur_dir();
	iput(old_inode);
	iput(new_inode);
	myFileSystem.users[myFileSystem.cur_userid].cur_dir = tmp_dir;
	myFileSystem.users[myFileSystem.cur_userid].path = tmp_path;
	myFileSystem.cur_path_inode = iget(tmp_dir.direct[1].d_ino);
	//iput(inode);
	cout << "复制文件成功！" << endl;
}

void MoveFile(string org, string des)
{
	vector<string> tmp_path = myFileSystem.users[myFileSystem.cur_userid].path;
	Dir tmp_dir = myFileSystem.users[myFileSystem.cur_userid].cur_dir;
	unsigned int dinodeid;
	Inode * inode;

	int di_ith;
	dinodeid = namei(org.c_str());
	if (dinodeid == NOT_FOUND)    /* no such file */
	{
		printf("\nfile does not existed!!\n");
		return;
	}
	inode = iget(dinodeid);
	if (!myFileSystem.users[myFileSystem.cur_userid].u_ofile.empty()) {
		for (auto& f : myFileSystem.users[myFileSystem.cur_userid].u_ofile) {
			if (myFileSystem.sys_ofile[f].f_inode->i_ino == inode->i_ino) {
				iput(inode);
				cout << "File is open already!" << endl;
				return;
			}
		}
	}
	iput(inode);
	if (ChangeDir(des) == 0) {
		cout << "目的路径不存在！" << endl;
		return;
	}
	int di_ino = namei(org.c_str());
	if (di_ino != NOT_FOUND)	/* already existed */
	{
		printf("file already exists!\n");
		myFileSystem.users[myFileSystem.cur_userid].cur_dir = tmp_dir;
		myFileSystem.users[myFileSystem.cur_userid].path = tmp_path;
		myFileSystem.cur_path_inode = iget(tmp_dir.direct[1].d_ino);
		return;
	}
	di_ith = iname(org.c_str());
	if (di_ith == NOT_FOUND)
	{
		printf("no empty directory items!\n");
		myFileSystem.users[myFileSystem.cur_userid].cur_dir = tmp_dir;
		myFileSystem.users[myFileSystem.cur_userid].path = tmp_path;
		myFileSystem.cur_path_inode = iget(tmp_dir.direct[1].d_ino);
		return;
	}
	inode = iget(dinodeid);
	inode->di_number++;
	strcpy(myFileSystem.users[myFileSystem.cur_userid].cur_dir.direct[di_ith].d_name, org.c_str());
	myFileSystem.users[myFileSystem.cur_userid].cur_dir.direct[di_ith].d_ino = inode->i_ino;
	myFileSystem.users[myFileSystem.cur_userid].cur_dir.size++;
	if (di_ith + 1 < DIRNUM)myFileSystem.users[myFileSystem.cur_userid].cur_dir.direct[di_ith + 1].d_ino = 0;
	save_cur_dir();
	iput(inode);
	myFileSystem.users[myFileSystem.cur_userid].cur_dir = tmp_dir;
	myFileSystem.users[myFileSystem.cur_userid].path = tmp_path;
	myFileSystem.cur_path_inode = iget(tmp_dir.direct[1].d_ino);
	if (!DeleteFile(org.c_str())) {
		cout << "原文件删除失败！" << endl;
	}
	cout << "移动文件成功！" << endl;
}

void Copy(string org, string des)
{
	vector<string> tmp_path = myFileSystem.users[myFileSystem.cur_userid].path;
	Dir tmp_dir = myFileSystem.users[myFileSystem.cur_userid].cur_dir;
	unsigned int dinodeid;
	Inode * inode;
	int di_ith;
	dinodeid = namei(org.c_str());
	if (dinodeid == NOT_FOUND)    /* no such file */
	{
		printf("\nfile does not existed!!\n");
		return;
	}
	if (ChangeDir(des) == 0) {
		cout << "目的路径不存在！" << endl;
		return;
	}
	int di_ino = namei(org.c_str());
	if (di_ino != NOT_FOUND)	/* already existed */
	{
		printf("file already exists!\n");
		myFileSystem.users[myFileSystem.cur_userid].cur_dir = tmp_dir;
		myFileSystem.users[myFileSystem.cur_userid].path = tmp_path;
		myFileSystem.cur_path_inode = iget(tmp_dir.direct[1].d_ino);
		return;
	}
	di_ith = iname(org.c_str());
	if (di_ith == NOT_FOUND)
	{
		printf("no empty directory items!\n");
		myFileSystem.users[myFileSystem.cur_userid].cur_dir = tmp_dir;
		myFileSystem.users[myFileSystem.cur_userid].path = tmp_path;
		myFileSystem.cur_path_inode = iget(tmp_dir.direct[1].d_ino);
		return;
	}
	inode = iget(dinodeid);
	inode->di_number++;
	strcpy(myFileSystem.users[myFileSystem.cur_userid].cur_dir.direct[di_ith].d_name, org.c_str());
	myFileSystem.users[myFileSystem.cur_userid].cur_dir.direct[di_ith].d_ino = inode->i_ino;
	myFileSystem.users[myFileSystem.cur_userid].cur_dir.size++;
	if (di_ith + 1 < DIRNUM)myFileSystem.users[myFileSystem.cur_userid].cur_dir.direct[di_ith + 1].d_ino = 0;
	save_cur_dir();
	iput(inode);
	myFileSystem.users[myFileSystem.cur_userid].cur_dir = tmp_dir;
	myFileSystem.users[myFileSystem.cur_userid].path = tmp_path;
	myFileSystem.cur_path_inode = iget(tmp_dir.direct[1].d_ino);
	//iput(inode);
	cout << "复制引用成功！" << endl;
}

int open(const char * filename, unsigned short openmode)
{
	unsigned int dinodeid;
	Inode * inode;
	int i, j;
	dinodeid = namei(filename);
	if (dinodeid == NOT_FOUND)    /* no such file */
	{
		printf("\nfile does not existed!!\n");
		return NOT_FOUND;
	}
	inode = iget(dinodeid);
	if (!access(inode, openmode))    /* access denied */
	{
		printf("\nfile open has not access!!!");
		iput(inode);
		return NOT_FOUND;
	}
	/* alloc the sys-ofile item */
	if (!myFileSystem.users[myFileSystem.cur_userid].u_ofile.empty()) {
		for (auto& f : myFileSystem.users[myFileSystem.cur_userid].u_ofile) {
			if (myFileSystem.sys_ofile[f].f_inode->i_ino == inode->i_ino) {
				iput(inode);
				cout << "File is open already!" << endl;
				return NOT_FOUND;
			}
		}
	}
	
	File tmp;
	tmp.f_inode = iget(inode->i_ino);
	tmp.f_flag = openmode;
	tmp.f_count = 1;
	if (openmode & FAPPEND)
		tmp.f_off = inode->di_size;
	else
		tmp.f_off = 0;
	myFileSystem.sys_ofile.push_back(tmp);
	/* alloc the user open file item */
	myFileSystem.users[myFileSystem.cur_userid].u_ofile.push_back(myFileSystem.sys_ofile.size() - 1);
	/*if WRITE, free the block of the file before */
	if (inode->di_size && (openmode & FWRITE))
	{
		for (i = 0; i < inode->di_size / BLOCKSIZ + (inode->di_size % BLOCKSIZ != 0); i++)
			bfree(inode->di_addr[i]);
		inode->di_size = 0;
		inode->di_addr[0] = balloc();
	}
	iput(inode);
	return myFileSystem.sys_ofile.size() - 1;//返回在用户打开表中的位置
}

int search_uof(string filename) {
	int ino = namei(filename.c_str());
	if (ino == 0) {
		cout << "no such a file" << endl;
		return -1;
	}
	Inode* inode = iget(ino);
	for (int i = 0; i < myFileSystem.users[myFileSystem.cur_userid].u_ofile.size(); ++i) {
		if (myFileSystem.sys_ofile[myFileSystem.users[myFileSystem.cur_userid].u_ofile[i]].f_inode->i_ino == inode->i_ino) {
			iput(inode);
			return i;
		}
	}
	iput(inode);
	return -1;
}

unsigned int read(int cfd, char * buf, unsigned int size)
{
	unsigned long off;
	int block, block_off, i, j;
	Inode * inode;
	char * temp_buf;

	inode = myFileSystem.sys_ofile[myFileSystem.users[myFileSystem.cur_userid].u_ofile[cfd]].f_inode;  //通过用户打开文件表找到系统文件打开表再找到内存索引节点
	if (!(myFileSystem.sys_ofile[myFileSystem.users[myFileSystem.cur_userid].u_ofile[cfd]].f_flag & FREAD))
	{
		printf(">the file is not opened for read!\n");
		return 0;
	}

	if (!access(inode, READ))    //判断有无权限
	{
		printf(">fail to read file because of no authority!\n");
		iput(inode);				//释放i节点内容
		return NULL;
	}
	memset(buf, 0, sizeof(buf));
	for (i = 0, j = 0; i < inode->di_size / BLOCKSIZ + (inode->di_size % BLOCKSIZ!=0); i++, j += BLOCKSIZ)
	{
		fseek(fp, DATASTART + inode->di_addr[i] * BLOCKSIZ, SEEK_SET);
		fread(buf+j, 1, BLOCKSIZ, fp);
	}

}

int write(int cfd, char * buf, unsigned int size)
{
	unsigned long off;
	unsigned int block, block_off, i, j;
	Inode * inode;
	char * temp_buf;
	//查看文件是否打开用作写操作
	if (!(myFileSystem.sys_ofile[myFileSystem.users[myFileSystem.cur_userid].u_ofile[cfd]].f_flag & FWRITE) &&
		!(myFileSystem.sys_ofile[myFileSystem.users[myFileSystem.cur_userid].u_ofile[cfd]].f_flag & FAPPEND))
	{
		printf(">the file is not opened for write!\n");
		return 0;
	}
	inode = myFileSystem.sys_ofile[myFileSystem.users[myFileSystem.cur_userid].u_ofile[cfd]].f_inode;

	temp_buf = buf;				//指向字符串首
	off = myFileSystem.sys_ofile[myFileSystem.users[myFileSystem.cur_userid].u_ofile[cfd]].f_off;//取得读/写偏移指针
	block_off = off % BLOCKSIZ;	//取得块内偏移地址
	block = off / BLOCKSIZ;		//取得inoden内相对块号
	if (size > BLOCKSIZ * NADDR - off) {//写入大小超过文件最大容量
		cout << "写入大小超过文件最大容量" << endl;
		return 0;
	}
	unsigned int total_size = size;
	unsigned int block_rem = BLOCKSIZ - block_off;
	int w_size = min(block_rem, size);
	fseek(fp, DATASTART + inode->di_addr[block] * BLOCKSIZ+block_off, SEEK_SET);
	fwrite(buf, 1, w_size, fp);
	size -= w_size;
	buf = buf + w_size;
	if (size > 0) {//当前块写不下
		for (i = block + 1; i < NADDR; ++i) {
			int tmp;
			if ((tmp = balloc())==DISKFULL) {
				myFileSystem.sys_ofile[myFileSystem.users[myFileSystem.cur_userid].u_ofile[cfd]].f_off += total_size - size;			//更新读/写指针
				inode->di_size += total_size - size;
				return -1;
			}
			inode->di_addr[i] = tmp;
			fseek(fp, DATASTART + inode->di_addr[i] * BLOCKSIZ, SEEK_SET);
			fwrite(buf, 1, BLOCKSIZ, fp);
			if (size <= BLOCKSIZ)
				break;
			else {
				size -= BLOCKSIZ;
				buf = buf + w_size;
			}
				
		}
	}
	
	myFileSystem.sys_ofile[myFileSystem.users[myFileSystem.cur_userid].u_ofile[cfd]].f_off += total_size;			//更新读/写指针
	inode->di_size += total_size;
	return total_size;
}

void CloseFile(unsigned short cfd)
{
	Inode *inode;
	inode = myFileSystem.sys_ofile[myFileSystem.users[myFileSystem.cur_userid].u_ofile[cfd]].f_inode;	//指向内存i节点的指针
	iput(inode);											//释放i节点内容
	myFileSystem.sys_ofile[myFileSystem.users[myFileSystem.cur_userid].u_ofile[cfd]].f_count--;		//引用计数减1
	if (myFileSystem.sys_ofile[myFileSystem.users[myFileSystem.cur_userid].u_ofile[cfd]].f_count == 0) {
		if(find(myFileSystem.sys_ofile.begin(), myFileSystem.sys_ofile.end(), myFileSystem.sys_ofile[myFileSystem.users[myFileSystem.cur_userid].u_ofile[cfd]])!= myFileSystem.sys_ofile.end())
			myFileSystem.sys_ofile.erase(find(myFileSystem.sys_ofile.begin(), myFileSystem.sys_ofile.end(), myFileSystem.sys_ofile[myFileSystem.users[myFileSystem.cur_userid].u_ofile[cfd]]));
	}
	if(find(myFileSystem.users[myFileSystem.cur_userid].u_ofile.begin(), myFileSystem.users[myFileSystem.cur_userid].u_ofile.end(), cfd)!= myFileSystem.users[myFileSystem.cur_userid].u_ofile.end())
		myFileSystem.users[myFileSystem.cur_userid].u_ofile.erase(find(myFileSystem.users[myFileSystem.cur_userid].u_ofile.begin(), myFileSystem.users[myFileSystem.cur_userid].u_ofile.end(), cfd));
	cout << "close file successfully!" << endl;
}

