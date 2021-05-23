#include"Head.h"

int SearchFile(string FileName)
{
	list<int>::iterator it = myFileSystem.SystemDirector[myFileSystem.DirectorIndex].FileList.begin();
	while (it != myFileSystem.SystemDirector[myFileSystem.DirectorIndex].FileList.end())
	{
		if (myFileSystem.SystemFile[*it].FileName == FileName)
		{
			return *it;
		}
		it++;
	}
	return -1;
}