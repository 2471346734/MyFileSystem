#include "Head.h"
void CopyDirectorSonFunc(int DirectorIndex) 
{
	director myDirector = myFileSystem.SystemDirector[DirectorIndex];//当前所在的文件夹
    //初始化被复制文件的信息
	director temp;
	temp.DirectorId = myFileSystem.SystemDirector.size();
	temp.DirectorName = myDirector.DirectorName;
	temp.FileOwner = myDirector.FileOwner;
	temp.ParentDirector = myFileSystem.DirectorIndex;
	//添加复制文件夹中的文件
	list<int>::iterator it = myDirector.FileList.begin();
	while (it!=myDirector.FileList.end())
	{
		temp.FileList.push_back(*it);
		it++;
	}
	//添加被复制的文件夹
	myFileSystem.SystemDirector[myFileSystem.DirectorIndex].DirectorList.push_back(temp.DirectorId);
	myFileSystem.SystemDirector.push_back(temp);
	//进入当前目录
	myFileSystem.DirectorIndex = temp.DirectorId;
	//添加复制文件夹中的文件夹
	it = myDirector.DirectorList.begin();
	while (it!=myDirector.DirectorList.end())
	{
		if ((*it) != -1)
		{
			CopyDirectorSonFunc(*it);
		}
		it++;
	}
}

bool CopyDirector(string DirectorName,string path)
{
	int CurrentDirectorIndex = myFileSystem.DirectorIndex;//保存当前所在目录
	int DirectorIndex = SearchDirector(DirectorName);//保存复制目录的目录号
	myFileSystem.DirectorIndex = 0;//回到根目录
	if (DirectorIndex == -1)
	{
		cout << "当前目录不存在该文件夹" << endl;
		return false;
	}
	string DirectorOwner = myFileSystem.SystemDirector[DirectorIndex].FileOwner;
	if (DirectorOwner != myFileSystem.CurrentUser.UserName&&DirectorOwner != "public")//检查权限
	{
		cout << "您没有该文件夹的操作权限" << endl;
		return false;
	}
	if (!ParsePath(path))
	{
		cout << "复制失败" << endl;
		return false;
	}
	CopyDirectorSonFunc(DirectorIndex);
	cout << "复制成功" << endl;
	return true;
}
