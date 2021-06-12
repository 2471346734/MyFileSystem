#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <stack>
#include <list>
#include <string>
#include < unordered_map >
#include<map>
#include<io.h>
#include<algorithm>
#include<queue>
using namespace std;

#define BLOCKSIZ    128 //每块大小
#define SUPERSIZ	512
#define DIRSIZ      12  //每个目录项名字部分所占字节数，另加i节点号4个字节
#define DIRITEM     sizeof(struct Direct)//每个目录项所占字节数
#define DIRNUM      BLOCKSIZ/DIRITEM //每个目录所包含的最大目录项数（文件数）
#define UNAME       20  //用户名长度
#define PWDSIZ      12  //口令字
#define PWDNUM      10  //最多可设10个口令登录
#define NOFILE      20  //每个用户最多可打开20个文件，即用户打开文件表最大项数
#define NADDR       10  //每个i节点最多指向几块，addr[0]~addr[NADDR]
#define USERNUM     10  //最多允许几个用户登录
#define DINODESIZ   sizeof(struct Dinode)//每个磁盘i节点所占字节

#define DINODEBLK   16  //所有磁盘i节点共占32个物理块
#define FILEBLK     48 //共有512个目录文件物理块
#define NICFREE     50  //超级块中空闲块数组的最大项数
#define NICINOD     50  //超级块中空闲i节点的最大项数
#define DINODESTART 2*SUPERSIZ  //i节点起始地址
#define DATASTART   DINODESTART+DINODEBLK*BLOCKSIZ  //目录、文件区起始地址
#define TOTALSIZ	DATASTART+BLOCKSIZ*FILEBLK		//文件卷总大小

/*文件类型*/
#define DIEMPTY     00000
#define DIFILE      01000
#define DIDIR       02000

/*用户权限*/
#define UDIREAD     00001
#define UDIWRITE    00002
#define UDIEXICUTE  00004

/*组权限*/
#define GDIREAD     00010
#define GDIWRITE    00020
#define GDIEXICUTE  00040

/*其他权限*/
#define ODIREAD     00100
#define ODIWRITE    00200
#define ODIEXICUTE  00400

/*文件访问权限*/
#define READ        1
#define WRITE       2
#define EXICUTE     3
#define DEFAULTMODE 00777

#define SUPDATE     00001//超级块修改
#define IUPDATE     00002//i节点修改

/*文件读写模式*/
const unsigned short FREAD = 00001;
const unsigned short FWRITE = 00002;
const unsigned short FAPPEND = 00004;

#define DISKFULL    65535//磁盘满

#define ARGVNUM     10//最大参数个数减一
#define ARGVSIZ     DIRSIZ*2

#define SEEK_SET    0//文件搜索

#define NOT_FOUND 65535//未找到标志

enum status
{
	OK,ERR
};
typedef struct Inode//内存i节点
{
	char i_flag;
	unsigned int i_ino;//磁盘索引节点标志
	unsigned int i_count;//引用计数
	unsigned short di_number;//关联文件数，当为0时，则删除该文件
	unsigned short di_mode;//存取权限
	unsigned short di_uid;//用户id
	unsigned short di_gid;//组id
	unsigned short di_size;//文件大小
	unsigned int di_addr[NADDR];//占用物理块号
}Inode;

typedef struct Dinode//磁盘i节点
{
	unsigned short di_number;
	unsigned short di_mode;
	unsigned short di_uid;
	unsigned short di_gid;
	unsigned long di_size;
	unsigned int di_addr[NADDR];
}Dinode;

typedef struct Direct//目录项
{
	char d_name[DIRSIZ];//名字
	unsigned int d_ino;//i节点号
}Direct;

typedef struct SuperBlock//超级块
{
	unsigned short s_isize;//索引节点块块数
	unsigned long s_fsize;//数据块块数
	unsigned int s_nfree;//空闲块块数
	unsigned short s_pfree;//空闲块指针
	unsigned int s_free[NICFREE];//空闲块堆栈

	unsigned int s_ninode;//空闲索引节点数
	unsigned short s_pinode;//空闲索引节点指针
	unsigned int s_inode[NICINOD];//空闲索引节点数组
	unsigned int s_rinode;//铭记索引节点
	char s_fmod;//超级块修改标志
}SuperBlock;

typedef struct user_table//密码结构
{
	char p_name[UNAME];//用户名
	unsigned short p_uid;//用户id
	unsigned short p_mode;//用户权限
	unsigned short p_gid;//组id
	bool sudo;//管理员yes or no
	char password[PWDSIZ];//密码
}user_table;

typedef struct Dir//目录结构
{
	struct Direct direct[DIRNUM];//目录项数组
	int size;//目录当前大小
}Dir;


typedef struct File
{
	char f_flag;//文件操作标志
	unsigned int f_count;//引用计数
	struct Inode *f_inode;//指向内存索引节点
	unsigned long f_off;//读/写指针
	bool operator==(const File& b) {
		return this->f_inode->i_ino == b.f_inode->i_ino;
	}
}File;

typedef struct user//用户结构
{
	string u_name;//用户名
	unsigned short u_default_mode;//用户权限
	unsigned short u_uid;
	unsigned short u_gid;
	Dir cur_dir;
	vector<string> path;
	bool sudo;//管理员yes or no
	vector<unsigned short> u_ofile;//用户打开文件表
	user() {}
	user(user_table u) {
		this->u_default_mode = u.p_mode;
		this->u_gid = u.p_gid;
		this->u_name = u.p_name;
		this->u_uid = u.p_uid;
		this->sudo = u.sudo;
	}
}user;

struct FileSystem
{
public:
	SuperBlock superblock;
	map<unsigned short, user> users;
	map<int, Inode*> Hinode;
	Dir* root_dir;
	int cur_userid;
	Inode* cur_path_inode;
	vector<user_table> reg_users;
	vector<File> sys_ofile;
};

extern FILE* fp;
extern FileSystem myFileSystem;
extern void HomePage();
extern status InitialFileSystem();
extern status InitialUser();
extern void LoadFileSystem();
extern bool LogInFileSystem();
extern int CreateFile(const char * filename, unsigned short mode = FWRITE);
extern void Chmod(int fid, unsigned short mode);
extern Inode * iget(unsigned int dinodeid);
extern void iput(Inode * pinode);
extern void bfree(unsigned int block_num);
extern unsigned int  balloc();
extern int namei(const char *name);
extern int iname(const char * name);
extern Inode * ialloc();
extern void ifree(unsigned dinodeid);
extern void _dir();
extern void mkdir(const char *dirname);
extern bool access(Inode * inode, unsigned short mode);
extern bool chdir(const char * dirname);
extern bool DeleteFile(const char *filename);
extern int open(const char * filename, unsigned short openmode);
extern unsigned int read(int fd1, char * buf, unsigned int size);
extern bool DeleteDir(const char *filename);
extern int search_uof(string filename);
extern int write(int cfd, char * buf, unsigned int size);
extern void CloseFile(unsigned short cfd);
extern void halt();
extern void InitialMemory();
extern void Logout();
extern void Regist();
extern void Rename(string oldname, string newname);
extern bool NameAccess(string name);
extern bool ChangeDir(string path);
extern void CopyFile(string org, string des);
extern void MoveFile(string org, string des);
extern void Copy(string org, string des);
extern void ChangeUser();
extern void save_cur_dir();