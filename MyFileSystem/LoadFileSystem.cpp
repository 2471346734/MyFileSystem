#include "Head.h"

void LoadDirector(FILE *fd, int DirectorNum)
{
	char temp[BLOCKLENGTH];
	for (int i = 0;i < DirectorNum;i++)
	{
		director myDirector;
		fscanf_s(fd, "%d", &myDirector.DirectorId);//Ŀ¼ID
		fscanf_s(fd, "%s", temp, BLOCKLENGTH);//Ŀ¼��
		myDirector.DirectorName = temp;
		fscanf_s(fd, "%s", temp, BLOCKLENGTH);//Ŀ¼Owner
		myDirector.FileOwner = temp;
		fscanf_s(fd, "%d", &myDirector.ParentDirector);//Ŀ¼���ϼ�Ŀ¼
		int DirectorCount;
		fscanf_s(fd, "%d", &DirectorCount);//Ŀ¼����Ŀ¼����
		while (DirectorCount--)//Ŀ¼����Ŀ¼
		{
			int DirectorIndex;
			fscanf_s(fd, "%d", &DirectorIndex);
			myDirector.DirectorList.push_back(DirectorIndex);
		}
		int FileCount;
		fscanf_s(fd, "%d", &FileCount);//Ŀ¼���ļ�����
		while (FileCount--)//Ŀ¼���ļ�
		{
			int FileIndex;
			fscanf_s(fd, "%d", &FileIndex);//�ļ���
			myDirector.FileList.push_back(FileIndex);
		}
		myFileSystem.SystemDirector.push_back(myDirector);
	}
	//cout << "LoadDirector!" << endl;
}

void LoadFile(FILE *fd, int FileNum)
{
	char temp[BLOCKLENGTH];
	while (FileNum--)
	{
		file myFile;
		fscanf_s(fd, "%d", &myFile.FileId);//�ļ���
		fscanf_s(fd, "%s", temp, BLOCKLENGTH);//�ļ���
		myFile.FileName = temp;
		fscanf_s(fd, "%d", &myFile.PosInBlock);//�ļ������ݿ����ʼ���
		fscanf_s(fd, "%s", temp, BLOCKLENGTH);
		myFile.FileOwner = temp;
		fscanf_s(fd, "%d", &myFile.FileLength);//�ļ�����
		fscanf_s(fd, "%d", &myFile.PosInMemory);//�ļ����ڴ��е�λ��
		myFileSystem.SystemFile.push_back(myFile);
	}
	//cout << "LoadFile!" << endl;
}

void LoadUserInfo(FILE *fd)
{
	char temp[BLOCKLENGTH];
	for (int i = 0;i < USERNUM;i++)
	{
		user *myUser = &myFileSystem.UserInfo[i];
		fscanf_s(fd, "%s", temp, BLOCKLENGTH);//�û���
		(*myUser).UserName = temp;
		fscanf_s(fd, "%s", temp, BLOCKLENGTH);//�û�����
		(*myUser).PassWord = temp;
	}
	//cout << "LoadUserInfo!" << endl;
}

void LoadFreeList(FILE *fd)
{
	for (int i = 0;i < LISTNUM;i++)
	{
		for (int j = 0;j < LISTLENGTH;j++)
		{
			fscanf_s(fd, "%d", &myFileSystem.FreeList[i][j]);//Freelist
		}
	}
	//cout << "LoadFreeList!" << endl;
}

void LoadSuperStack(FILE *fd)
{
	fscanf_s(fd, "%d", &myFileSystem.FreeListIndex);//FreeList����
	fscanf_s(fd, "%d", &myFileSystem.SuperStackNum);//ջ�ĸ���
	int SuperStackNum = myFileSystem.SuperStackNum;
	while (SuperStackNum--)
	{
		int SuperStackIndex;
		fscanf_s(fd, "%d", &SuperStackIndex);
		myFileSystem.SuperStack.push(SuperStackIndex);
	}
	//cout << "LoadSuperStack!" << endl;
}

void LoadBlock(FILE *fd)
{
	char temp[BLOCKLENGTH];
	for (int i = 0;i < BLOCKNUM;i++)
	{ 
		int flag;
		fscanf_s(fd, "%d", &flag);//�Ƿ�ʹ�õı��
		block* tempblock = (block*)malloc(sizeof(block));
		fscanf_s(fd, "%d", &(*tempblock).NextBlock);//���ݿ����һ��
		fscanf_s(fd, "%d", &(*tempblock).UseFlag);//���ݿ��ʹ�ÿռ�
		if (flag == 0)
		{
			fscanf_s(fd, "%s", temp, BLOCKLENGTH);//���ݿ������
			strcpy_s((*tempblock).Content, temp);
		}
		else
		{
			for (int j = 0;j<BLOCKNUM;j++)
			{
				(*tempblock).Content[j] = '\0';
			}
		}
		myFileSystem.DataBlock.push_back(*tempblock);
	}
	//cout << "LoadBlock!" << endl;
}

void LoadFileSystem()
{
	FILE* fd;
	string RootPath = "C:/Users/zhangjiarong/Desktop/FileSystem.txt";
	if (fopen_s(&fd, RootPath.c_str(), "r") != 0)
	{
		cout << "�������ļ�ϵͳ!" << endl;
	}
	int DirectorNum,FileNum;
	fscanf_s(fd, "%d", &DirectorNum);//����Ŀ¼��Ŀ
	LoadDirector(fd, DirectorNum);
	fscanf_s(fd, "%d", &FileNum);//�����ļ���Ŀ
	LoadFile(fd, FileNum);
	LoadUserInfo(fd);
	LoadFreeList(fd);
	LoadSuperStack(fd);
	LoadBlock(fd);
	fclose(fd);
	myFileSystem.MemoryIndex = 0;
	myFileSystem.DirectorIndex = 0;
	//cout << "LoadFileSystem!" << endl;
}