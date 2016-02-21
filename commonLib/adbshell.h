/*
ͨ���ܵ�����adb shell	--by sing
*/

#pragma once
#include <windows.h>
#include <string>
using namespace std;

class CAdbshell
{
public:
	CAdbshell(void);
	~CAdbshell(void);
public:
	//����adb shell������ɹ������#״̬������ʹ��RunCmdִ������
	BOOL Start();

	//�������������adb shell״̬�µ��������������ж�������\n�ָ�
	BOOL RunCmd(const string& strCmdline);

	//�˳�shell����״̬���رս���
	BOOL Stop();

	//��ȡ������,����ǰ����ص���Stop�Ƚ���
	string GetOutput();

	inline static string& trim(string& text)  
	{  
		if(!text.empty())  
		{  
			text.erase(0, text.find_first_not_of(" \n\r\t"));  
			text.erase(text.find_last_not_of(" \n\r\t") + 1);  
		}  
		return text;  
	}  

public:
	BOOL Loop();

private:
	HANDLE	hThread;
	string  m_strOutput;
	//HANDLE	m_hProcess;

	//ָʾ�߳�׼���ã����Խ�������������ź���
	HANDLE	m_hEvent;

	HANDLE	m_hReadPipe;
	HANDLE	m_hReadPipe2;
	HANDLE	m_hWritePipe;
	HANDLE	m_hWritePipe2;
};
