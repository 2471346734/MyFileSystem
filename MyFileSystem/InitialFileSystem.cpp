#include "Head.h"

status InitialUser()//将文件卷第0块读出，初始化用户信息
{
	int num_users = 0;
	fseek(fp, 0, SEEK_SET);
	fread(&num_users, sizeof(int), 1, fp);
	for (int i = 0; i < num_users; ++i) {
		user_table tmp;
		fread(&tmp, sizeof(user_table), 1, fp);
		myFileSystem.reg_users.push_back(tmp);

	}
	return OK;
}

/*status Regist()
{

}*/


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
	buf = (char *)malloc((DINODEBLK + FILEBLK + 2) * BLOCKSIZ * sizeof(char));
	memset(buf, 0, (DINODEBLK + FILEBLK + 2) * BLOCKSIZ * sizeof(char));
	if (buf == NULL)
	{
		printf("\nfile system file create failed! \n");
		exit(0);
	}
	fseek(fp, 0, SEEK_SET);
	fwrite(buf, 1, (DINODEBLK + FILEBLK + 2) * BLOCKSIZ * sizeof(char), fp);
	/*0.initialize the passwd */
	int user_num = 1;
	fseek(fp, 0, SEEK_SET);
	fwrite(&user_num, sizeof(int), 1, fp);
	user_table admin;
	admin.p_mode = 01777;
	admin.p_uid = 0001;
	admin.sudo = 'y';
	admin.p_gid = 01;
	cout << "===========================" << endl;
	cout << "请输入管理员用户名：";
	cin >> admin.p_name;
	cout << "请输入管理员密码：";
	cin >> admin.password;
	fwrite(&admin, sizeof(user_table), 1, fp);
	/*strcpy(pwd0[0].p_name, "morgan");
	pwd0[0].p_uid = 0001;
	pwd0[0].p_mode = 01777;
	pwd0[0].p_gid = 01;
	pwd0[0].sudo = 'y';
	strcpy(pwd0[0].password, "admin");*/
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
	/*for (i = 1; i < PWDNUM; i++)
	{
		pwd0[i].p_uid = 0;
		pwd0[i].p_mode = DEFAULTMODE;
		pwd0[i].p_gid = 0;
		strcpy(pwd0[i].password, "	");
	}*/
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
	fseek(fp, BLOCKSIZ, SEEK_SET);
	fwrite(&myFileSystem.superblock, 1, sizeof(SuperBlock), fp);
	//fclose(fp);
	printf("\nformat finished!\n\n");
	return OK;
}