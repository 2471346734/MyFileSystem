#include "Head.h"

bool RenameDirector(string OldDirectorName, string NewDirectorName)
{
	if (SearchDirector(OldDirectorName) == -1)
	{
		cout << "�����ڸ��������ļ���" << endl;
		return false;
	}
	if (SearchDirector(NewDirectorName) != -1)
	{
		cout << "����ͬ���ļ���" << endl;
		return false;
	}
	myFileSystem.SystemDirector[SearchDirector(OldDirectorName)].DirectorName = NewDirectorName;
	cout << "�������ɹ���" << endl;
	return true;
}