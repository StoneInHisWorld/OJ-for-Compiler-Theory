// ԭ���ߣ���Ԫ
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#define KEY_ "None" // �Զ����ʶ�ؼ���
#pragma warning(disable: 4996)

using namespace std;

typedef struct WORD{ //�ʵĽṹ����Ԫ����ʽ
	int typenum; //�����ֱ�
	char *word;//���������ֵ
}WORD;

char input[1024];//��¼������ַ���
char token[128] = "";//�ַ�����¼
int p_input;
int p_token;
char ch;//���������¼�ַ�
char * rwtab[] = { (char *)"begin", (char *)"end", (char *)KEY_ }; // �Զ���ġ��ؼ��֡�

WORD * scanner();//ɨ�躯������