#include "Head.h"

void ShowDirector()
{
	list<int>::iterator it = myFileSystem.SystemDirector[myFileSystem.DirectorIndex].DirectorList.begin();
	cout << "Ŀ¼�б�" << endl;
	while (it!=myFileSystem.SystemDirector[myFileSystem.DirectorIndex].DirectorList.end())
	{
		if ((*it) != -1)
		{
			cout << myFileSystem.SystemDirector[*it].DirectorName << endl;
		}
		it++;
	}
	list<int>::iterator it2 = myFileSystem.SystemDirector[myFileSystem.DirectorIndex].FileList.begin();
	cout << "�ļ��б�" << endl;
	while (it2 != myFileSystem.SystemDirector[myFileSystem.DirectorIndex].FileList.end())
	{
		if ((*it2) != -1)
		{
			cout << myFileSystem.SystemFile[*it2].FileName << endl;
		}
		it2++;
	}
}