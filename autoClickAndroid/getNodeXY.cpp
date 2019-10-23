#include<stdlib.h>
#include<iostream>
#include<Windows.h>
#include"exeCmd.h"
#include"tinystr.h"
#include"tinyxml.h"

using namespace std;

string Utf8ToGbk(const char *src_str)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, src_str, -1, NULL, 0);
	wchar_t* wszGBK = new wchar_t[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, src_str, -1, wszGBK, len);
	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char* szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
	string strTemp(szGBK);
	if (wszGBK) delete[] wszGBK;
	if (szGBK) delete[] szGBK;
	return strTemp;
}


//���ֻ���ǰ��Ļ��xml�ļ���ȡ��pcPath���ڵ�λ��,�ɹ�����0
int pullXmltoPc(const char* pcPath){
	if (getCurrentScreen() != 0){
		return -1;
	}
	if (PullFileSys("/sdcard/window_dump.xml",pcPath) != 0){
		return -2;
	}
	return 0;
}


//������ȡ���нڵ㣬ֻ����ײ�ڵ��������õ�xy��Ϣ���ɹ��ҵ����ؽ����û�ҵ�����NULL
char* getInfoFromNodes(TiXmlElement *pCurrentNode,string textName)
{
	/*//���xyPos������""��˵���Ѿ��ҵ�xyλ������
	if (strcmp(xyPos, "") != 0) {
		return 1;
	}*/
	//ΪNULLʱ��ʾһ�εݹ����
	if (NULL == pCurrentNode)
	{
		return NULL;
	}
	else if (pCurrentNode->NoChildren())//û���ӽ��������ײ�Ľ��
	{
		string res = pCurrentNode->Attribute("text");
		res = Utf8ToGbk(res.c_str());
		if (res == textName){
			string xyRes = pCurrentNode->Attribute("bounds");
			char* xyPos = (char*)malloc(50);
			strcpy(xyPos, xyRes.c_str());
			return xyPos;
		}
		return NULL;
	}
	else if (!pCurrentNode->NoChildren())//���ӽ����ÿ���ӽ�㶼�ݹ鴦��
	{
		TiXmlElement * pChilds = pCurrentNode->FirstChildElement();//��ȡ��һ���ӽ��
		char* res = getInfoFromNodes(pChilds,textName);//�ݹ��ӽ��
		if (res != NULL && strcmp(res,"")!=0) {
			return res; 
		}
		pChilds = pChilds->NextSiblingElement();//��ȡ����ʾ�Ľ���ͬ����һ�����
		while (NULL != pChilds)//�ݹ鴦��˽���µ����н��
		{
			res = getInfoFromNodes(pChilds,textName);
			if (res != NULL&&strcmp(res, "") != 0) {
				return res;
			}
			pChilds = pChilds->NextSiblingElement();
		}
		return NULL;//�˽�������ӽ�㶼�������
	}
}

//�������꣬���õ������е����ĵ��xyֵ,�ɹ�����0
int getXyPos(char* source,int* x, int* y) {
	int x1, x2, y1, y2;
	char* res = strtok(source, ",");
	if (!res) {
		return -1;
	}
	res = res + 1;
	x1 = atoi(res);

	res = strtok(NULL, "]");
	if (!res) {
		return -2;
	}
	y1 = atoi(res);

	res = strtok(NULL, ",");
	if (!res) {
		return -3;
	}
	res = res + 1;
	x2 = atoi(res);
	
	res = strtok(NULL, "]");
	if (!res) {
		return -4;
	}
	y2 = atoi(res);

	*x = (x2 - x1) / 2 + x1;
	*y = (y2 - y1) / 2 + y1;
	return 0;
}

//����textName,��xmlPathָ����ĵ��м���ð�ť������λ�ã��洢��xy�У��ɹ�����1,δ�ҵ�����0��-1˵�������ĵ�ʧ�ܣ�-2˵����������ʧ��
int getXyFromXmlByText(char* xmlPath,string textName,int* x,int* y){
	TiXmlDocument mydoc(xmlPath);//xml�ĵ�����
	bool loadOk = mydoc.LoadFile();//�����ĵ�
	if (!loadOk){
		return -1;
	}
	TiXmlElement* rootElement = mydoc.RootElement();//��Ԫ��
	char* xyPos = getInfoFromNodes(rootElement,textName);
	//�ɹ��ҵ���Ӧ��ť
	if (xyPos != NULL && strcmp(xyPos, "") != 0) {
		//��������ɹ�
		if (getXyPos(xyPos, x, y) == 0) {
			free(xyPos);
			return 1;
		}
		//����˵����������ʧ�ܣ�����-2
		free(xyPos);
		return -2;
	}
	//Ϊ��˵��δ�ҵ���ذ�ť������0
	else {
		return 0;
	}
}

/*
��ȡxy������
pcXmlPath�����ֻ���ȡxml���ͻ��˵�·��
textName��text��ֵ
x,yΪ��ť���ĵ������
����ֵΪ1˵���ɹ��ҵ���Ϊ0˵��δ�ҵ�����ֵ˵����������
*/
int getScreenXY(const char* pcXmlPath, string textName, int* x, int* y){
	int res = pullXmltoPc(pcXmlPath);
	if (res != 0)
	{
		return -1;
	}
	char path[50];
	strcpy(path, pcXmlPath);
	strcat(path,"/window_dump.xml");
	res = getXyFromXmlByText(path, textName, x, y);
	//����ҵ�����1��û�ҵ�����0
	if (res == 1 || res == 0) {
		return res;
	}
	//getXyFromXmlByText���󷵻�-2
	return -2;
}

int main() {
	const char* pcPath = "d:/test";
	string textName = "ɾ��";
	int x, y;
	int res = getScreenXY(pcPath, textName, &x, &y);
	if (res == 1) {
		autoClick(x, y);
	}
	else if (res == 0) {
		cout << "δ�ҵ���ذ�ť" << endl;
	}
	else {
		cout << "���ֳ������" << endl;
	}
	return 0;
}