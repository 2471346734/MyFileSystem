#include "Head.h"

bool MakeDirector(string DirectorName)
{
	if (SearchDirector(DirectorName) != -1)
	{
		cout << "�Ѵ���ͬ���ļ���" << endl;
		return false;
	}
	if (SearchFile(DirectorName) != -1)
	{
		cout << "�Ѵ���ͬ���ļ�" << endl;
		return false;
	}
	director myDirector;
	myDirector.DirectorName = DirectorName;
	myDirector.FileOwner = myFileSystem.CurrentUser.UserName;
	myDirector.DirectorId = myFileSystem.SystemDirector.size();
	myDirector.ParentDirector = myFileSystem.DirectorIndex;
	myFileSystem.SystemDirector.push_back(myDirector);
	myFileSystem.SystemDirector[myFileSystem.DirectorIndex].DirectorList.push_back(myDirector.DirectorId);
	cout << "�ļ��д����ɹ�" << endl;
	return true;

}