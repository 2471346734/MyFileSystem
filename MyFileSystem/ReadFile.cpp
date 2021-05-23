#include "Head.h"

bool ReadFile(string FileName)
{
	int FileIndex = SearchFile(FileName);
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
	list<OpenFile>::iterator it = myFileSystem.OpenFileList.begin();
	while (it!=myFileSystem.OpenFileList.end())
	{
		if ((*it).FileIndex == FileIndex)
		{
			break;
		}
		it++;
	}
	if (it == myFileSystem.OpenFileList.end())
	{
		cout << "文件未被打开，无法读取" << endl;
		return false;
	}
	file currentfile = myFileSystem.SystemFile[FileIndex];
	cout << "读入内容：";

	block myBlock;
	int BlockIndex = myFileSystem.SystemFile[FileIndex].PosInBlock;
	while (1)
	{
		if (BlockIndex != -1)
		{
			myBlock = myFileSystem.DataBlock[BlockIndex];
			//cout << "文件打开成功" << endl;;
			for (int i = 0;i < myBlock.UseFlag;i++)
			{
				myFileSystem.Memory[myFileSystem.MemoryIndex][i] = myBlock.Content[i];
				cout << myBlock.Content[i];
			}
			myFileSystem.MemoryIndex++;
			BlockIndex = myBlock.NextBlock;
		}
		else
		{
			break;
		}
	}
	/*for (int i = currentfile.PosInMemory;i <= currentfile.PosInMemory + currentfile.FileLength;i++)
	{
		for (int j = 0;j < BLOCKLENGTH;j++)
		{
			if (myFileSystem.Memory[i][j] != '\0')
			{
				cout << myFileSystem.Memory[i][j];
			}
			else
			{
				break;
			}
		}
	}*/
	cout << endl;
	return true;
}