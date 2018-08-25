#include "stdafx.h"
#include "MyThread.h"


MyThread::MyThread()
{
}


MyThread::~MyThread()
{
}

void * MyThread::run(void *pra)
{
	char *p = (char*)pra;
	char szBuff[1024] = { 0 };
	sprintf(szBuff, "%d--%d", GetCurrentProcessId(), GetCurrentThreadId);
	MessageBoxA(NULL, szBuff,p,NULL);
	if (onFinish != nullptr) {
		onFinish();
	}
	return NULL;
}

//传入函数指针，下载完成后调用
void MyThread::setOnFinish(fonFinish *pTest) {
	onFinish = pTest;
}