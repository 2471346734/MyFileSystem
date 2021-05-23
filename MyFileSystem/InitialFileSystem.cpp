#include "Head.h"

void InitialBlock()
{
	for (int i = 0;i < BLOCKNUM;i++)
	{
		block myBlock;
		myBlock.NextBlock = -1;
		myBlock.UseFlag = 0;
		for (int j = 0;j < BLOCKLENGTH;j++)
		{
			myBlock.Content[j] = '\0';
		}
		myFileSystem.DataBlock.push_back(myBlock);
	}
}

void InitialList()
{
	for (int i = 0;i < LISTNUM;i++)
	{
		for (int j = LISTLENGTH-1;j>=0;j--)
		{
			myFileSystem.FreeList[i][j] = i*LISTLENGTH + LISTLENGTH - j - 1;
			//cout << "(" << i << "," << j << ") " << myFileSystem.FreeList[i][j] << endl;
		}
	}
}

FileSystem InitialUser()
{
	char name[6];
	strcpy_s(name, "user");
	for (int i = 0;i<USERNUM;i++)
	{
		name[4] = (char)i + 49;
		name[5] = '\0';
		myFileSystem.UserInfo[i].UserName = name;
		myFileSystem.UserInfo[i].PassWord = "1234";
	}
	return myFileSystem;
}

void InitialDirector()
{
	director root;
	root.ParentDirector = -1;
	root.DirectorId = 0;
	root.DirectorName = "Home";
	root.FileOwner = "public";
	myFileSystem.SystemDirector.push_back(root);
}

void InitialFileSystem()
{
	myFileSystem.FreeListIndex = 0;
	myFileSystem.SuperStackIndex = 0;
	myFileSystem.MemoryIndex = 0;
	myFileSystem.SuperStackNum = 0;
	myFileSystem.DirectorIndex = 0;
	myFileSystem.SystemFile.clear();
	myFileSystem.SystemDirector.clear();
	while (!myFileSystem.SuperStack.empty())
	{
		myFileSystem.SuperStack.pop();
	}
	InitialBlock();
	InitialDirector();
	InitialList();
	InitialUser();
	for (int i = 0;i < FILELENGTH;i++)
	{
		for (int j = 0;j < BLOCKLENGTH;j++)
		{
			myFileSystem.Memory[i][j] = '\0';
		}
	}
}