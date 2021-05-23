#include "Head.h"

bool CloseFile(string FileName)
{
	int FileIndex = SearchFile(FileName);
	if (FileIndex == -1)//�ļ����ڵ�ǰĿ¼
	{
		cout << "��ǰ�ļ��в����ڸ��ļ�" << endl;
		return false;
	}
	//����ļ��Ƿ񱻴�
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
		cout << "���ļ�δ��" << endl;
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
	cout << "�ļ��رճɹ�" << endl;
	return true;
}