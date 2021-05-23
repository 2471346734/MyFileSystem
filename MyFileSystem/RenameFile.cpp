#include "Head.h"

bool RenameFile(string OldFileName,string NewFileName)
{
	if (SearchFile(OldFileName) == -1)
	{
		cout << "不存在该重命名文件！" << endl;
		return false;
	}
	if (SearchFile(NewFileName) != -1)
	{
		cout << "存在同名文件！" << endl;
		return false;
	}
	myFileSystem.SystemFile[SearchFile(OldFileName)].FileName = NewFileName;
	cout << "重命名成功！" << endl;
	return true;
}