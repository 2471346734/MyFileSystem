#include "Head.h"

bool WriteFile(string FileName, string content)
{
	//�ж��ļ��Ƿ����
	int FileIndex = SearchFile(FileName);
	if (FileIndex == -1)
	{
		cout << "��ǰ�ļ��в����ڸ��ļ�" << endl;
		return false;
	}
	//�ж��û��Ƿ����Ȩ��
	if (myFileSystem.SystemFile[FileIndex].FileOwner != myFileSystem.CurrentUser.UserName)
	{
		cout << "��û�и��ļ��Ĳ���Ȩ��" << endl;
		return false;
	}
	//�ж��ļ��Ƿ��
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
		cout << "�ļ�δ���򿪣��޷�д��" << endl;
		return false;
	}
	if (content != "")
	{
		int BlockIndex = myFileSystem.SystemFile[FileIndex].PosInBlock;//���浱ǰ���ʹ�����
		block *currentblock = &myFileSystem.DataBlock[BlockIndex];//���浱ǰ���
		//�������п�
		while ((*currentblock).NextBlock!=-1)
		{
			BlockIndex = (*currentblock).NextBlock;
			currentblock = &myFileSystem.DataBlock[BlockIndex];
		}
		int length = content.length();//���ݳ���
		int UseFlag = 0;
		while (1)
		{
			if (BLOCKLENGTH - (*currentblock).UseFlag < length)//��ǰ�鲻������
			{
				for (int i = UseFlag;i < BLOCKLENGTH - (*currentblock).UseFlag;i++)
				{
					(*currentblock).Content[(*currentblock).UseFlag] = content[i];
					length--;
					UseFlag++;
					(*currentblock).UseFlag++;
				}
				BlockIndex = BlockAssign();//�����¿�
				(*currentblock).NextBlock = BlockIndex;
				if (BlockIndex != -1)
				{
					currentblock = &myFileSystem.DataBlock[BlockIndex];
				}
				else
				{
					cout << "�ڴ�����" << endl;
					return false;
				}
			}
			else
			{
				for (int i = (*currentblock).UseFlag;i < (*currentblock).UseFlag + length;i++)
				{
					(*currentblock).Content[i] = content[UseFlag];
					UseFlag++;
				}
				(*currentblock).UseFlag = (*currentblock).UseFlag + length;
				cout << "д��ɹ�" << endl;
				cout << "��" << BlockIndex << "�����ݿ�д��" << (*currentblock).UseFlag << "�ֽ�" << endl;
				return true;
			}
		}
	}
}