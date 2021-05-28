#include "Head.h"

int namei(const Dir& dir,const char *name) //��dir��������Ϊname��Ŀ¼���i�ڵ��
{
	int i, notfound = 1;
	/*cout<<"---------------------------------------------------------------------------------------------------"<<endl;
	cout<<"---------------------------------------------------------------------------------------------------"<<endl;*/
	for (i = 0; ((i < dir.size) && (notfound)); i++)
	{
		//cout<<"i="<<i<<"                 <name> = "<<name<<"                 <d_name>:"<<dir0.direct[i].d_name<<"                 <d_ino>"<<dir0.direct[i].d_ino<<endl;
		if ((!strcmp(dir.direct[i].d_name, name)) && (dir.direct[i].d_ino != 0))
			return (dir.direct[i].d_ino); /* find */
	}
	/* not find */
	return NOT_FOUND;
};

int iname(Dir& dir,const char * name)	//����dirĿ¼����Ϊname�Ŀ�Ŀ¼����dir�е��±�
{
	int i, notfound = 1;
	for (i = 0; ((i < DIRNUM) && (notfound)); i++)
	{
		if (dir.direct[i].d_ino == 0)
		{
			notfound = 0;
			break;
		}
	}
	if (notfound)
	{
		printf("\nThe current directory is full! !\n");
		return NOT_FOUND;
	}
	else
	{
		strcpy(dir.direct[i].d_name, name);
		dir.direct[i].d_ino = 1;
		return i;
	}
}