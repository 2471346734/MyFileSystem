#include"Head.h"

int BlockAssign()
{
	if (myFileSystem.SuperStack.empty())//如果超级栈为空
	{
		for (int i = 0;i < LISTLENGTH;i++)//将链表中的元素存到超级栈中
		{
			int ListUnit = myFileSystem.FreeList[myFileSystem.FreeListIndex][i];
			myFileSystem.SuperStack.push(ListUnit);
			myFileSystem.FreeList[myFileSystem.FreeListIndex][i] = -1;//被使用过的置-1
		}
		myFileSystem.SuperStackNum = LISTLENGTH;//设置超级栈的元素数
		if (myFileSystem.FreeListIndex < LISTNUM)
		{
			myFileSystem.FreeListIndex++;
		}
	}
	int blockassign = myFileSystem.SuperStack.top();
	myFileSystem.SuperStack.pop();
	myFileSystem.SuperStackNum--;
	return blockassign;
}

bool CreateFile(string FileName)
{
	if (SearchFile(FileName) != -1)
	{
		cout << "本文件夹中已存在同名文件，创建失败" << endl;
		return false;
	}
	if (SearchDirector(FileName) != -1)////应该允许
	{
		cout << "本文件夹中已存在同名子文件夹，创建失败" << endl;
		return false;
	}
	if (myFileSystem.SuperStack.empty() && myFileSystem.FreeList[LISTNUM - 1][0] == -1)
	{
		cout << "该文件系统内存没有足够内存创建文件" << endl;
		return false;
	}
	file myFile;
	myFile.FileId = myFileSystem.SystemFile.size();
	myFile.FileLength = 1;
	myFile.FileOwner = myFileSystem.CurrentUser.UserName;
	myFile.FileName = FileName;
	myFile.PosInMemory = 0;
	myFile.PosInBlock = BlockAssign();
	myFileSystem.SystemFile.push_back(myFile);
	myFileSystem.SystemDirector[myFileSystem.DirectorIndex].FileList.push_back(myFile.FileId);
	cout << "文件被分配的数据块为" << myFile.PosInBlock << endl;
	cout << "文件创建成功" << endl;
}