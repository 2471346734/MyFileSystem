#include "Head.h"

void LoadDirector(FILE *fd, int DirectorNum)
{
	char temp[BLOCKLENGTH];
	for (int i = 0;i < DirectorNum;i++)
	{
		director myDirector;
		fscanf_s(fd, "%d", &myDirector.DirectorId);//目录ID
		fscanf_s(fd, "%s", temp, BLOCKLENGTH);//目录名
		myDirector.DirectorName = temp;
		fscanf_s(fd, "%s", temp, BLOCKLENGTH);//目录Owner
		myDirector.FileOwner = temp;
		fscanf_s(fd, "%d", &myDirector.ParentDirector);//目录的上级目录
		int DirectorCount;
		fscanf_s(fd, "%d", &DirectorCount);//目录的子目录个数
		while (DirectorCount--)//目录的子目录
		{
			int DirectorIndex;
			fscanf_s(fd, "%d", &DirectorIndex);
			myDirector.DirectorList.push_back(DirectorIndex);
		}
		int FileCount;
		fscanf_s(fd, "%d", &FileCount);//目录的文件个数
		while (FileCount--)//目录的文件
		{
			int FileIndex;
			fscanf_s(fd, "%d", &FileIndex);//文件号
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
		fscanf_s(fd, "%d", &myFile.FileId);//文件号
		fscanf_s(fd, "%s", temp, BLOCKLENGTH);//文件名
		myFile.FileName = temp;
		fscanf_s(fd, "%d", &myFile.PosInBlock);//文件在数据块的起始块号
		fscanf_s(fd, "%s", temp, BLOCKLENGTH);
		myFile.FileOwner = temp;
		fscanf_s(fd, "%d", &myFile.FileLength);//文件长度
		fscanf_s(fd, "%d", &myFile.PosInMemory);//文件在内存中的位置
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
		fscanf_s(fd, "%s", temp, BLOCKLENGTH);//用户名
		(*myUser).UserName = temp;
		fscanf_s(fd, "%s", temp, BLOCKLENGTH);//用户密码
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
	fscanf_s(fd, "%d", &myFileSystem.FreeListIndex);//FreeList索引
	fscanf_s(fd, "%d", &myFileSystem.SuperStackNum);//栈的个数
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
		fscanf_s(fd, "%d", &flag);//是否使用的标记
		block* tempblock = (block*)malloc(sizeof(block));
		fscanf_s(fd, "%d", &(*tempblock).NextBlock);//数据块的下一块
		fscanf_s(fd, "%d", &(*tempblock).UseFlag);//数据块的使用空间
		if (flag == 0)
		{
			fscanf_s(fd, "%s", temp, BLOCKLENGTH);//数据块的内容
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
		cout << "不存在文件系统!" << endl;
	}
	int DirectorNum,FileNum;
	fscanf_s(fd, "%d", &DirectorNum);//保存目录数目
	LoadDirector(fd, DirectorNum);
	fscanf_s(fd, "%d", &FileNum);//保存文件数目
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