#include "Head.h"

bool OpenFileFunc(string FileName)
{
	int FileIndex = SearchFile(FileName);
	list<OpenFile>::iterator it = myFileSystem.OpenFileList.begin();
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
	if (myFileSystem.SystemFile[FileIndex].FileLength > FILELENGTH - myFileSystem.MemoryIndex)
	{
		cout << "�ڴ�ռ䲻�㣬�޷�����" << endl;
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
		cout << "�����ظ����ļ�" << endl;
		return false;
	}
	//����OpenFileList
	OpenFile myOpenFile;
	myOpenFile.FileIndex = FileIndex;
	myOpenFile.DirectorIndex = myFileSystem.DirectorIndex;
	myFileSystem.OpenFileList.push_back(myOpenFile);
	int BlockIndex = myFileSystem.SystemFile[FileIndex].PosInBlock;//��ǰ���
	myFileSystem.SystemFile[FileIndex].PosInMemory = myFileSystem.MemoryIndex;//��ǰ�ڴ�ʹ��
	block myBlock;
	while (1)
	{
		if (BlockIndex != -1)
		{
			myBlock = myFileSystem.DataBlock[BlockIndex];
			cout << "�ļ��򿪳ɹ�" << endl;;
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