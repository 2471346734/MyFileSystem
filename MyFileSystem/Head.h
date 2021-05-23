#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <stack>
#include <list>
#include <string>
using namespace std;

#define BLOCKLENGTH 512
#define BLOCKNUM 512
#define LISTNUM 4
#define LISTLENGTH BLOCKNUM/LISTNUM
#define FILELENGTH 10
#define USERNUM 8

typedef struct file
{
	int FileId; 
	string FileName;
	int    PosInBlock;
	string FileOwner;
	int    FileLength;
	int PosInMemory;
}file;

typedef struct director
{
	int DirectorId;
	string DirectorName;
	list<int> FileList;
	list<int> DirectorList; 
	string FileOwner;
	int ParentDirector;
}director;

typedef struct block
{
	int UseFlag;
	int NextBlock;
	char Content[BLOCKLENGTH]; 
}block;

typedef struct user
{
	string UserName;
	string PassWord;
}user;

typedef struct OpenFile
{
	int FileIndex;
	int DirectorIndex;
}OpenFile;

typedef struct FileSystem
{
	vector<block> DataBlock;
	stack<int> SuperStack; 
	list<OpenFile> OpenFileList;
	vector<director> SystemDirector; 
	vector<file> SystemFile;
	user CurrentUser;
	int FreeList[LISTNUM][LISTLENGTH];
	char Memory[FILELENGTH][BLOCKLENGTH];
	user UserInfo[USERNUM];
	int FreeListIndex;
	int SuperStackIndex;
	int DirectorIndex;
	int MemoryIndex;
	int SuperStackNum;
}FileSystem;

extern FileSystem myFileSystem;
extern void HomePage();
extern void InitialFileSystem();
extern FileSystem InitialUser( );
extern void LoadFileSystem();
extern void SaveFileSystem();
extern bool LogInFileSystem();
extern bool ParsePath(string path);

extern int SearchFile(string FileName);
extern int BlockAssign();
extern bool CreateFile(string FileName);
extern bool DeleteFile(string FileName);
extern bool ReadFile(string FileName);
extern bool WriteFile(string FileName, string content);
extern bool OpenFileFunc(string FileName);
extern bool RenameFile(string OldFileName, string NewFileName);
extern bool CloseFile(string FileName);
extern bool CopyFile(string FileName, string path);
extern void CutFile(string FileName, string path);

extern bool MakeDirector(string DirectorName);
extern int SearchDirector(string DirectorName);
extern bool CloseFile(string FileName);
extern bool ReturnToParentDir();
extern bool EnterLowDir(string DirectorName);
extern void ShowDirector();
extern bool DeleteDirector(string DirectorName);
extern bool RenameDirector(string OldDirectorName, string NewDirectorName);
extern bool CopyDirector(string FileName, string path);
extern void CutDirector(string DirectorName, string path);



