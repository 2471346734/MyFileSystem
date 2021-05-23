#include "Head.h"

void BlockRelease(int BlockIndex)
{
	BlockIndex = myFileSystem.SystemFile[BlockIndex].PosInBlock;
	block myBlock = myFileSystem.DataBlock[BlockIndex];
	cout << "�ͷ����ݿ飺";
	while (1)
	{
		myFileSystem.SuperStack.push(BlockIndex);//���ͷŵ����ݿ�ѹ��ջ
		myFileSystem.SuperStackNum++;//ջ�����ݿ�ĸ�����1
		//ջ�����ݿ�ĸ�������LISTLENGTH����ջ��Ԫ�ر��浽�µ�list��
		if (myFileSystem.SuperStackNum > LISTLENGTH)
		{
			myFileSystem.FreeListIndex--;
			while (!myFileSystem.SuperStack.empty())
			{
				myFileSystem.FreeList[myFileSystem.FreeListIndex][BlockIndex] = myFileSystem.SuperStack.top();
				myFileSystem.SuperStack.pop();
				BlockIndex--;
			}
			myFileSystem.SuperStackNum = 0;
		}
		cout << BlockIndex;
		cout << endl;
		//�ͷ����ݿ�
		if (myBlock.NextBlock != -1)
		{
			BlockIndex = myBlock.NextBlock;
			myBlock = myFileSystem.DataBlock[myBlock.NextBlock];
		}
		else
		{
			break;
		}
	}
}

bool DeleteFile(string FileName)
{
	int FileIndex = SearchFile(FileName);
	//�ж��Ƿ���ڸ��ļ�
	if (FileIndex == -1)
	{
		cout << "��ǰĿ¼�����ڸ��ļ�"  << endl;
		return false;
	}
	//���Ȩ��
	if (myFileSystem.SystemFile[FileIndex].FileOwner != myFileSystem.CurrentUser.UserName&&myFileSystem.SystemFile[FileIndex].FileOwner != "public")
	{
		cout << "��û�и��ļ��Ĳ���Ȩ��" << endl;
		return false;
	}
	list<int> NewFreeList;
	list<int>::iterator it = myFileSystem.SystemDirector[myFileSystem.DirectorIndex].FileList.begin();
	while (it!=myFileSystem.SystemDirector[myFileSystem.DirectorIndex].FileList.end())
	{
		if (myFileSystem.SystemFile[*it].FileName == FileName)
		{
			BlockRelease(*it);
		}
		else
		{
			NewFreeList.push_back(*it);
		}
		it++;
	}
	myFileSystem.SystemDirector[myFileSystem.DirectorIndex].FileList = NewFreeList;
	cout << "ɾ���ɹ�" << endl;
	return true;
}