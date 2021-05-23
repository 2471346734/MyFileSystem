#include "Head.h"

bool MakeDirector(string DirectorName)
{
	if (SearchDirector(DirectorName) != -1)
	{
		cout << "已存在同名文件夹" << endl;
		return false;
	}
	if (SearchFile(DirectorName) != -1)
	{
		cout << "已存在同名文件" << endl;
		return false;
	}
	director myDirector;
	myDirector.DirectorName = DirectorName;
	myDirector.FileOwner = myFileSystem.CurrentUser.UserName;
	myDirector.DirectorId = myFileSystem.SystemDirector.size();
	myDirector.ParentDirector = myFileSystem.DirectorIndex;
	myFileSystem.SystemDirector.push_back(myDirector);
	myFileSystem.SystemDirector[myFileSystem.DirectorIndex].DirectorList.push_back(myDirector.DirectorId);
	cout << "文件夹创建成功" << endl;
	return true;

}