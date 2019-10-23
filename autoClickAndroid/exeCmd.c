#include<stdlib.h>
#include<stdio.h>
//��װapk
int InstallApkSys(char *apkPath){
	char chars[1024];
	sprintf(chars, "adb install %s", apkPath);
	return system(chars);
}

//ж��apk
int UninstallApkSys(char *packageName){
	char chars[1024];
	sprintf(chars, "adb uninstall %s", packageName);
	return system(chars);
}

//��ȡ�ļ�
int PullFileSys(const char *fromPath, const char *toPath){
	char chars[1024];
	sprintf(chars, "adb pull %s %s", fromPath, toPath);
	return system(chars);
}

//�鿴��ǰapp�����
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

//pc�˺��ֻ��˻���
int socketCon(char* pcPort, char* phonePort){
	char chars[1024];
	sprintf(chars, "adb forward tcp:%s tcp:%s", pcPort, phonePort);
	return system(chars);
}

int killAdb(){
	char chars[1024] = "adb kill-server";
	return system(chars);
}

//��ȡ��ǰ��Ļ��xml�ļ�
int getCurrentScreen(){
	return system("adb shell uiautomator dump");
}

int execmd(char * cmd, char *result){
	//���建��qu
	char buffer[128];
	FILE * pipe = _popen(cmd, "r"); //�򿪹ܵ�����ִ������
	if (result == NULL){
		return 0; //����0��ʾ����Ҫ��������ֻ��Ҫִ������
	}
	if (!pipe)
		return 0; //����0��ʾ����ʧ��
	while (!feof(pipe)){  //���ܵ������result��
		if (fgets(buffer, 128, pipe)){
			strcat(result, buffer);
		}
	}
	_pclose(pipe); //�رչܵ�
	return 1;  //����1��ʾ�ɹ�
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


