#include  "Head.h"

bool ParsePath(string path)
{
	int start = 0;//当前解析路径的开始指针
	int end = 0;//当前解析路径的结束指针
	int flag = 1;
	while (end < strlen(path.c_str()))//未到path尾部
	{
		if (path[end] == '/')
		{
			string director = path.substr(start, end - start);//当前目录
			if (flag)//用来第一次进入循环时，检查是否在Home路径下
			{
				if (director != "Home")
				{
					cout << "输入的路径不正确！" << endl;
					return false;
				}
				flag = 0;
			}
			else
			{
				if (!EnterLowDir(director))//进入解析的目录下
				{
					return false;
				}
			}
			start = end + 1;
		}
		end++;
	}
	string director = path.substr(start, end - start);
	if (!EnterLowDir(director))
	{
		return false;
	}
	return true;
}

bool CopyFile(string FileName, string path)
{
	int DirectorIndex = myFileSystem.DirectorIndex;//保存目录号
	int FileIndex = SearchFile(FileName);//文件号
	if (FileIndex == -1)
	{
		cout << "当前目录不存在该文件" << endl;
		return false;
	}
	string FileOwner = myFileSystem.SystemFile[FileIndex].FileOwner;
	if (FileOwner != myFileSystem.CurrentUser.UserName&&FileOwner != "public")//检查权限
	{
		cout << "您没有该文件的操作权限" << endl;
		return false;
	}
	if (!ParsePath(path))//解析路径
	{
		cout << "复制失败" << endl;
		return false;
	}
	myFileSystem.SystemDirector[myFileSystem.DirectorIndex].FileList.push_back(FileIndex);//把该文件添加到复制的文件夹中
	myFileSystem.DirectorIndex = DirectorIndex;//恢复目录
	cout << "复制成功" << endl;
}