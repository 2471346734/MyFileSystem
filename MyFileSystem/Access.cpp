#include "Head.h"
bool access(Inode * inode, unsigned short mode)
{
	switch (mode)
	{
	case READ:
	{
		if (inode->di_mode & ODIREAD) return true;
		if ((inode->di_mode & ODIREAD) && (myFileSystem.users[myFileSystem.cur_userid].u_gid == inode->di_gid)) return true;
		if ((inode->di_mode & UDIREAD) && (myFileSystem.users[myFileSystem.cur_userid].u_uid == inode->di_uid)) return true;
		return false;
	}
	case WRITE:
	{
		if (inode->di_mode & ODIWRITE) return true;
		if ((inode->di_mode & GDIWRITE) && (myFileSystem.users[myFileSystem.cur_userid].u_gid == inode->di_gid)) return true;
		if ((inode->di_mode & UDIWRITE) && (myFileSystem.users[myFileSystem.cur_userid].u_uid == inode->di_uid)) return true;
		return false;
	}
	case EXICUTE:
	{
		if (inode->di_mode & ODIEXICUTE) return true;
		if ((inode->di_mode & GDIEXICUTE) && (myFileSystem.users[myFileSystem.cur_userid].u_gid == inode->di_gid)) return true;
		if ((inode->di_mode & UDIEXICUTE) && (myFileSystem.users[myFileSystem.cur_userid].u_uid == inode->di_uid)) return true;
		return false;
	}
	case DEFAULTMODE:
		return true;
		//default:
	}
	return true;
}
