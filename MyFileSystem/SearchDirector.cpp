#include "Head.h"

int SearchDirector(string DirectorName)
{
	list<int>::iterator it = myFileSystem.SystemDirector[myFileSystem.DirectorIndex].DirectorList.begin();
	while (it != myFileSystem.SystemDirector[myFileSystem.DirectorIndex].DirectorList.end())
	{
		if (myFileSystem.SystemDirector[*it].DirectorName == DirectorName)
		{
			return *it;
		}
		it++;
	}
	return -1;
}
