#include "Head.h"
bool ReturnToParentDir()
{
	if (myFileSystem.SystemDirector[myFileSystem.DirectorIndex].ParentDirector != -1)
	{
		myFileSystem.DirectorIndex = myFileSystem.SystemDirector[myFileSystem.DirectorIndex].ParentDirector;
		cout << endl;
		return true;
	}
	else
	{
		cout << "您已在根目录！" << endl;
		return false;
	}
	
}