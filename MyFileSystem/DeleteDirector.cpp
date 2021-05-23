#include "Head.h"

bool DeleteDirector(string DirectorName)
{
	int CurrentDirectorIndex = SearchDirector(DirectorName);//返回当前目录的索引号
	if (CurrentDirectorIndex == -1)
	{
		cout << "当前目录不存在该文件夹" << endl;
		return false;
	}
	if (myFileSystem.SystemDirector[CurrentDirectorIndex].FileOwner != myFileSystem.CurrentUser.UserName&&myFileSystem.SystemDirector[CurrentDirectorIndex].FileOwner != "public")
	{
		cout << "您没有该文件夹的操作权限！" << endl;
		return false;
	}
	director myDirector = myFileSystem.SystemDirector[myFileSystem.DirectorIndex];//保存当前文件夹
	myFileSystem.DirectorIndex = myFileSystem.SystemDirector[CurrentDirectorIndex].DirectorId;//保存当前Director索引
	//删除文件夹中所有的文件
	list<int>::iterator it = myFileSystem.SystemDirector[myFileSystem.DirectorIndex].FileList.begin();
	while (it != myFileSystem.SystemDirector[myFileSystem.DirectorIndex].FileList.end())
	{
		DeleteFile(myFileSystem.SystemFile[*it].FileName);
		it++;
	}
	//删除文件夹中所有的目录
	list<int>::iterator it2 = myFileSystem.SystemDirector[myFileSystem.DirectorIndex].DirectorList.begin();
	while (it2 != myFileSystem.SystemDirector[myFileSystem.DirectorIndex].DirectorList.end())
	{
		DeleteDirector(myFileSystem.SystemDirector[*it].DirectorName);
		it2++;
	}
	while (!myFileSystem.SystemDirector[myFileSystem.DirectorIndex].DirectorList.empty())
	{
		myFileSystem.SystemDirector[myFileSystem.DirectorIndex].DirectorList.pop_back();
	}
	cout << "删除成功" << endl;
	return true;
}