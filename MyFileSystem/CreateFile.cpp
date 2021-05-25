#include"Head.h"

int BlockAssign()
{
	if (myFileSystem.SuperStack.empty())//�������ջΪ��
	{
		for (int i = 0;i < LISTLENGTH;i++)//�������е�Ԫ�ش浽����ջ��
		{
			int ListUnit = myFileSystem.FreeList[myFileSystem.FreeListIndex][i];
			myFileSystem.SuperStack.push(ListUnit);
			myFileSystem.FreeList[myFileSystem.FreeListIndex][i] = -1;//��ʹ�ù�����-1
		}
		myFileSystem.SuperStackNum = LISTLENGTH;//���ó���ջ��Ԫ����
		if (myFileSystem.FreeListIndex < LISTNUM)
		{
			myFileSystem.FreeListIndex++;
		}
	}
	int blockassign = myFileSystem.SuperStack.top();
	myFileSystem.SuperStack.pop();
	myFileSystem.SuperStackNum--;
	return blockassign;
}

bool CreateFile(string FileName)
{
	if (SearchFile(FileName) != -1)
	{
		cout << "���ļ������Ѵ���ͬ���ļ�������ʧ��" << endl;
		return false;
	}
	if (SearchDirector(FileName) != -1)////Ӧ������
	{
		cout << "���ļ������Ѵ���ͬ�����ļ��У�����ʧ��" << endl;
		return false;
	}
	if (myFileSystem.SuperStack.empty() && myFileSystem.FreeList[LISTNUM - 1][0] == -1)
	{
		cout << "���ļ�ϵͳ�ڴ�û���㹻�ڴ洴���ļ�" << endl;
		return false;
	}
	file myFile;
	myFile.FileId = myFileSystem.SystemFile.size();
	myFile.FileLength = 1;
	myFile.FileOwner = myFileSystem.CurrentUser.UserName;
	myFile.FileName = FileName;
	myFile.PosInMemory = 0;
	myFile.PosInBlock = BlockAssign();
	myFileSystem.SystemFile.push_back(myFile);
	myFileSystem.SystemDirector[myFileSystem.DirectorIndex].FileList.push_back(myFile.FileId);
	cout << "�ļ�����������ݿ�Ϊ" << myFile.PosInBlock << endl;
	cout << "�ļ������ɹ�" << endl;
}