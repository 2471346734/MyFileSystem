#include  "Head.h"

bool ParsePath(string path)
{
	int start = 0;//��ǰ����·���Ŀ�ʼָ��
	int end = 0;//��ǰ����·���Ľ���ָ��
	int flag = 1;
	while (end < strlen(path.c_str()))//δ��pathβ��
	{
		if (path[end] == '/')
		{
			string director = path.substr(start, end - start);//��ǰĿ¼
			if (flag)//������һ�ν���ѭ��ʱ������Ƿ���Home·����
			{
				if (director != "Home")
				{
					cout << "�����·������ȷ��" << endl;
					return false;
				}
				flag = 0;
			}
			else
			{
				if (!EnterLowDir(director))//���������Ŀ¼��
				{
					return false;
				}
			}
			start = end + 1;
		}
		end++;
	}
	string director = path.substr(start, end - start);
	if (!EnterLowDir(director))
	{
		return false;
	}
	return true;
}

bool CopyFile(string FileName, string path)
{
	int DirectorIndex = myFileSystem.DirectorIndex;//����Ŀ¼��
	int FileIndex = SearchFile(FileName);//�ļ���
	if (FileIndex == -1)
	{
		cout << "��ǰĿ¼�����ڸ��ļ�" << endl;
		return false;
	}
	string FileOwner = myFileSystem.SystemFile[FileIndex].FileOwner;
	if (FileOwner != myFileSystem.CurrentUser.UserName&&FileOwner != "public")//���Ȩ��
	{
		cout << "��û�и��ļ��Ĳ���Ȩ��" << endl;
		return false;
	}
	if (!ParsePath(path))//����·��
	{
		cout << "����ʧ��" << endl;
		return false;
	}
	myFileSystem.SystemDirector[myFileSystem.DirectorIndex].FileList.push_back(FileIndex);//�Ѹ��ļ���ӵ����Ƶ��ļ�����
	myFileSystem.DirectorIndex = DirectorIndex;//�ָ�Ŀ¼
	cout << "���Ƴɹ�" << endl;
}