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


//将手机当前屏幕的xml文件提取到pcPath所在的位置,成功返回0
int pullXmltoPc(const char* pcPath){
	if (getCurrentScreen() != 0){
		return -1;
	}
	if (PullFileSys("/sdcard/window_dump.xml",pcPath) != 0){
		return -2;
	}
	return 0;
}


//遍历读取所有节点，只对最底层节点做处理，得到xy信息，成功找到返回结果，没找到返回NULL
char* getInfoFromNodes(TiXmlElement *pCurrentNode,string textName)
{
	/*//如果xyPos不等于""，说明已经找到xy位置坐标
	if (strcmp(xyPos, "") != 0) {
		return 1;
	}*/
	//为NULL时表示一次递归完成
	if (NULL == pCurrentNode)
	{
		return NULL;
	}
	else if (pCurrentNode->NoChildren())//没有子结点则是最底层的结点
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
	else if (!pCurrentNode->NoChildren())//有子结点则每个子结点都递归处理
	{
		TiXmlElement * pChilds = pCurrentNode->FirstChildElement();//获取第一个子结点
		char* res = getInfoFromNodes(pChilds,textName);//递归子结点
		if (res != NULL && strcmp(res,"")!=0) {
			return res; 
		}
		pChilds = pChilds->NextSiblingElement();//获取刚显示的结点的同级下一个结点
		while (NULL != pChilds)//递归处理此结点下的所有结点
		{
			res = getInfoFromNodes(pChilds,textName);
			if (res != NULL&&strcmp(res, "") != 0) {
				return res;
			}
			pChilds = pChilds->NextSiblingElement();
		}
		return NULL;//此结点所有子结点都处理完毕
	}
}

//解析坐标，并得到坐标中的中心点的xy值,成功返回0
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

//根据textName,由xmlPath指向的文档中计算该按钮的中心位置，存储到xy中，成功返回1,未找到返回0，-1说明加载文档失败，-2说明解析坐标失败
int getXyFromXmlByText(char* xmlPath,string textName,int* x,int* y){
	TiXmlDocument mydoc(xmlPath);//xml文档对象
	bool loadOk = mydoc.LoadFile();//加载文档
	if (!loadOk){
		return -1;
	}
	TiXmlElement* rootElement = mydoc.RootElement();//根元素
	char* xyPos = getInfoFromNodes(rootElement,textName);
	//成功找到对应按钮
	if (xyPos != NULL && strcmp(xyPos, "") != 0) {
		//解析坐标成功
		if (getXyPos(xyPos, x, y) == 0) {
			free(xyPos);
			return 1;
		}
		//否则说明解析坐标失败，返回-2
		free(xyPos);
		return -2;
	}
	//为空说明未找到相关按钮，返回0
	else {
		return 0;
	}
}

/*
获取xy的坐标
pcXmlPath：从手机提取xml到客户端的路径
textName：text的值
x,y为按钮中心点的坐标
返回值为1说明成功找到，为0说明未找到，负值说明遇到错误
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
	//如果找到返回1，没找到返回0
	if (res == 1 || res == 0) {
		return res;
	}
	//getXyFromXmlByText错误返回-2
	return -2;
}

int main() {
	const char* pcPath = "d:/test";
	string textName = "删除";
	int x, y;
	int res = getScreenXY(pcPath, textName, &x, &y);
	if (res == 1) {
		autoClick(x, y);
	}
	else if (res == 0) {
		cout << "未找到相关按钮" << endl;
	}
	else {
		cout << "出现程序错误" << endl;
	}
	return 0;
}