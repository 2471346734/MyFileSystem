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

#define BLOCKSIZ    128 //ÿ���С
#define SUPERSIZ	512
#define DIRSIZ      12  //ÿ��Ŀ¼�����ֲ�����ռ�ֽ��������i�ڵ��4���ֽ�
#define DIRITEM     sizeof(struct Direct)//ÿ��Ŀ¼����ռ�ֽ���
#define DIRNUM      BLOCKSIZ/DIRITEM //ÿ��Ŀ¼�����������Ŀ¼�������ļ�����
#define UNAME       20  //�û�������
#define PWDSIZ      12  //������
#define PWDNUM      10  //������10�������¼
#define NOFILE      20  //ÿ���û����ɴ�20���ļ������û����ļ����������
#define NADDR       10  //ÿ��i�ڵ����ָ�򼸿飬addr[0]~addr[NADDR]
#define USERNUM     10  //����������û���¼
#define DINODESIZ   sizeof(struct Dinode)//ÿ������i�ڵ���ռ�ֽ�

#define DINODEBLK   16  //���д���i�ڵ㹲ռ32�������
#define FILEBLK     48 //����512��Ŀ¼�ļ������
#define NICFREE     50  //�������п��п�������������
#define NICINOD     50  //�������п���i�ڵ���������
#define DINODESTART 2*SUPERSIZ  //i�ڵ���ʼ��ַ
#define DATASTART   DINODESTART+DINODEBLK*BLOCKSIZ  //Ŀ¼���ļ�����ʼ��ַ
#define TOTALSIZ	DATASTART+BLOCKSIZ*FILEBLK		//�ļ����ܴ�С

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
const unsigned short FREAD = 00001;
const unsigned short FWRITE = 00002;
const unsigned short FAPPEND = 00004;

#define DISKFULL    65535//������

#define ARGVNUM     10//������������һ
#define ARGVSIZ     DIRSIZ*2

#define SEEK_SET    0//�ļ�����

#define NOT_FOUND 65535//δ�ҵ���־

enum status
{
	OK,ERR
};
typedef struct Inode//�ڴ�i�ڵ�
{
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
	bool operator==(const File& b) {
		return this->f_inode->i_ino == b.f_inode->i_ino;
	}
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
	vector<unsigned short> u_ofile;//�û����ļ���
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