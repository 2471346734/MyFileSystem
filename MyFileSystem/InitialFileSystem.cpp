#include "Head.h"

status InitialUser()//将文件卷第0块读出，初始化用户信息
{
	myFileSystem.reg_users.clear();
	int num_users = 0;
	fseek(fp, 0, SEEK_SET);
	fread(&num_users, sizeof(int), 1, fp);
	for (int i = 0; i < num_users; ++i) {
		user_table tmp;
		fseek(fp, sizeof(int)+i*sizeof(user_table), SEEK_SET);
		fread(&tmp, sizeof(user_table), 1, fp);
		myFileSystem.reg_users.push_back(tmp);

	}
	return OK;
}

void InitialMemory()
{
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
			iter++;
		}
	}
	myFileSystem.Hinode.clear();//清空Hinode
	//清空系统打开表
	myFileSystem.sys_ofile.clear();
	//清空用户打开表
	myFileSystem.users[myFileSystem.cur_userid].u_ofile.clear();
	//	write back the current directory 
	myFileSystem.cur_path_inode->di_size = myFileSystem.users[myFileSystem.cur_userid].cur_dir.size*DIRITEM;
	for (i = 0; i < myFileSystem.cur_path_inode->di_size / BLOCKSIZ + 1; i++)
	{
		bfree(myFileSystem.cur_path_inode->di_addr[i]);
	}
	for (i = 0; i < myFileSystem.users[myFileSystem.cur_userid].cur_dir.size; i += BLOCKSIZ / DIRITEM)
	{
		unsigned short block;
		block = balloc();
		myFileSystem.cur_path_inode->di_addr[i] = block;
		fseek(fp, DATASTART + block * BLOCKSIZ, SEEK_SET);
		fwrite(&myFileSystem.users[myFileSystem.cur_userid].cur_dir.direct[i], 1, BLOCKSIZ, fp);
	}
	iput(myFileSystem.cur_path_inode);
	/**************step2: 写回磁盘块并关闭文件系统************************/
	fseek(fp, SUPERSIZ, SEEK_SET);
	fwrite(&myFileSystem.superblock, 1, sizeof(SuperBlock), fp);  //写回磁盘块
}

status InitialFileSystem()
{
	Inode * inode;
	Direct dir_buf[BLOCKSIZ / DIRITEM];
	unsigned int block_buf[BLOCKSIZ / sizeof(int)];
	char * buf;
	int i, j;
	/*	creat the file system file */
	if ((fp = fopen("filesystem", "wb+")) == nullptr) {
		cout << "创建文件卷失败！" << endl;
		exit(0);
	}
	buf = (char *)malloc(TOTALSIZ * sizeof(char));
	memset(buf, 0, TOTALSIZ * sizeof(char));
	if (buf == NULL)
	{
		printf("\nfile system file create failed! \n");
		exit(0);
	}
	fseek(fp, 0, SEEK_SET);
	fwrite(buf, 1, TOTALSIZ * sizeof(char), fp);
	//初始化管理员账户
	int user_num = 1;
	fseek(fp, 0, SEEK_SET);
	fwrite(&user_num, sizeof(int), 1, fp);
	user_table admin;
	admin.p_mode = 01017;
	admin.p_uid = 0001;
	admin.sudo = 'y';
	admin.p_gid = 01;
	cout << "===========================" << endl;
	cout << "请输入管理员用户名：";
	cin >> admin.p_name;
	cout << "请输入管理员密码：";
	cin >> admin.password;
	fseek(fp, sizeof(int), SEEK_SET);
	fwrite(&admin, sizeof(user_table), 1, fp);

	/*	1.creat the main directory and its sub dir etc and the file password */
	inode = iget(0);	/* 0 empty dinode id */
	inode->di_mode = DIEMPTY;
	iput(inode);

	inode = iget(1);    /* 1 main dir id */
	inode->di_number = 1;
	inode->di_mode = DEFAULTMODE | DIDIR;
	inode->di_size = 3 * DIRITEM;
	inode->di_addr[0] = 0;    /* block 0# is used by the main directory */
	strcpy(dir_buf[0].d_name, "..");//主目录的父目录是它本身
	dir_buf[0].d_ino = 1;
	strcpy(dir_buf[1].d_name, ".");
	dir_buf[1].d_ino = 1;
	strcpy(dir_buf[2].d_name, "etc");
	dir_buf[2].d_ino = 2;
	fseek(fp, DATASTART, SEEK_SET);//
	fwrite(dir_buf, 1, 3 * DIRITEM, fp);
	iput(inode);

	inode = iget(2);/* 2 etc dir id */
	inode->di_number = 1;
	inode->di_mode = DEFAULTMODE | DIDIR;
	inode->di_size = 3 * DIRITEM;
	inode->di_addr[0] = 1;    /* block 1# is used by the etc */
	strcpy(dir_buf[0].d_name, "..");
	dir_buf[0].d_ino = 1;
	strcpy(dir_buf[1].d_name, ".");
	dir_buf[1].d_ino = 2;
	strcpy(dir_buf[2].d_name, "password");
	dir_buf[2].d_ino = 3;
	fseek(fp, DATASTART + BLOCKSIZ * 1, SEEK_SET);
	fwrite(dir_buf, 1, 3 * DIRITEM, fp);
	iput(inode);

	inode = iget(3);    /* 3 password id */
	inode->di_number = 1;
	inode->di_mode = DEFAULTMODE | DIFILE;
	inode->di_size = BLOCKSIZ;
	inode->di_addr[0] = 2;

	fseek(fp, DATASTART + 2 * BLOCKSIZ, SEEK_SET);
	//fwrite(pwd0, 1, sizeof(pwd0), fp);
	iput(inode);

	/*	2. initialize the superblock */
	myFileSystem.superblock.s_isize = DINODEBLK;
	myFileSystem.superblock.s_fsize = FILEBLK;
	myFileSystem.superblock.s_ninode = DINODEBLK * BLOCKSIZ / DINODESIZ - 4;
	myFileSystem.superblock.s_nfree = FILEBLK - 3;
	for (i = 0; i < NICINOD; i++)
	{
		myFileSystem.superblock.s_inode[i] = 4 + i;/*	begin with 4, 0,1.2,3, is used by main, etc, password */
	}
	myFileSystem.superblock.s_pinode = 0;
	myFileSystem.superblock.s_rinode = NICINOD + 4;
	/*FILEBLK+1 is a flag of end */
	block_buf[NICFREE - 1] = FILEBLK + 1;
	for (i = 0; i < NICFREE - 1; i++)
		block_buf[NICFREE - 2 - i] = FILEBLK - i;
	fseek(fp, DATASTART + BLOCKSIZ * (FILEBLK - NICFREE - 1), SEEK_SET);
	fwrite(block_buf, 1, BLOCKSIZ, fp);
	for (i = FILEBLK - NICFREE; i > 2; i -= NICFREE)
	{
		for (j = 0; j < NICFREE; j++)
		{
			block_buf[j] = i - j;
		}
		fseek(fp, DATASTART + BLOCKSIZ * (i - j), SEEK_SET);
		fwrite(block_buf, 1, BLOCKSIZ, fp);
	}

	j = i + NICFREE;

	for (i = j; i > 2; i--)
	{
		myFileSystem.superblock.s_free[NICFREE - 1 + i - j] = i;
	}
	myFileSystem.superblock.s_pfree = NICFREE - 1 - j + 3;
	fseek(fp, SUPERSIZ, SEEK_SET);
	fwrite(&myFileSystem.superblock, 1, sizeof(SuperBlock), fp);
	//fclose(fp);
	printf("\nformat finished!\n\n");
	return OK;
}