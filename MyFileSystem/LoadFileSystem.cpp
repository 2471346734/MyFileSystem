#include "Head.h"

void LoadFileSystem()
{
	//��ʼ��������
	fseek(fp, SUPERSIZ, SEEK_SET);
	fread(&myFileSystem.superblock, 1, sizeof(SuperBlock), fp);
	//��ʼ����Ŀ¼
	for (int i = 0; i < DIRNUM; i++)
	{
		strcpy(myFileSystem.users[myFileSystem.cur_userid].cur_dir.direct[i].d_name, "              ");
		myFileSystem.users[myFileSystem.cur_userid].cur_dir.direct[i].d_ino = 0;
	}
	myFileSystem.cur_path_inode = iget(1);
	myFileSystem.users[myFileSystem.cur_userid].cur_dir.size = myFileSystem.cur_path_inode->di_size / (DIRITEM);	//Ŀ¼��ĸ���������ǰĿ¼��С
	int i = 0;
	for (i = 0; i < myFileSystem.users[myFileSystem.cur_userid].cur_dir.size / (BLOCKSIZ / DIRITEM); i++)//������Ŀ¼�����п�
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
	
	myFileSystem.Hinode.clear();//��ʼ���ڴ�i�ڵ��ϣ��
}