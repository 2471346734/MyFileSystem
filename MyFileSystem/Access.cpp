#include "Head.h"
bool access(Inode * inode, unsigned short mode)
{
	if (myFileSystem.users[myFileSystem.cur_userid].sudo) {
		return true;
	}
	switch (mode)
	{
	case READ:
	{
		if (inode->di_mode & ODIREAD) return true;
		if ((inode->di_mode & GDIREAD) && (myFileSystem.users[myFileSystem.cur_userid].u_gid == inode->di_gid)) return true;
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
	case FAPPEND:
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

bool NameAccess(string name)
{
	for (char c : name) {
		if (!isalnum(c) && c != '_') {
			return false;
		}
	}
	return true;
}
