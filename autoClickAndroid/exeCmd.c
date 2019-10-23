#include<stdlib.h>
#include<stdio.h>
//安装apk
int InstallApkSys(char *apkPath){
	char chars[1024];
	sprintf(chars, "adb install %s", apkPath);
	return system(chars);
}

//卸载apk
int UninstallApkSys(char *packageName){
	char chars[1024];
	sprintf(chars, "adb uninstall %s", packageName);
	return system(chars);
}

//提取文件
int PullFileSys(const char *fromPath, const char *toPath){
	char chars[1024];
	sprintf(chars, "adb pull %s %s", fromPath, toPath);
	return system(chars);
}

//查看当前app的入口
int ShowApp(){
	return system("adb shell dumpsys window windows | findstr 'Current'");
}

int openApp(char* iName){
	char chars[1024];
	sprintf(chars, "adb shell am start -n %s", iName);
	return system(chars);
}

int closeApp(char* iName){
	char chars[1024];
	sprintf(chars, "adb shell pm clear %s", iName);
	return system(chars);
}

//pc端和手机端互联
int socketCon(char* pcPort, char* phonePort){
	char chars[1024];
	sprintf(chars, "adb forward tcp:%s tcp:%s", pcPort, phonePort);
	return system(chars);
}

int killAdb(){
	char chars[1024] = "adb kill-server";
	return system(chars);
}

//获取当前屏幕的xml文件
int getCurrentScreen(){
	return system("adb shell uiautomator dump");
}

int execmd(char * cmd, char *result){
	//定义缓冲qu
	char buffer[128];
	FILE * pipe = _popen(cmd, "r"); //打开管道，并执行命令
	if (result == NULL){
		return 0; //返回0表示不需要输出结果，只需要执行命令
	}
	if (!pipe)
		return 0; //返回0表示运行失败
	while (!feof(pipe)){  //将管道输出到result中
		if (fgets(buffer, 128, pipe)){
			strcat(result, buffer);
		}
	}
	_pclose(pipe); //关闭管道
	return 1;  //返回1表示成功
}

int getSerialNo(char* res){
	return execmd("adb get-serialno", res);
}

int getWmSize(char* res){
	return execmd("adb shell wm size", res);
}

int autoClick(int x, int y){
	char xPos[8];
	char yPos[8];
	itoa(x, xPos, 10);
	itoa(y, yPos, 10);
	char chars[128];
	sprintf(chars, "adb shell input tap %s %s", xPos, yPos);
	return system(chars);
}


