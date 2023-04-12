// 原作者：纪元
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#define KEY_ "None" // 自定义标识关键字
#pragma warning(disable: 4996)

using namespace std;

typedef struct WORD{ //词的结构，二元组形式
	int typenum; //单词种别
	char *word;//单词自身的值
}WORD;

char input[1024];//记录输入的字符串
char token[128] = "";//字符串记录
int p_input;
int p_token;
char ch;//检测用来记录字符
char * rwtab[] = { (char *)"begin", (char *)"end", (char *)KEY_ }; // 自定义的“关键字”

WORD * scanner();//扫描函数声明