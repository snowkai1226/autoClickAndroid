#ifndef _EXECMD_H_
#define _EXECMD_H_


//��װapk
extern "C" int InstallApkSys(char *apkPath);
//ж��apk
extern "C" int UninstallApkSys(char *packageName);
//��ȡ�ļ�
extern "C" int PullFileSys(const char *fromPath, const char *toPath);

//�鿴��ǰapp�����
extern "C" int ShowApp();

//��app
extern "C" int openApp(char* iName);

//�ر�app
extern "C" int closeApp(char* iName);

//�ܵ�����
extern "C" int socketCon(char* pcPort, char* phonePort);

extern "C" int killAdb();

extern "C" int execmd(char * cmd, char *result);

extern "C" int getSerialNo(char* res);

extern "C" int getWmSize(char* res);

extern "C" int autoClick(int x, int y);

//��ȡ��ǰ��Ļ��xml�ļ�
extern "C" int getCurrentScreen();


#endif