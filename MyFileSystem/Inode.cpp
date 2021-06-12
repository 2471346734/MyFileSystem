#include "Head.h"

static Dinode block_buf[BLOCKSIZ / DINODESIZ];
Inode * ialloc()
{
	int i;								//buf操作下标
	int count;							//计数找到的空闲磁盘i节点
	int block_end_flag;					//对一次分配的一个磁盘块大小的空间遍历完标志
	Inode * temp_inode;			//暂存i节点
	unsigned int cur_di;				//临时铭记i节点

	if (myFileSystem.superblock.s_ninode == 0)			// 空闲磁盘i节点为0
	{
		printf(">Inode null!\n");		// 磁盘i节点使用完  
		return nullptr;
	}

	if (myFileSystem.superblock.s_pinode == NICINOD)		//空闲i节点栈中的空闲i节点使用完，调入新的空闲i节点
	{
		i = 0;
		count = 0;						//对找到的空闲i节点计数
		block_end_flag = 1;

		myFileSystem.superblock.s_pinode = NICINOD - 1;	//更新空闲i节点指针为49，即栈中的最后一个i
		cur_di = myFileSystem.superblock.s_rinode;		//铭记i节点指针

		while ((count <= NICINOD) && (count <= myFileSystem.superblock.s_ninode))						//存入空闲i节点栈的i节点数<=50并且剩余的总空闲i节点数>0
		{
			if (block_end_flag)
			{
				if (cur_di <= (BLOCKSIZ * (DINODEBLK - 1) / DINODESIZ))				//cur_di <= (BLOCKSIZ * DINODEBLK) / DINODESIZ - BLOCKSIZ / DINODESIZ    这里有足够一个磁盘块的剩余空间提取  就提取一个磁盘块的空间
				{
					fseek(fp, DINODESTART + cur_di * DINODESIZ, SEEK_SET);
					fread(block_buf, 1, BLOCKSIZ, fp);									// 读一个磁盘块大小（512B）的i节点空间
					block_end_flag = 0;
					i = 0;
				}
				else																	//这里没有足够一个磁盘块的剩余空间提取  就将剩下的空间全部提取
				{
					fseek(fp, DINODESTART + cur_di * DINODESIZ, SEEK_SET);
					fread(block_buf, 1, BLOCKSIZ * DINODEBLK - cur_di * DINODESIZ, fp);	//读入当前i节点之后的所有i节点空间
					block_end_flag = 0;
					i = 0;
				}
			}

			while ((block_buf[i].di_mode != DIEMPTY) && i < (BLOCKSIZ / DINODESIZ))	//比值是一个磁盘块大小空间中的i节点数   查看每个磁盘i节点空闲
			{
				cur_di++;
				i++;
			}//退出时找到一个空闲的磁盘i节点  退出条件：找到一个空心啊磁盘i节点 or 该磁盘块中的所有i节点遍历完


			if (i == BLOCKSIZ / DINODESIZ)// 一个磁盘块大小空间中的i节点数
				block_end_flag = 1;
			else						  //否则 说明成功找到一个空闲i节点，若不是最后的铭记i节点，就存入空闲i节点栈
			{
				if (count != NICINOD)	  //用于判断找到最终状态的铭记i节点，即找到50个i节点后，再继续查找至找到一个空闲i节点可以作为铭记i节点
				{
					myFileSystem.superblock.s_inode[myFileSystem.superblock.s_pinode--] = cur_di;
					count++;			  //成功加入一个空闲i节点至空闲i节点栈
					cur_di++;			  //临时铭记i节点
					i++;				  //从刚找到的位置继续往后找空闲i节点		
				}
				else
					count++;
			}
		}

		myFileSystem.superblock.s_rinode = cur_di;			  //修改铭记i节点
		myFileSystem.superblock.s_pinode++;					  //空闲i节点退回一个，指向栈中的第一个空闲i节点   不能再用myFileSystem.superblock.s_pinode = NICINOD - count
	}

	temp_inode = iget(myFileSystem.superblock.s_inode[myFileSystem.superblock.s_pinode]);				 //获得一个内存i节点
	fseek(fp, DINODESTART + myFileSystem.superblock.s_inode[myFileSystem.superblock.s_pinode] * DINODESIZ, SEEK_SET);
	fwrite(&(temp_inode->di_number), 1, sizeof(Dinode), fp);//将内存i节点相应的磁盘i节点部分内容写入磁盘
	myFileSystem.superblock.s_pinode++;						  //空闲i节点指针+1
	myFileSystem.superblock.s_ninode--;					      //空闲i节点块数-1
	myFileSystem.superblock.s_fmod = SUPDATE;				  //修改超级块修改标志
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
	block_buf[0].di_mode = DIEMPTY;						// 将该i节点标志为空闲
	fseek(fp, DINODESTART + dinodeid * DINODESIZ, SEEK_SET);
	fwrite(&(block_buf[0].di_number), 1, DINODESIZ, fp);//将新的空闲i节点（本质是其内部修改后的信息）写入磁盘
}

Inode * iget(unsigned int dinodeid)    /* iget( ) */
{
	unsigned int inodeid;
	long addr;
	Inode *temp, *newinode;
	inodeid = dinodeid ;

	if (myFileSystem.Hinode.count(dinodeid))
	{
		myFileSystem.Hinode[dinodeid]->i_count++;
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
	if (pinode->i_count > 1)
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
			if (pinode->di_size == 0) {
				bfree(pinode->di_addr[0]);
			}
			else {
				block_num = (unsigned int)pinode->di_size / BLOCKSIZ + ((unsigned int)pinode->di_size % BLOCKSIZ != 0);
				for (i = 0; i < block_num; i++)
				{
					bfree(pinode->di_addr[i]);
				}
			}
			
			ifree(pinode->i_ino);
		}
		/*	free the inode in the memory */
		myFileSystem.Hinode.erase(pinode->i_ino);
		//free (pinode);
	}
}
