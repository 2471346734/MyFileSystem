#include "Head.h"

bool RenameDirector(string OldDirectorName, string NewDirectorName)
{
	if (SearchDirector(OldDirectorName) == -1)
	{
		cout << "不存在该重命名文件！" << endl;
		return false;
	}
	if (SearchDirector(NewDirectorName) != -1)
	{
		cout << "存在同名文件！" << endl;
		return false;
	}
	myFileSystem.SystemDirector[SearchDirector(OldDirectorName)].DirectorName = NewDirectorName;
	cout << "重命名成功！" << endl;
	return true;
}