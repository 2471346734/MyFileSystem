#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <stack>
#include <list>
#include <string>
#include < unordered_map >
#include<io.h>
using namespace std;

#define BLOCKSIZ    512 //ÿ���С
#define SYSOPENFILE 40  //ϵͳ���ļ����������
#define DIRNUM      128 //ÿ��Ŀ¼�����������Ŀ¼�������ļ�����
#define DIRSIZ      12//14  //ÿ��Ŀ¼�����ֲ�����ռ�ֽ��������i�ڵ��4���ֽ�
#define DIRITEM     sizeof(struct Direct)//ÿ��Ŀ¼����ռ�ֽ���

#define UNAME       20  //�û�������
#define PWDSIZ      12  //������
#define PWDNUM      10  //������10�������¼
#define NOFILE      20  //ÿ���û����ɴ�20���ļ������û����ļ����������
#define NADDR       10  //ÿ��i�ڵ����ָ�򼸿飬addr[0]~addr[NADDR]
#define NHINO       128 //��128��Hash�����ṩ����i�ڵ�
#define USERNUM     10  //����������û���¼
#define DINODESIZ   sizeof(struct Dinode)//ÿ������i�ڵ���ռ�ֽ�

#define DINODEBLK   32  //���д���i�ڵ㹲ռ32�������
#define FILEBLK     512 //����512��Ŀ¼�ļ������
#define NICFREE     50  //�������п��п�������������
#define NICINOD     50  //�������п���i�ڵ���������
#define DINODESTART 2*BLOCKSIZ  //i�ڵ���ʼ��ַ
#define DATASTART   (2+DINODEBLK)*BLOCKSIZ  //Ŀ¼���ļ�����ʼ��ַ

/*�ļ�����*/
#define DIEMPTY     00000
#define DIFILE      01000
#define DIDIR       02000

/*�û�Ȩ��*/
#define UDIREAD     00001
#define UDIWRITE    00002
#define UDIEXICUTE  00004

/*��Ȩ��*/
#define GDIREAD     00010
#define GDIWRITE    00020
#define GDIEXICUTE  00040

/*����Ȩ��*/
#define ODIREAD     00100
#define ODIWRITE    00200
#define ODIEXICUTE  00400

/*�ļ�����Ȩ��*/
#define READ        1
#define WRITE       2
#define EXICUTE     3
#define DEFAULTMODE 00777

#define SUPDATE     00001//�������޸�
#define IUPDATE     00002//i�ڵ��޸�

/*�ļ���дģʽ*/
#define FREAD       00001
#define FWRITE      00002
#define FAPPEND     00004

#define DISKFULL    65535//������

#define ARGVNUM     10//������������һ
#define ARGVSIZ     DIRSIZ*2

#define SEEK_SET    0//�ļ�����

#define NOT_FOUND -1//δ�ҵ���־

enum status
{
	OK,ERR
};
typedef struct Inode//�ڴ�i�ڵ�
{
	//struct Inode *i_forw;
	//struct Inode *i_back;
	char i_flag;
	unsigned int i_ino;//���������ڵ��־
	unsigned int i_count;//���ü���
	unsigned short di_number;//�����ļ�������Ϊ0ʱ����ɾ�����ļ�
	unsigned short di_mode;//��ȡȨ��
	unsigned short di_uid;//�û�id
	unsigned short di_gid;//��id
	unsigned short di_size;//�ļ���С
	unsigned int di_addr[NADDR];//ռ��������
}Inode;

typedef struct Dinode//����i�ڵ�
{
	unsigned short di_number;
	unsigned short di_mode;
	unsigned short di_uid;
	unsigned short di_gid;
	unsigned long di_size;
	unsigned int di_addr[NADDR];
}Dinode;

typedef struct Direct//Ŀ¼��
{
	char d_name[DIRSIZ];//����
	unsigned int d_ino;//i�ڵ��
}Direct;

typedef struct SuperBlock//������
{
	unsigned short s_isize;//�����ڵ�����
	unsigned long s_fsize;//���ݿ����
	unsigned int s_nfree;//���п����
	unsigned short s_pfree;//���п�ָ��
	unsigned int s_free[NICFREE];//���п��ջ

	unsigned int s_ninode;//���������ڵ���
	unsigned short s_pinode;//���������ڵ�ָ��
	unsigned int s_inode[NICINOD];//���������ڵ�����
	unsigned int s_rinode;//���������ڵ�
	char s_fmod;//�������޸ı�־
}SuperBlock;

typedef struct user_table//����ṹ
{
	char p_name[UNAME];//�û���
	unsigned short p_uid;//�û�id
	unsigned short p_mode;//�û�Ȩ��
	unsigned short p_gid;//��id
	bool sudo;//����Աyes or no
	char password[PWDSIZ];//����
}user_table;

typedef struct Dir//Ŀ¼�ṹ
{
	struct Direct direct[DIRNUM];//Ŀ¼������
	int size;//Ŀ¼��ǰ��С
}Dir;


typedef struct File
{
	char f_flag;//�ļ�������־
	unsigned int f_count;//���ü���
	struct Inode *f_inode;//ָ���ڴ������ڵ�
	unsigned long f_off;//��/дָ��
}File;

typedef struct user//�û��ṹ
{
	string u_name;//�û���
	unsigned short u_default_mode;//�û�Ȩ��
	unsigned short u_uid;
	unsigned short u_gid;
	Dir cur_dir;
	vector<string> path;
	bool sudo;//����Աyes or no
	unsigned short u_ofile[NOFILE];//�û����ļ���
	user() {}
	user(user_table u) {
		this->u_default_mode = u.p_mode;
		this->u_gid = u.p_gid;
		this->u_name = u.p_name;
		this->u_uid = u.p_uid;
		this->sudo = u.sudo;
	}
}user;

class FileSystem
{
public:
	SuperBlock superblock;
	unordered_map<unsigned short, user> users;
	unordered_map<int, Inode*> Hinode;
	Dir root_dir;
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
extern void chdir(const char * dirname);
extern int creat(const char * filename, unsigned short mode);
extern void close(unsigned short cfd);
extern bool Delete(const char *filename);
extern int open(const char * filename, unsigned short openmode);
extern unsigned int read(int fd1, char * buf, unsigned int size);
extern bool write(int fd1, char * buf, unsigned int size);

