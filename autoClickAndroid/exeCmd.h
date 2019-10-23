#ifndef _EXECMD_H_
#define _EXECMD_H_


//安装apk
extern "C" int InstallApkSys(char *apkPath);
//卸载apk
extern "C" int UninstallApkSys(char *packageName);
//提取文件
extern "C" int PullFileSys(const char *fromPath, const char *toPath);

//查看当前app的入口
extern "C" int ShowApp();

//打开app
extern "C" int openApp(char* iName);

//关闭app
extern "C" int closeApp(char* iName);

//管道连接
extern "C" int socketCon(char* pcPort, char* phonePort);

extern "C" int killAdb();

extern "C" int execmd(char * cmd, char *result);

extern "C" int getSerialNo(char* res);

extern "C" int getWmSize(char* res);

extern "C" int autoClick(int x, int y);

//获取当前屏幕的xml文件
extern "C" int getCurrentScreen();


#endif