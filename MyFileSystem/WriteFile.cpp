#include "Head.h"

bool WriteFile(string FileName, string content)
{
	//判断文件是否存在
	int FileIndex = SearchFile(FileName);
	if (FileIndex == -1)
	{
		cout << "当前文件夹不存在该文件" << endl;
		return false;
	}
	//判断用户是否具有权限
	if (myFileSystem.SystemFile[FileIndex].FileOwner != myFileSystem.CurrentUser.UserName)
	{
		cout << "您没有该文件的操作权限" << endl;
		return false;
	}
	//判断文件是否打开
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
		cout << "文件未被打开，无法写入" << endl;
		return false;
	}
	if (content != "")
	{
		int BlockIndex = myFileSystem.SystemFile[FileIndex].PosInBlock;//保存当前块的使用情况
		block *currentblock = &myFileSystem.DataBlock[BlockIndex];//保存当前块号
		//遍历所有块
		while ((*currentblock).NextBlock!=-1)
		{
			BlockIndex = (*currentblock).NextBlock;
			currentblock = &myFileSystem.DataBlock[BlockIndex];
		}
		int length = content.length();//内容长度
		int UseFlag = 0;
		while (1)
		{
			if (BLOCKLENGTH - (*currentblock).UseFlag < length)//当前块不够分配
			{
				for (int i = UseFlag;i < BLOCKLENGTH - (*currentblock).UseFlag;i++)
				{
					(*currentblock).Content[(*currentblock).UseFlag] = content[i];
					length--;
					UseFlag++;
					(*currentblock).UseFlag++;
				}
				BlockIndex = BlockAssign();//分配新块
				(*currentblock).NextBlock = BlockIndex;
				if (BlockIndex != -1)
				{
					currentblock = &myFileSystem.DataBlock[BlockIndex];
				}
				else
				{
					cout << "内存已满" << endl;
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
				cout << "写入成功" << endl;
				cout << "在" << BlockIndex << "号数据块写入" << (*currentblock).UseFlag << "字节" << endl;
				return true;
			}
		}
	}
}