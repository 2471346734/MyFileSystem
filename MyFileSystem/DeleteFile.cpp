#include "Head.h"

void BlockRelease(int BlockIndex)
{
	BlockIndex = myFileSystem.SystemFile[BlockIndex].PosInBlock;
	block myBlock = myFileSystem.DataBlock[BlockIndex];
	cout << "释放数据块：";
	while (1)
	{
		myFileSystem.SuperStack.push(BlockIndex);//将释放的数据块压入栈
		myFileSystem.SuperStackNum++;//栈中数据块的个数增1
		//栈中数据块的个数大于LISTLENGTH，将栈中元素保存到新的list中
		if (myFileSystem.SuperStackNum > LISTLENGTH)
		{
			myFileSystem.FreeListIndex--;
			while (!myFileSystem.SuperStack.empty())
			{
				myFileSystem.FreeList[myFileSystem.FreeListIndex][BlockIndex] = myFileSystem.SuperStack.top();
				myFileSystem.SuperStack.pop();
				BlockIndex--;
			}
			myFileSystem.SuperStackNum = 0;
		}
		cout << BlockIndex;
		cout << endl;
		//释放数据块
		if (myBlock.NextBlock != -1)
		{
			BlockIndex = myBlock.NextBlock;
			myBlock = myFileSystem.DataBlock[myBlock.NextBlock];
		}
		else
		{
			break;
		}
	}
}

bool DeleteFile(string FileName)
{
	int FileIndex = SearchFile(FileName);
	//判断是否存在该文件
	if (FileIndex == -1)
	{
		cout << "当前目录不存在该文件"  << endl;
		return false;
	}
	//检查权限
	if (myFileSystem.SystemFile[FileIndex].FileOwner != myFileSystem.CurrentUser.UserName&&myFileSystem.SystemFile[FileIndex].FileOwner != "public")
	{
		cout << "您没有该文件的操作权限" << endl;
		return false;
	}
	list<int> NewFreeList;
	list<int>::iterator it = myFileSystem.SystemDirector[myFileSystem.DirectorIndex].FileList.begin();
	while (it!=myFileSystem.SystemDirector[myFileSystem.DirectorIndex].FileList.end())
	{
		if (myFileSystem.SystemFile[*it].FileName == FileName)
		{
			BlockRelease(*it);
		}
		else
		{
			NewFreeList.push_back(*it);
		}
		it++;
	}
	myFileSystem.SystemDirector[myFileSystem.DirectorIndex].FileList = NewFreeList;
	cout << "删除成功" << endl;
	return true;
}