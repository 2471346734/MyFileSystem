#include "Head.h"

void LoadFileSystem()
{
	/*	1. read the filsys from the superblock */
	fseek(fp, BLOCKSIZ, SEEK_SET);
	fread(&myFileSystem.superblock, 1, sizeof(SuperBlock), fp);
	/* 5. read the main directory to initialize the dir */
	for (int i = 0; i < DIRNUM; i++)
	{
		strcpy(myFileSystem.root_dir.direct[i].d_name, "                 ");
		myFileSystem.root_dir.direct[i].d_ino = 0;
	}
	myFileSystem.cur_path_inode = iget(1);
	//cur_dir.size=cur_path_inode->di_size/DIRITEM;
	int i = 0;
	for (i = 0; i < myFileSystem.root_dir.size / (BLOCKSIZ / DIRITEM); i++)//遍历主目录的所有块
	{
		fseek(fp, DATASTART + BLOCKSIZ * myFileSystem.cur_path_inode->di_addr[i], SEEK_SET);
		fread(&myFileSystem.root_dir.direct[(BLOCKSIZ / DIRITEM) * i], 1, BLOCKSIZ, fp);
	}
	fseek(fp, DATASTART + BLOCKSIZ * myFileSystem.cur_path_inode->di_addr[i], SEEK_SET);
	fread(&myFileSystem.root_dir.direct[(BLOCKSIZ / DIRITEM) * i], 1, myFileSystem.cur_path_inode->di_size % BLOCKSIZ, fp);
	myFileSystem.root_dir.size = 0;
	i = 0;
	while (myFileSystem.root_dir.direct[i++].d_ino != 0)
	{
		myFileSystem.root_dir.size++;
	}
	myFileSystem.users[myFileSystem.cur_userid].cur_dir = myFileSystem.root_dir;
}