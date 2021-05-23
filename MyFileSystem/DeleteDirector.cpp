#include "Head.h"

bool DeleteDirector(string DirectorName)
{
	int CurrentDirectorIndex = SearchDirector(DirectorName);//���ص�ǰĿ¼��������
	if (CurrentDirectorIndex == -1)
	{
		cout << "��ǰĿ¼�����ڸ��ļ���" << endl;
		return false;
	}
	if (myFileSystem.SystemDirector[CurrentDirectorIndex].FileOwner != myFileSystem.CurrentUser.UserName&&myFileSystem.SystemDirector[CurrentDirectorIndex].FileOwner != "public")
	{
		cout << "��û�и��ļ��еĲ���Ȩ�ޣ�" << endl;
		return false;
	}
	director myDirector = myFileSystem.SystemDirector[myFileSystem.DirectorIndex];//���浱ǰ�ļ���
	myFileSystem.DirectorIndex = myFileSystem.SystemDirector[CurrentDirectorIndex].DirectorId;//���浱ǰDirector����
	//ɾ���ļ��������е��ļ�
	list<int>::iterator it = myFileSystem.SystemDirector[myFileSystem.DirectorIndex].FileList.begin();
	while (it != myFileSystem.SystemDirector[myFileSystem.DirectorIndex].FileList.end())
	{
		DeleteFile(myFileSystem.SystemFile[*it].FileName);
		it++;
	}
	//ɾ���ļ��������е�Ŀ¼
	list<int>::iterator it2 = myFileSystem.SystemDirector[myFileSystem.DirectorIndex].DirectorList.begin();
	while (it2 != myFileSystem.SystemDirector[myFileSystem.DirectorIndex].DirectorList.end())
	{
		DeleteDirector(myFileSystem.SystemDirector[*it].DirectorName);
		it2++;
	}
	while (!myFileSystem.SystemDirector[myFileSystem.DirectorIndex].DirectorList.empty())
	{
		myFileSystem.SystemDirector[myFileSystem.DirectorIndex].DirectorList.pop_back();
	}
	cout << "ɾ���ɹ�" << endl;
	return true;
}