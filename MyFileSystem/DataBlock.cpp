/* 磁盘块分配与释放函数*/
#include "Head.h"

static unsigned int block_buf[BLOCKSIZ];
unsigned int  balloc()
{
	unsigned int free_block, free_block_num;
	unsigned int i;
	if (myFileSystem.superblock.s_nfree == 0)
	{
		printf("\nDisk Full!!! \n");
		return DISKFULL;
	}

	free_block = myFileSystem.superblock.s_free[myFileSystem.superblock.s_pfree];
	if (myFileSystem.superblock.s_pfree == NICFREE - 1)
	{
		fseek(fp, DATASTART + free_block * BLOCKSIZ, SEEK_SET); //modify here
		fread(block_buf, 1, BLOCKSIZ, fp);
		free_block_num = block_buf[NICFREE];
		for (i = 0; i < free_block_num; i++)
		{
			myFileSystem.superblock.s_free[NICFREE - 1 - i] = block_buf[i];
		}
		myFileSystem.superblock.s_pfree = NICFREE - free_block_num;
	}
	else myFileSystem.superblock.s_pfree++;
	myFileSystem.superblock.s_nfree--;
	myFileSystem.superblock.s_fmod = SUPDATE;
	return free_block;
}

void bfree(unsigned int block_num)
{
	int i;
	if (myFileSystem.superblock.s_pfree == 0)	/* if s-free full */
	{
		block_buf[NICFREE] = NICFREE;
		for (i = 0; i < NICFREE; i++)
		{
			block_buf[i] = myFileSystem.superblock.s_free[NICFREE - 1 - i];
		}
		myFileSystem.superblock.s_pfree = NICFREE - 1;
	}
	else memset(block_buf, 0, BLOCKSIZ);
	fseek(fp, DATASTART + block_num * BLOCKSIZ, SEEK_SET);
	fwrite(block_buf, 1, BLOCKSIZ, fp);
	myFileSystem.superblock.s_nfree++;
	myFileSystem.superblock.s_fmod = SUPDATE;
}
