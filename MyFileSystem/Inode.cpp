#include "Head.h"

static Dinode block_buf[BLOCKSIZ / DINODESIZ];
Inode * ialloc()
{
	Inode * temp_inode;
	unsigned int cur_di, count;
	int i, block_end_flag;
	if (myFileSystem.superblock.s_pinode == NICINOD)//超级块中最后一个i节点
	{
		i = 0;
		count = 0;
		block_end_flag = 1;
		myFileSystem.superblock.s_pinode = NICINOD - 1;
		cur_di = myFileSystem.superblock.s_rinode;
		while ((count < NICINOD) || (count <= myFileSystem.superblock.s_ninode))
		{
			if (block_end_flag)
			{
				fseek(fp, DINODESTART + cur_di * DINODESIZ, SEEK_SET);
				fread(block_buf, 1, BLOCKSIZ, fp);
				block_end_flag = 0;
				i = 0;
			}
			while (block_buf[i].di_mode != DIEMPTY)
			{
				cur_di++;
				i++;
			}
			if (i == BLOCKSIZ / DINODESIZ)
				block_end_flag = 1;
			else
			{
				myFileSystem.superblock.s_inode[myFileSystem.superblock.s_pinode--] = cur_di;
				count++;
			}
		}
		myFileSystem.superblock.s_rinode = cur_di;
	}
	temp_inode = iget(myFileSystem.superblock.s_inode[myFileSystem.superblock.s_pinode]);
	fseek(fp, DINODESTART + myFileSystem.superblock.s_inode[myFileSystem.superblock.s_pinode] * DINODESIZ, SEEK_SET);
	fwrite(&temp_inode->di_number, 1, sizeof(Dinode), fp);
	myFileSystem.superblock.s_pinode++;
	myFileSystem.superblock.s_ninode--;
	myFileSystem.superblock.s_fmod = SUPDATE;
	return temp_inode;
}

void ifree(unsigned dinodeid)	 /* ifree */
{
	myFileSystem.superblock.s_ninode++;
	if (myFileSystem.superblock.s_pinode != 0)    /* not full */
	{
		myFileSystem.superblock.s_inode[--myFileSystem.superblock.s_pinode] = dinodeid;
	}
	else /* full */
	{
		if (dinodeid < myFileSystem.superblock.s_rinode)
		{
			//myFileSystem.superblock.s_inode[NICINOD] =dinodeid;
			myFileSystem.superblock.s_rinode = dinodeid;
		}
	}
	myFileSystem.superblock.s_fmod = SUPDATE;
}

Inode * iget(unsigned int dinodeid)    /* iget( ) */
{
	unsigned int inodeid;
	long addr;
	Inode *temp, *newinode;
	inodeid = dinodeid ;

	if (myFileSystem.Hinode.count(dinodeid))
	{
		return myFileSystem.Hinode[dinodeid];
	}

	/*	1. calculate the addr of the dinode in the file sys column */
	addr = DINODESTART + dinodeid * DINODESIZ;
	/*	2. malloc the new inode */
	newinode = (Inode *) malloc(sizeof(Inode));
	//Inode * newinode2;
	//newinode2 = (Inode * ) malloc (sizeof (Inode));//权宜之计，应考虑深拷贝，防止其重复分配地址
	//newinode->i_forw = NULL;
	//newinode->i_back = NULL;

	/*	3.read the dinode to the inode */
	fseek(fp, addr, SEEK_SET);
	fread(&(newinode->di_number), DINODESIZ, 1, fp);
	
	/* 4.put it into hinode0[inodeid] queue */
	myFileSystem.Hinode[dinodeid] = newinode;
	/* 5.initialize the inode */
	newinode->i_count = 1;
	newinode->i_flag = 0;    /* flag for not update */
	newinode->i_ino = dinodeid;
	//if (dinodeid < 3)newinode->di_size = 3 * DIRITEM;
	/*if(dinodeid==3)
		newinode->di_size=BLOCKSIZ;*/
	return newinode;
}

void iput(Inode * pinode) //释放内存i节点，并将i节点内容写回磁盘
{
	long addr;
	unsigned int block_num, i;
	if (pinode->i_count > 2)
	{
		pinode->i_count--;
		return;
	}
	else
	{
		if (pinode->di_number != 0)
		{
			/*write back the inode */
			addr = DINODESTART + pinode->i_ino * DINODESIZ;
			fseek(fp, addr, SEEK_SET);
			fwrite(&pinode->di_number, DINODESIZ, 1, fp);
		}
		else
		{
			/*	rm the inode & the block of the file in the disk */
			block_num = (unsigned int)pinode->di_size / BLOCKSIZ;
			for (i = 0; i < block_num; i++)
			{
				bfree(pinode->di_addr[i]);
			}
			ifree(pinode->i_ino);
		}
		/*	free the inode in the memory */
		myFileSystem.Hinode.erase(pinode->i_ino);
		//free (pinode);
	}
}
