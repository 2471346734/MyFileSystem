#include "Head.h"

void LoadFileSystem()
{
	//初始化超级块
	fseek(fp, SUPERSIZ, SEEK_SET);
	fread(&myFileSystem.superblock, 1, sizeof(SuperBlock), fp);
	//初始化主目录
	for (int i = 0; i < DIRNUM; i++)
	{
		strcpy(myFileSystem.users[myFileSystem.cur_userid].cur_dir.direct[i].d_name, "              ");
		myFileSystem.users[myFileSystem.cur_userid].cur_dir.direct[i].d_ino = 0;
	}
	myFileSystem.cur_path_inode = iget(1);
	myFileSystem.users[myFileSystem.cur_userid].cur_dir.size = myFileSystem.cur_path_inode->di_size / (DIRITEM);	//目录项的个数，即当前目录大小
	int i = 0;
	for (i = 0; i < myFileSystem.users[myFileSystem.cur_userid].cur_dir.size / (BLOCKSIZ / DIRITEM); i++)//遍历主目录的所有块
	{
		fseek(fp, DATASTART + BLOCKSIZ * myFileSystem.cur_path_inode->di_addr[i], SEEK_SET);
		fread(&myFileSystem.users[myFileSystem.cur_userid].cur_dir.direct[(BLOCKSIZ / DIRITEM) * i], 1, BLOCKSIZ, fp);
	}
	fseek(fp, DATASTART + BLOCKSIZ * myFileSystem.cur_path_inode->di_addr[i], SEEK_SET);
	fread(&myFileSystem.users[myFileSystem.cur_userid].cur_dir.direct[(BLOCKSIZ / DIRITEM) * i], 1, myFileSystem.cur_path_inode->di_size % BLOCKSIZ, fp);
	myFileSystem.users[myFileSystem.cur_userid].cur_dir.size = 0;
	i = 0;
	while (myFileSystem.users[myFileSystem.cur_userid].cur_dir.direct[i++].d_ino != 0)
	{
		myFileSystem.users[myFileSystem.cur_userid].cur_dir.size++;
	}
	myFileSystem.root_dir = &myFileSystem.users[myFileSystem.cur_userid].cur_dir;
	
	myFileSystem.Hinode.clear();//初始化内存i节点哈希表
}