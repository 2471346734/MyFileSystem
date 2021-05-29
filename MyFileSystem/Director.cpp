#include "Head.h"

void _dir()	/* _dir */
{
	int i;
	Inode * temp_inode;
	printf("\nCURRENT DIRECTORY\n");
	Dir cur_dir = myFileSystem.users[myFileSystem.cur_userid].cur_dir;
	for (i = 0; i < cur_dir.size; i++)
	{
		if (cur_dir.direct[i].d_ino != DIEMPTY)
		{
			printf("%.*s", DIRSIZ, cur_dir.direct[i].d_name);
			temp_inode = iget(cur_dir.direct[i].d_ino);
			if (temp_inode->di_mode & DIFILE)
				printf(" f ");
			else
				printf(" d ");
			if (temp_inode->di_mode & DIFILE)
			{
				printf("%d", temp_inode->di_size);
				printf(" block chain:");
				for (int i = 0; i < temp_inode->di_size / BLOCKSIZ + 1; i++)
				{
					if (i == 0 && temp_inode->di_addr[0] != 0)printf("%d", temp_inode->di_addr[i]);
					else if (temp_inode->di_addr[i] != 0)printf("%d", temp_inode->di_addr[i]);
				}
				printf("\n");
			}
			else
				printf(" <dir> inode number:%d\n", cur_dir.direct[i].d_ino);
			iput(temp_inode);
		}
	}
}

void mkdir(const char *dirname)	/* mkdir */
{

	int dirid, dirpos;
	Inode * inode;
	Dir* cur_dir = &myFileSystem.users[myFileSystem.cur_userid].cur_dir;
	Direct buf[BLOCKSIZ / DIRITEM];
	unsigned int block;
	memset(buf, 0, (BLOCKSIZ / DIRITEM) * sizeof(Direct));
	dirid = namei(dirname);
	if (dirid != NOT_FOUND)
	{
		inode = iget(dirid);
		if (inode->di_mode & DIDIR)
			printf("\n%s directory already existed! ! \n", dirname);
		else
			printf("\n%s is a file name, &can't create a dir the same name", dirname);
		iput(inode);
		return;
	}
	dirpos = iname(dirname);
	inode = ialloc();
	cur_dir->direct[dirpos].d_ino = inode->i_ino;
	cur_dir->size++;
	/*	fill the new dir buf */
	strcpy(buf[0].d_name, "..");
	buf[0].d_ino = myFileSystem.cur_path_inode->i_ino;
	strcpy(buf[1].d_name, ".");
	buf[1].d_ino = inode->i_ino;
	buf[2].d_ino = 0;
	block = balloc();

	fseek(fp, DATASTART + block * BLOCKSIZ, SEEK_SET);
	fwrite(buf, 1, BLOCKSIZ, fp);

	inode->di_size = 2 * DIRITEM;
	inode->di_number = 1;
	inode->di_mode = DEFAULTMODE | DIDIR;
	inode->di_uid = myFileSystem.users[myFileSystem.cur_userid].u_uid;
	inode->di_gid = myFileSystem.users[myFileSystem.cur_userid].u_gid;
	inode->di_addr[0] = block;
	iput(inode);
	return;
}

void chdir(const char * dirname) 
{
	//    cout<<"---------------------------------------------------------------------------------------------------"<<endl;
	//    cout<<"---------------------------------------------------------------------------------------------------"<<endl;
	int dirid, i, j;
	Inode * inode;
	unsigned short block;
	if (myFileSystem.cur_path_inode->i_ino == 1) {
		if (strcmp(dirname, ".") && strcmp(dirname, "..")) {
			myFileSystem.users[myFileSystem.cur_userid].path.push_back(dirname);
		}
	}
	else
	{
		if (!strcmp(dirname, "..")) {
			myFileSystem.users[myFileSystem.cur_userid].path.pop_back();
		}
		else if(strcmp(dirname, "."))
		{
			myFileSystem.users[myFileSystem.cur_userid].path.push_back(dirname);
		}
	}
		
	if (!strcmp(dirname, "/"))
	{
		dirid = 1;
	}
	else
	{
		dirid = namei(dirname);

		if (dirid == NOT_FOUND)
		{
			printf("\n%s does not existed\n", dirname);
			return;
		}
	}

	inode = iget(dirid);
	if (!access(inode, myFileSystem.users[myFileSystem.cur_userid].u_default_mode))
	{
		printf("\nhas not access to the directory %s", dirname);
		iput(inode);
		return;
	}

	//	write back the current directory 
	myFileSystem.cur_path_inode->di_size = myFileSystem.users[myFileSystem.cur_userid].cur_dir.size*DIRITEM;
	for (i = 0; i < myFileSystem.cur_path_inode->di_size / BLOCKSIZ + 1; i++)
	{
		bfree(myFileSystem.cur_path_inode->di_addr[i]);
	}
	for (i = 0; i < myFileSystem.users[myFileSystem.cur_userid].cur_dir.size; i += BLOCKSIZ / DIRITEM)
	{
		block = balloc();
		myFileSystem.cur_path_inode->di_addr[i] = block;
		fseek(fp, DATASTART + block * BLOCKSIZ, SEEK_SET);
		fwrite(&myFileSystem.users[myFileSystem.cur_userid].cur_dir.direct[i], 1, BLOCKSIZ, fp);
	}
	iput(myFileSystem.cur_path_inode);
	myFileSystem.cur_path_inode = inode;

	//	read the change dir from disk 
	j = 0;

	for (i = 0; i < inode->di_size / BLOCKSIZ + 1; i++)
	{
		//cout<<"read block "<<i<<":"<<inode->di_addr[i]<<endl;
		fseek(fp, DATASTART + inode->di_addr[i] * BLOCKSIZ, SEEK_SET);
		fread(&myFileSystem.users[myFileSystem.cur_userid].cur_dir.direct[j], 1, BLOCKSIZ, fp);
		j += BLOCKSIZ / DIRITEM;
	}
	i = 0;
	myFileSystem.users[myFileSystem.cur_userid].cur_dir.size = 0;
	while (myFileSystem.users[myFileSystem.cur_userid].cur_dir.direct[i++].d_ino != 0)
	{
		myFileSystem.users[myFileSystem.cur_userid].cur_dir.size++;
	}
	
	return;
}
