#include "Head.h"

bool EnterLowDir(string DirectorName)
{
	//检查是否存在该目录
	if (SearchDirector(DirectorName) == -1)
	{
		cout << "该文件夹不存在当前目录中!" << endl;
		return false;
	}
	//遍历目录链表
	list<int>::iterator it = myFileSystem.SystemDirector[myFileSystem.DirectorIndex].DirectorList.begin();
	while (it != myFileSystem.SystemDirector[myFileSystem.DirectorIndex].DirectorList.end())
	{
		if (myFileSystem.SystemDirector[*it].DirectorName == DirectorName)
		{
			string FileOwner = myFileSystem.SystemDirector[*it].FileOwner;
			if (FileOwner == myFileSystem.CurrentUser.UserName || FileOwner == "public")
			{
				myFileSystem.DirectorIndex = *it;
				cout << endl;
				return true;
			}
			else
			{
				cout << "您没有操作权限!" << endl;
				return false;
			}
		}
		it++;
	}
}