#include "Head.h"

bool CloseFile(string FileName)
{
	int FileIndex = SearchFile(FileName);
	if (FileIndex == -1)//文件不在当前目录
	{
		cout << "当前文件夹不存在该文件" << endl;
		return false;
	}
	//检查文件是否被打开
	list<OpenFile>::iterator it = myFileSystem.OpenFileList.begin();
	while (it != myFileSystem.OpenFileList.end())
	{
		if ((*it).FileIndex == FileIndex)
		{
			break;
		}
		it++;
	}
	if (it == myFileSystem.OpenFileList.end())
	{
		cout << "该文件未打开" << endl;
		return false;
	}
	file myFile = myFileSystem.SystemFile[FileIndex];
	for (int i = myFile.PosInMemory;i <= myFile.PosInMemory + myFile.FileLength;i++)
	{
		for (int j = 0;j < BLOCKLENGTH;j++)
		{
			myFileSystem.Memory[i][j] = '\0';
		}
	}
	myFileSystem.OpenFileList.erase(it);
	cout << "文件关闭成功" << endl;
	return true;
}