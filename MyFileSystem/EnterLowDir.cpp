#include "Head.h"

bool EnterLowDir(string DirectorName)
{
	//����Ƿ���ڸ�Ŀ¼
	if (SearchDirector(DirectorName) == -1)
	{
		cout << "���ļ��в����ڵ�ǰĿ¼��!" << endl;
		return false;
	}
	//����Ŀ¼����
	list<int>::iterator it = myFileSystem.SystemDirector[myFileSystem.DirectorIndex].DirectorList.begin();
	while (it != myFileSystem.SystemDirector[myFileSystem.DirectorIndex].DirectorList.end())
	{
		if (myFileSystem.SystemDirector[*it].DirectorName == DirectorName)
		{
			string FileOwner = myFileSystem.SystemDirector[*it].FileOwner;
			if (FileOwner == myFileSystem.CurrentUser.UserName || FileOwner == "public")
			{
				myFileSystem.DirectorIndex = *it;
				cout << endl;
				return true;
			}
			else
			{
				cout << "��û�в���Ȩ��!" << endl;
				return false;
			}
		}
		it++;
	}
}