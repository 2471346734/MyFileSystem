#include "Head.h"

bool ReadFile(string FileName)
{
	int FileIndex = SearchFile(FileName);
	if (FileIndex == -1)
	{
		cout << "��ǰ�ļ��в����ڸ��ļ�" << endl;
		return false;
	}
	if (myFileSystem.SystemFile[FileIndex].FileOwner != myFileSystem.CurrentUser.UserName)
	{
		cout << "��û�и��ļ��Ĳ���Ȩ��" << endl;
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
		cout << "�ļ�δ���򿪣��޷���ȡ" << endl;
		return false;
	}
	file currentfile = myFileSystem.SystemFile[FileIndex];
	cout << "�������ݣ�";

	block myBlock;
	int BlockIndex = myFileSystem.SystemFile[FileIndex].PosInBlock;
	while (1)
	{
		if (BlockIndex != -1)
		{
			myBlock = myFileSystem.DataBlock[BlockIndex];
			//cout << "�ļ��򿪳ɹ�" << endl;;
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