#include "Head.h"

static Dinode block_buf[BLOCKSIZ / DINODESIZ];
Inode * ialloc()
{
	int i;								//buf�����±�
	int count;							//�����ҵ��Ŀ��д���i�ڵ�
	int block_end_flag;					//��һ�η����һ�����̿��С�Ŀռ�������־
	Inode * temp_inode;			//�ݴ�i�ڵ�
	unsigned int cur_di;				//��ʱ����i�ڵ�

	if (myFileSystem.superblock.s_ninode == 0)			// ���д���i�ڵ�Ϊ0
	{
		printf(">Inode null!\n");		// ����i�ڵ�ʹ����  
		return nullptr;
	}

	if (myFileSystem.superblock.s_pinode == NICINOD)		//����i�ڵ�ջ�еĿ���i�ڵ�ʹ���꣬�����µĿ���i�ڵ�
	{
		i = 0;
		count = 0;						//���ҵ��Ŀ���i�ڵ����
		block_end_flag = 1;

		myFileSystem.superblock.s_pinode = NICINOD - 1;	//���¿���i�ڵ�ָ��Ϊ49����ջ�е����һ��i
		cur_di = myFileSystem.superblock.s_rinode;		//����i�ڵ�ָ��

		while ((count <= NICINOD) && (count <= myFileSystem.superblock.s_ninode))						//�������i�ڵ�ջ��i�ڵ���<=50����ʣ����ܿ���i�ڵ���>0
		{
			if (block_end_flag)
			{
				if (cur_di <= (BLOCKSIZ * (DINODEBLK - 1) / DINODESIZ))				//cur_di <= (BLOCKSIZ * DINODEBLK) / DINODESIZ - BLOCKSIZ / DINODESIZ    �������㹻һ�����̿��ʣ��ռ���ȡ  ����ȡһ�����̿�Ŀռ�
				{
					fseek(fp, DINODESTART + cur_di * DINODESIZ, SEEK_SET);
					fread(block_buf, 1, BLOCKSIZ, fp);									// ��һ�����̿��С��512B����i�ڵ�ռ�
					block_end_flag = 0;
					i = 0;
				}
				else																	//����û���㹻һ�����̿��ʣ��ռ���ȡ  �ͽ�ʣ�µĿռ�ȫ����ȡ
				{
					fseek(fp, DINODESTART + cur_di * DINODESIZ, SEEK_SET);
					fread(block_buf, 1, BLOCKSIZ * DINODEBLK - cur_di * DINODESIZ, fp);	//���뵱ǰi�ڵ�֮�������i�ڵ�ռ�
					block_end_flag = 0;
					i = 0;
				}
			}

			while ((block_buf[i].di_mode != DIEMPTY) && i < (BLOCKSIZ / DINODESIZ))	//��ֵ��һ�����̿��С�ռ��е�i�ڵ���   �鿴ÿ������i�ڵ����
			{
				cur_di++;
				i++;
			}//�˳�ʱ�ҵ�һ�����еĴ���i�ڵ�  �˳��������ҵ�һ�����İ�����i�ڵ� or �ô��̿��е�����i�ڵ������


			if (i == BLOCKSIZ / DINODESIZ)// һ�����̿��С�ռ��е�i�ڵ���
				block_end_flag = 1;
			else						  //���� ˵���ɹ��ҵ�һ������i�ڵ㣬��������������i�ڵ㣬�ʹ������i�ڵ�ջ
			{
				if (count != NICINOD)	  //�����ж��ҵ�����״̬������i�ڵ㣬���ҵ�50��i�ڵ���ټ����������ҵ�һ������i�ڵ������Ϊ����i�ڵ�
				{
					myFileSystem.superblock.s_inode[myFileSystem.superblock.s_pinode--] = cur_di;
					count++;			  //�ɹ�����һ������i�ڵ�������i�ڵ�ջ
					cur_di++;			  //��ʱ����i�ڵ�
					i++;				  //�Ӹ��ҵ���λ�ü��������ҿ���i�ڵ�		
				}
				else
					count++;
			}
		}

		myFileSystem.superblock.s_rinode = cur_di;			  //�޸�����i�ڵ�
		myFileSystem.superblock.s_pinode++;					  //����i�ڵ��˻�һ����ָ��ջ�еĵ�һ������i�ڵ�   ��������myFileSystem.superblock.s_pinode = NICINOD - count
	}

	temp_inode = iget(myFileSystem.superblock.s_inode[myFileSystem.superblock.s_pinode]);				 //���һ���ڴ�i�ڵ�
	fseek(fp, DINODESTART + myFileSystem.superblock.s_inode[myFileSystem.superblock.s_pinode] * DINODESIZ, SEEK_SET);
	fwrite(&(temp_inode->di_number), 1, sizeof(Dinode), fp);//���ڴ�i�ڵ���Ӧ�Ĵ���i�ڵ㲿������д�����
	myFileSystem.superblock.s_pinode++;						  //����i�ڵ�ָ��+1
	myFileSystem.superblock.s_ninode--;					      //����i�ڵ����-1
	myFileSystem.superblock.s_fmod = SUPDATE;				  //�޸ĳ������޸ı�־
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
	block_buf[0].di_mode = DIEMPTY;						// ����i�ڵ��־Ϊ����
	fseek(fp, DINODESTART + dinodeid * DINODESIZ, SEEK_SET);
	fwrite(&(block_buf[0].di_number), 1, DINODESIZ, fp);//���µĿ���i�ڵ㣨���������ڲ��޸ĺ����Ϣ��д�����
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
	//newinode2 = (Inode * ) malloc (sizeof (Inode));//Ȩ��֮�ƣ�Ӧ�����������ֹ���ظ������ַ
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

void iput(Inode * pinode) //�ͷ��ڴ�i�ڵ㣬����i�ڵ�����д�ش���
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
