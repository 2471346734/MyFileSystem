#include "Head.h"

bool RenameFile(string OldFileName,string NewFileName)
{
	if (SearchFile(OldFileName) == -1)
	{
		cout << "�����ڸ��������ļ���" << endl;
		return false;
	}
	if (SearchFile(NewFileName) != -1)
	{
		cout << "����ͬ���ļ���" << endl;
		return false;
	}
	myFileSystem.SystemFile[SearchFile(OldFileName)].FileName = NewFileName;
	cout << "�������ɹ���" << endl;
	return true;
}