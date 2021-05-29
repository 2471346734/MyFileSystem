#include "Head.h"

int creat(const char * filename, unsigned short mode)
{
	int di_ith, di_ino, i, j;
	Inode * inode;
	di_ino = namei(filename);
	if (di_ino != NOT_FOUND)	/* already existed */
	{
		printf("file already exists!\n");
		return -1;
	}
	else /* not existed before */
	{
		inode = ialloc();
		di_ith = iname(filename);
		if (di_ith == NOT_FOUND)
		{
			printf("no empty directory items!\n");
			return -1;
		}
		myFileSystem.users[myFileSystem.cur_userid].cur_dir.size++;
		myFileSystem.users[myFileSystem.cur_userid].cur_dir.direct[di_ith].d_ino = inode->i_ino;
		if (di_ith + 1 < DIRNUM)myFileSystem.users[myFileSystem.cur_userid].cur_dir.direct[di_ith + 1].d_ino = 0;
		inode->di_mode = myFileSystem.users[myFileSystem.cur_userid].u_default_mode | DIFILE;
		inode->di_uid = myFileSystem.users[myFileSystem.cur_userid].u_uid;
		inode->di_gid = myFileSystem.users[myFileSystem.cur_userid].u_gid;
		inode->di_addr[0] = balloc();
		inode->di_size = BLOCKSIZ;
		inode->di_number = 1;
		for (i = 0; i < SYSOPENFILE; i++)
		{
			if (myFileSystem.sys_ofile[i].f_count == 0)
			{
				break;
			}
		}
		if (i == SYSOPENFILE)
		{
			printf("\nsystem open file too much!!!\n");
			return -1;
		}
		for (j = 0; j < NOFILE; j++)
		{
			if (myFileSystem.users[myFileSystem.cur_userid].u_ofile[j] == SYSOPENFILE + 1)
			{
				break;
			}
		}
		if (j == NOFILE)
		{
			printf("\nuser open file too much!!! \n");
			return -1;
		}
		myFileSystem.users[myFileSystem.cur_userid].u_ofile[j] = i;
		myFileSystem.sys_ofile[i].f_flag = mode;
		myFileSystem.sys_ofile[i].f_count = 1;
		myFileSystem.sys_ofile[i].f_off = 0;
		myFileSystem.sys_ofile[i].f_inode = inode;
		return j;
	}
	return -1;
}

void close(unsigned short cfd)
{
	Inode *inode;
	inode = myFileSystem.sys_ofile[myFileSystem.users[myFileSystem.cur_userid].u_ofile[cfd]].f_inode;
	iput(inode);
	myFileSystem.sys_ofile[myFileSystem.users[myFileSystem.cur_userid].u_ofile[cfd]].f_count--;
	myFileSystem.users[myFileSystem.cur_userid].u_ofile[cfd] = SYSOPENFILE + 1;
	//cout<<"file has been closed!"<<endl;
}

bool Delete(const char *filename)
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

int open(const char * filename, unsigned short openmode)
{
	unsigned int dinodeid;
	Inode * inode;
	int i, j;
	dinodeid = namei(filename);
	if (dinodeid == 0)    /* no such file */
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
	for (i = 0; i < SYSOPENFILE; i++)
	{
		if (myFileSystem.sys_ofile[i].f_count == 0) break;
	}
	if (i == SYSOPENFILE)
	{
		printf("\nsystem open file too much\n");
		iput(inode);
		return NOT_FOUND;
	}
	myFileSystem.sys_ofile[i].f_inode = inode;
	myFileSystem.sys_ofile[i].f_flag = openmode;
	myFileSystem.sys_ofile[i].f_count = 1;
	if (openmode & FAPPEND)
		myFileSystem.sys_ofile[i].f_off = inode->di_size;
	else
		myFileSystem.sys_ofile[i].f_off = 0;
	/* alloc the user open file item */
	for (j = 0; j < NOFILE; j++)
	{
		if (myFileSystem.users[myFileSystem.cur_userid].u_ofile[j] == SYSOPENFILE + 1) break;
	}
	if (j == NOFILE)
	{
		printf("\nuser open file too much!!! \n");
		myFileSystem.sys_ofile[i].f_count = 0;
		iput(inode);
		return NOT_FOUND;
	}
	myFileSystem.users[myFileSystem.cur_userid].u_ofile[j] = i;
	/*if WRITE, free the block of the file before */
	if (openmode & FWRITE)
	{
		for (i = 0; i < inode->di_size / BLOCKSIZ + 1; i++)
			bfree(inode->di_addr[i]);
		inode->di_size = 0;
	}
	return j;
}

unsigned int read(int fd1, char * buf, unsigned int size)
{
	unsigned long off;
	int block, block_off;
	unsigned int i, j;
	Inode * inode;
	char * temp_buf;
	//cout<<"size="<<size<<endl;
	inode = myFileSystem.sys_ofile[myFileSystem.users[myFileSystem.cur_userid].u_ofile[fd1]].f_inode;
	//cout<<"inode:"<<inode->i_ino<<endl;
	if (!(myFileSystem.sys_ofile[myFileSystem.users[myFileSystem.cur_userid].u_ofile[fd1]].f_flag & FREAD))
	{
		printf("\nthe file is not opened for read\n");
		return 0;
	}
	temp_buf = buf;
	off = myFileSystem.sys_ofile[myFileSystem.users[myFileSystem.cur_userid].u_ofile[fd1]].f_off;
	//cout<<"off="<<off<<endl;
	cout << "file size=" << inode->di_size << endl;
	if ((off + size) > inode->di_size) size = inode->di_size - off;
	block_off = off % BLOCKSIZ;
	//cout<<"block_off="<<block_off<<endl;
	block = off / BLOCKSIZ;
	cout << "block=" << inode->di_addr[block] << endl;
	if (block_off + size < BLOCKSIZ)
	{
		fseek(fp, DATASTART + inode->di_addr[block] * BLOCKSIZ + block_off, SEEK_SET);
		fread(buf, 1, size, fp);
		if (size == 0)cout << "read failed!" << endl;
		//else cout<<"read success!"<<endl;
		return size;
	}
	fseek(fp, DATASTART + inode->di_addr[block] * BLOCKSIZ + block_off, SEEK_SET);
	fread(temp_buf, 1, BLOCKSIZ - block_off, fp);
	temp_buf += BLOCKSIZ - block_off;
	j = block + 1;
	for (i = 0; i < (size - block_off) / BLOCKSIZ + 1; i++)
	{
		fseek(fp, DATASTART + inode->di_addr[j + i] * BLOCKSIZ, SEEK_SET);
		fread(temp_buf, 1, BLOCKSIZ, fp);
		temp_buf += BLOCKSIZ;
	}

	block_off = (size + BLOCKSIZ - block_off) % BLOCKSIZ;
	block = inode->di_addr[off + size / BLOCKSIZ + 1];
	fseek(fp, DATASTART + block * BLOCKSIZ, SEEK_SET);
	fread(temp_buf, 1, block_off, fp);
	myFileSystem.sys_ofile[myFileSystem.users[myFileSystem.cur_userid].u_ofile[fd1]].f_off += size;
	if (size == 0)cout << "read failed!" << endl;
	//else cout<<"read success!"<<endl;
	return size;

}

bool write(int fd1, char * buf, unsigned int size)
{
	unsigned long off;
	int block, block_off;
	unsigned int i;
	Inode * inode;
	char * temp_buf;
	inode = myFileSystem.sys_ofile[myFileSystem.users[myFileSystem.cur_userid].u_ofile[fd1]].f_inode;
	if (!(myFileSystem.sys_ofile[myFileSystem.users[myFileSystem.cur_userid].u_ofile[fd1]].f_flag & (FWRITE | FAPPEND)))
	{
		printf("\n the file is not opened for write\n");
		return false;
	}
	temp_buf = buf;

	off = myFileSystem.sys_ofile[myFileSystem.users[myFileSystem.cur_userid].u_ofile[fd1]].f_off;
	block_off = off % BLOCKSIZ;
	block = off / BLOCKSIZ;

	if (myFileSystem.sys_ofile[myFileSystem.users[myFileSystem.cur_userid].u_ofile[fd1]].f_flag & FWRITE)
	{
		for (int i = block + 1; i < (inode->di_size / BLOCKSIZ); i++)
		{
			bfree(inode->di_addr[i]);
		}
	}

	if (block_off + size < BLOCKSIZ)
	{
		fseek(fp, DATASTART + inode->di_addr[block] * BLOCKSIZ + block_off, SEEK_SET);
		fwrite(buf, 1, size, fp);
		myFileSystem.sys_ofile[myFileSystem.users[myFileSystem.cur_userid].u_ofile[fd1]].f_off += size;
		inode->di_size = myFileSystem.sys_ofile[myFileSystem.users[myFileSystem.cur_userid].u_ofile[fd1]].f_off;
		printf("Write success!\n");
		return true;
	}

	fseek(fp, DATASTART + inode->di_addr[block] * BLOCKSIZ + block_off, SEEK_SET);
	fwrite(temp_buf, 1, BLOCKSIZ - block_off, fp);
	temp_buf += BLOCKSIZ - block_off;
	for (i = 0; i < (size - block_off) / BLOCKSIZ + 1; i++)
	{
		inode->di_addr[block + 1 + i] = balloc();
		fseek(fp, DATASTART + inode->di_addr[block + 1 + i] * BLOCKSIZ, SEEK_SET);
		fwrite(temp_buf, 1, BLOCKSIZ, fp);
		temp_buf += BLOCKSIZ;
	}
	block_off = (size + BLOCKSIZ - block_off) % BLOCKSIZ;
	inode->di_addr[block + 1 + i] = balloc();
	block = inode->di_addr[block + 1 + i];
	fseek(fp, DATASTART + block * BLOCKSIZ, SEEK_SET);
	fwrite(temp_buf, 1, block_off, fp);
	myFileSystem.sys_ofile[myFileSystem.users[myFileSystem.cur_userid].u_ofile[fd1]].f_off += size;
	inode->di_size = myFileSystem.sys_ofile[myFileSystem.users[myFileSystem.cur_userid].u_ofile[fd1]].f_off;
	printf("Write success!\n");
	return true;
}