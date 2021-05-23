#include "Head.h"
void CopyDirectorSonFunc(int DirectorIndex) 
{
	director myDirector = myFileSystem.SystemDirector[DirectorIndex];//��ǰ���ڵ��ļ���
    //��ʼ���������ļ�����Ϣ
	director temp;
	temp.DirectorId = myFileSystem.SystemDirector.size();
	temp.DirectorName = myDirector.DirectorName;
	temp.FileOwner = myDirector.FileOwner;
	temp.ParentDirector = myFileSystem.DirectorIndex;
	//��Ӹ����ļ����е��ļ�
	list<int>::iterator it = myDirector.FileList.begin();
	while (it!=myDirector.FileList.end())
	{
		temp.FileList.push_back(*it);
		it++;
	}
	//��ӱ����Ƶ��ļ���
	myFileSystem.SystemDirector[myFileSystem.DirectorIndex].DirectorList.push_back(temp.DirectorId);
	myFileSystem.SystemDirector.push_back(temp);
	//���뵱ǰĿ¼
	myFileSystem.DirectorIndex = temp.DirectorId;
	//��Ӹ����ļ����е��ļ���
	it = myDirector.DirectorList.begin();
	while (it!=myDirector.DirectorList.end())
	{
		if ((*it) != -1)
		{
			CopyDirectorSonFunc(*it);
		}
		it++;
	}
}

bool CopyDirector(string DirectorName,string path)
{
	int CurrentDirectorIndex = myFileSystem.DirectorIndex;//���浱ǰ����Ŀ¼
	int DirectorIndex = SearchDirector(DirectorName);//���渴��Ŀ¼��Ŀ¼��
	myFileSystem.DirectorIndex = 0;//�ص���Ŀ¼
	if (DirectorIndex == -1)
	{
		cout << "��ǰĿ¼�����ڸ��ļ���" << endl;
		return false;
	}
	string DirectorOwner = myFileSystem.SystemDirector[DirectorIndex].FileOwner;
	if (DirectorOwner != myFileSystem.CurrentUser.UserName&&DirectorOwner != "public")//���Ȩ��
	{
		cout << "��û�и��ļ��еĲ���Ȩ��" << endl;
		return false;
	}
	if (!ParsePath(path))
	{
		cout << "����ʧ��" << endl;
		return false;
	}
	CopyDirectorSonFunc(DirectorIndex);
	cout << "���Ƴɹ�" << endl;
	return true;
}
