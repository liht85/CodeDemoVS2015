#pragma once
#include "Thread.h"
class MyThread :public Thread
{
public:
	MyThread();
	~MyThread();
	void * run(void *);


	typedef void fonFinish();
	;
	//传入函数指针，下载完成后调用
	void setOnFinish(fonFinish *pTest = nullptr);
	// 线程结束后的  回调函数
	void(*onFinish)();
};

