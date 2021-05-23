#include "Head.h"

void SaveDirector(FILE *fd)
{
	vector<director>::iterator itv = myFileSystem.SystemDirector.begin();
	while (itv != myFileSystem.SystemDirector.end())
	{
		int DirectorNum = (*itv).DirectorList.size();
		int FileNum = (*itv).FileList.size();
		fprintf(fd, "%d\n", (*itv).DirectorId);
		fprintf(fd, "%s\n", (*itv).DirectorName.c_str());
		fprintf(fd, "%s\n", (*itv).FileOwner.c_str());
		fprintf(fd, "%d\n", (*itv).ParentDirector);
		fprintf(fd, "%d\n", DirectorNum);
		list<int>::iterator itd = (*itv).DirectorList.begin();
		while (itd != (*itv).DirectorList.end())
		{
			fprintf(fd, "%d\n", (*itd));
			itd++;
		}
		fprintf(fd, "%d\n", FileNum);
		itd = (*itv).FileList.begin();
		while (itd != (*itv).FileList.end())
		{
			fprintf(fd, "%d\n", (*itd));
			itd++;
		}
		//cout << "SaveDirector!" << endl;
		itv++;
	}
	
}

void SaveFile(FILE *fd)
{
	vector<file>::iterator it = myFileSystem.SystemFile.begin();
	while (it != myFileSystem.SystemFile.end())
	{
		fprintf(fd, "%d\n", (*it).FileId);
		fprintf(fd, "%s\n", (*it).FileName.c_str());
		fprintf(fd, "%d\n", (*it).PosInBlock);
		fprintf(fd, "%s\n", (*it).FileOwner.c_str());
		fprintf(fd, "%d\n", (*it).FileLength);
		fprintf(fd, "%d\n", (*it).PosInMemory);
		it++;
	}
	//cout << "SaveFile!" << endl;
}

void SaveUserInfo(FILE *fd)
{
	for (int i = 0;i < USERNUM;i++)
	{
		fprintf(fd, "%s\n", myFileSystem.UserInfo[i].UserName.c_str());
		fprintf(fd, "%s\n", myFileSystem.UserInfo[i].PassWord.c_str());
	}
	//cout << "SaveUserInfo!" << endl;
}

void SaveFreeList(FILE *fd)
{
	for (int i = 0;i < LISTNUM;i++)
	{
		for (int j = 0;j < LISTLENGTH;j++)
		{
			fprintf(fd, "%d\n", myFileSystem.FreeList[i][j]);
		}
	}
	//cout << "SaveFreeList!" << endl;
}

void SaveSuperStack(FILE *fd)
{
	stack<int> stacktemp;
	fprintf(fd, "%d\n", myFileSystem.SuperStackIndex);
	fprintf(fd, "%d\n", myFileSystem.SuperStackNum);
	while (!myFileSystem.SuperStack.empty())
	{
		int top = myFileSystem.SuperStack.top();
		stacktemp.push(top);
		myFileSystem.SuperStack.pop();
	}
	while (!stacktemp.empty())
	{
		fprintf(fd, "%d\n", stacktemp.top());
		stacktemp.pop();
	}
	//cout << "SaveSuperStack!" << endl;
}

void SaveBlock(FILE *fd)
{
	for (int i = 0;i<BLOCKNUM;i++)
	{
		int flag = 1;
		block tempblock = myFileSystem.DataBlock[i];
		if (tempblock.Content[0] != '\0')
		{
			flag = 0;
		}
		fprintf(fd, "%d\n", flag);
		fprintf(fd, "%d\n", tempblock.NextBlock);
		fprintf(fd, "%d\n", tempblock.UseFlag);
		if (flag == 0)
		{
			fprintf(fd, "%s\n", tempblock.Content);
		}
	}
	//cout << "SaveBlock!" << endl;
}

void SaveFileSystem()
{
	FILE* fd;
	string RootPath = "C:/Users/zhangjiarong/Desktop/FileSystem.txt";
	if (fopen_s(&fd, RootPath.c_str(), "w+") != 0)
	{
		cout << "打开文件失败" << endl;
	}
	int DirectorNum = myFileSystem.SystemDirector.size();
	int FileNum = myFileSystem.SystemFile.size();
	fprintf(fd, "%d\n", DirectorNum);
	SaveDirector(fd);
	fprintf(fd, "%d\n", FileNum);
	SaveFile(fd);
	SaveUserInfo(fd);
	SaveFreeList(fd);
	SaveSuperStack(fd);
	SaveBlock(fd);
	cout << "文件系统保存成功" << endl;
	fclose(fd);
}