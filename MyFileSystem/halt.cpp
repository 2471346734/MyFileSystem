#include "Head.h"

void halt()
{
	int i, j;
	/**************step1: 将所有内存i节点写回磁盘*************************/
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
	fclose(fp);
	printf(">Good Bye！See You Next Time.\n");
	exit(0);
}