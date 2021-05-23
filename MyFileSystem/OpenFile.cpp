#include "Head.h"

bool OpenFileFunc(string FileName)
{
	int FileIndex = SearchFile(FileName);
	list<OpenFile>::iterator it = myFileSystem.OpenFileList.begin();
	if (FileIndex == -1)
	{
		cout << "当前文件夹不存在该文件" << endl;
		return false;
	}
	if (myFileSystem.SystemFile[FileIndex].FileOwner != myFileSystem.CurrentUser.UserName)
	{
		cout << "您没有该文件的操作权限" << endl;
		return false;
	}
	if (myFileSystem.SystemFile[FileIndex].FileLength > FILELENGTH - myFileSystem.MemoryIndex)
	{
		cout << "内存空间不足，无法读入" << endl;
		return false;
	}
	while (it != myFileSystem.OpenFileList.end())
	{
		if ((*it).FileIndex == FileIndex)
		{
			break;
		}
		it++;
	}
	if (it != myFileSystem.OpenFileList.end())
	{
		cout << "请勿重复打开文件" << endl;
		return false;
	}
	//加入OpenFileList
	OpenFile myOpenFile;
	myOpenFile.FileIndex = FileIndex;
	myOpenFile.DirectorIndex = myFileSystem.DirectorIndex;
	myFileSystem.OpenFileList.push_back(myOpenFile);
	int BlockIndex = myFileSystem.SystemFile[FileIndex].PosInBlock;//当前块号
	myFileSystem.SystemFile[FileIndex].PosInMemory = myFileSystem.MemoryIndex;//当前内存使用
	block myBlock;
	while (1)
	{
		if (BlockIndex != -1)
		{
			myBlock = myFileSystem.DataBlock[BlockIndex];
			cout << "文件打开成功" << endl;;
			for (int i = 0;i < myBlock.UseFlag;i++)
			{
				myFileSystem.Memory[myFileSystem.MemoryIndex][i] = myBlock.Content[i];
				//cout << myBlock.Content[i];
			}
			myFileSystem.MemoryIndex++;
			BlockIndex = myBlock.NextBlock;
		}
		else
		{
			break;
		}
	}
	return true;
}