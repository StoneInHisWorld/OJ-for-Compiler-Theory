#include "declaration.h"

int main(){
	WORD* oneword = new WORD;
	ofstream fout;
	FILE *fp;
	fout.open("output.txt");//写文件操作
	if ((fp = freopen("testfile.txt", "r", stdin)) == NULL){
		//读文件操作，若读取失败直接结束程序，成功则转换成标准输入
		cout << "Not found file!" << endl;
		return 0;
	}
	else{
		while ((scanf("%s", &input)) != EOF){//对文件所有字符进行读取，利用格式化输入直接跳过空格和回车
			p_input = 0;
			// 识别一个单词
			while (1){
				oneword = scanner();
				if (oneword->typenum == -2)//对应的符号位\0字符串结束符
					break;
				else if (oneword->typenum == -1)//检测到的字符（串）在此处属于非法字符，输出error
					//cout << oneword->word << endl;
					fout << oneword->word << endl;
				else//合法字符，进行输出
					//cout << oneword->typenum << ' ' << oneword->word << endl;
					fout << oneword->typenum << ' ' << oneword->word << endl;
			}
		}
	}
	//system("pause");
	return 0;
}

//从输入缓冲区读取一个字符到ch中
char m_getch(){
	ch = input[p_input];
	p_input++;
	return ch;
}

//去掉空白符号，在使用非格式化输入时必须使用
void getbc(){
	while (ch == ' ' || ch == '\n'){
		ch = input[p_input];
		p_input++;
	}
}

//拼接单词
void concat(){
	token[p_token] = ch;
	p_token++;
	token[p_token] = '\0';
}

//判断是否字母
int letter(){
	if ((ch >= 'a'&&ch <= 'z') || (ch >= 'A'&&ch <= 'Z'))
		return 1;
	return 0;
}

//判断是否数字
int digit(){
	if (ch >= '0'&&ch <= '9')
		return 1;
	return 0;
}

//检索关键字表格
int reserve(){
	int i = 0;
	while (strcmp(rwtab[i], KEY_)){
		if (!strcmp(rwtab[i], token))
			return i + 1;
		i++;
	}
	return 10;		// 如果不是关键字，则返回种别码10，标识符
}

//回退一个字符
void retract(){
	p_input--;
}

//词法扫描程序
WORD * scanner(){
	WORD * myword = new WORD;
	myword->typenum = 10;  // 初始值
	myword->word = (char *)"";	// 初始化字符串
	p_token = 0;   //单词缓冲区指针
	m_getch();	// 获取一个字符
	getbc();	//	去掉空白

	if (letter()){	// 判断读取到的首字母是字母
		//如int
		while (letter() || digit()){
			concat(); //连接
			m_getch();
		}
		retract(); //回退一个字符
		myword->typenum = reserve();//判断是否为关键字，返回种别码
		myword->word = token;
		return myword;
	}
	else if (digit()){  //判断读取到的单词首字符是数字
		while (digit()){ //所有数字连接起来
			concat();
			m_getch();
		}
		retract();
		//数字单词种别码统一为11，单词自身的值为数字本身
		myword->typenum = 11;
		myword->word = token;
		return(myword);
	}
	else switch (ch){
	case '=':
		myword->typenum = 18;
		myword->word = (char *)"=";
		return(myword);
	case '+':
		myword->typenum = 13;
		myword->word = (char *)"+";
		return(myword);
	case '-':
		myword->typenum = 14;
		myword->word = (char *)"-";
		return(myword);
	case '/':
		myword->typenum = 16;
		myword->word = (char *)"/";
		return (myword);
	case '*':
		myword->typenum = 15;
		myword->word = (char *)"*";
		return(myword);
	case '(':
		myword->typenum = 24;
		myword->word = (char *)"(";
		return(myword);
	case ')':
		myword->typenum = 25;
		myword->word = (char *)")";
		return(myword);
	case ',':
		myword->typenum = 32;
		myword->word = (char *)",";
		return(myword);
	case ';':
		myword->typenum = 17;
		myword->word = (char *)";";
		return(myword);
	case '>':
		m_getch();
		if (ch == '='){
			myword->typenum = 23;
			myword->word = (char *)">=";
			return(myword);
		}
		retract();
		myword->typenum = 22;
		myword->word = (char *)">";
		return(myword);
	case '<':
		m_getch();
		if (ch == '='){
			myword->typenum = 21;
			myword->word = (char *)"<=";
			return(myword);
		}
		else if (ch == '>'){
			myword->typenum = 20;
			myword->word = (char *)"<>";
			return(myword);
		}
		else{
			retract();
			myword->typenum = 19;
			myword->word = (char *)"<";
			return (myword);
		}
	case '#':
		myword->typenum = 0;
		myword->word = (char *)"#";
		return (myword);
	case '\0':
		myword->typenum = -2;
		myword->word = (char *)"\0";
		return (myword);
	default:
		myword->typenum = -1;
		myword->word = (char *)"error";
		return(myword);
	}
}


