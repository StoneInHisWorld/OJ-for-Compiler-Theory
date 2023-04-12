#pragma once
#include<iostream>
#include <string>
#include <set>

#define MAX 100
#define MaxVtNum 20 /*终结符最大的数目*/
#define MaxVnNum 20 /*非终结符最大的数目*/
#define MaxPLength 20 /*产生式的右部最大长度*/
#define EP '~' // 表示epsilon
#define TER char // 终结符要求只能有一个字符
#define NTER string // 非终结符可以有多个字符
//typedef char TER;// 表示终结符
//typedef string NTER;// 表示非终结符



//struct product
//{
//	int length; /*产生式右部的文法符号串的长度*/
//	char left; /*产生式左部的非终结符*/
//	char right[MaxPLength]; /*产生式右部的文法符号串*/
//}p[100];


//void collect_TandNT(product_s p);
//struct set
//{
//	int n;
//	char elm[100];
//}first[MAX], follow[MAX];

//TER termin[MaxVtNum];
//char non_termin[MaxVnNum];
//std::set<TER> TER_set;
//std::set<NTER> NTER_set;
//int n;
//int VtNum;
//int VnNum;

//inline bool isTer(char x);
//bool exist(char x);
//void read();
//void show();
//int char_id(char x);
//bool in(struct set &st, char id);
//void add(struct set &st, char e);
//void compute_first();
//void print_first(struct set *st);


