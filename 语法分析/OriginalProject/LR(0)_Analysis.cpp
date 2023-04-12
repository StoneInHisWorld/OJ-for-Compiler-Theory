#include <stdio.h>
#include <string.h>

#define VtNum 6
#define VnNum 3
#define PNum 7
#define StateNum 12

// 使用字符串记录动作
char *action[StateNum][VtNum] = {
    "s5#", NULL, NULL, "s4#", NULL, NULL,
    NULL, "s6#", NULL, NULL, NULL, "acc",
    NULL, "r2", "s7#", NULL, "r2#", "r2#",
    NULL, "r4#", "r4#", NULL, "r4#", "r4#",
    "s5#", NULL, NULL, "s4#", NULL, NULL,
    NULL, "r6#", "r6#", NULL, "r6#", "r6#",
    "s5#", NULL, NULL, "s4#", NULL, NULL,
    "s5#", NULL, NULL, "s4#", NULL, NULL,
    NULL, "s6#", NULL, NULL, "s11#", NULL,
    NULL, "r1#", "s7#", NULL, "r1#", "r1#",
    NULL, "r3#", "r3#", NULL, "r3#", "r3#",
    NULL, "r5#", "r5#", NULL, "r5#", "r5#"};
// 自己程序使用的ACTION表
    char *action1[StateNum][VtNum] = {
        // #, (, ), *, +, i, 
        NULL, "s4", NULL, NULL, NULL, "s5"
        "acc", NULL, NULL, NULL, "s6", NULL,
        "r2", NULL, "r2", "s7", "r2", NULL,
        "r4", NULL, "r4", "r4", "r4", NULL,
        NULL, "s4", NULL, NULL, NULL, "s5",
        "r6", NULL, "r6", "r6", "r6", NULL,
        NULL, "s4", NULL, NULL, NULL, "s5",
        NULL, "s4", NULL, NULL, NULL, "s5",
        NULL, NULL, "s11", NULL, "s6", NULL,
        "r1", NULL, "r1", "s7", "r1", NULL,
        "r3", NULL, "r3", "r3", "r3", NULL,
        "r5", NULL, "r5", "r5", "r5", NULL
    };

int GOTOTable[StateNum][VnNum] = {
    1, 2, 3,
    0, 0, 0,
    0, 0, 0, 
    0, 0, 0,
    8, 2, 3, 
    0, 0, 0, 
    0, 9, 3, 
    0, 0, 10,
    0, 0, 0, 
    0, 0, 0, 
    0, 0, 0, 
    0, 0, 0};
// 自己程序使用的GOTO表
    int GOTOTable1[StateNum][VnNum] = {
    1, 3, 2,
    0, 0, 0,
    0, 0, 0, 
    0, 0, 0,
    8, 3, 2, 
    0, 0, 0, 
    0, 3, 9, 
    0, 10, 0,
    0, 0, 0, 
    0, 0, 0, 
    0, 0, 0, 
    0, 0, 0};
char termin[VtNum] = {'i', '+', '*', '(', ')', '#'};
char non_termin[VnNum] = {'E', 'T', 'F'};
char *LR[PNum] = {"S->E#", "E->E+T#", "E->T#", "T->T*F#", "T->F#", "F->(E)#", "F->i#"};
int state_stack[StateNum];
char symbol_stack[StateNum];
char input[StateNum];

void main()
{
    int m, n, g, i;
    int nextState, len_proToReduce, curState;
    int id_curState, Step, id_curChar, id_proToReduce, id_nterInPro;
    char cur_char, c;
    int top_stateStack = 0;
    int top_symbolStack = 0;
    // 输入栈栈顶指针
    int top_input = 0;
    // 接受输入字符串
    printf("本程序只能对由'i','+','*','(',')'构成的以'#'结束的字符串进行分析。\n");
    printf("请输入要分析的字符串:");
    do
    {
        scanf("%c", &c);
        input[top_input] = c;
        top_input = top_input + 1;
    } while (c != '#');
    // 根据分析表分析输入字符串
    int top = 0;
    id_curState = state_stack[0] = 0;
    symbol_stack[0] = '#';
    // 步骤
    Step = 0;
    curState = 0;
    printf("步骤\t状态栈\t\t符号栈\t\t输入串\t\tACTION\tGOTO\n");
    do
    {
        Step++;
        printf("%d\t", Step);

        id_curState = curState;
        cur_char = input[top]; // 获取输入字符
        // 打印状态栈
        m = 0;
        while (m <= top_stateStack)
        {
            printf("%d", state_stack[m]);
            m = m + 1;
        }
        printf("\t\t");
        // 打印符号栈
        n = 0;
        while (n <= top_symbolStack)
        {
            printf("%c", symbol_stack[n]);
            n = n + 1;
        }
        printf("\t\t");
        // 打印剩余输入串
        g = top;
        while (g <= top_input)
        {
            printf("%c", input[g]);
            g = g + 1;
        }
        printf("\t\t");
        // 获取cur_char的下标，并检查输入字符串是否有非终结符
        id_curChar = 0;
        while (cur_char != termin[id_curChar] && id_curChar <= VtNum)
            id_curChar++;
        if (id_curChar == VtNum && cur_char != termin[id_curChar])
        {
            printf("输入字符串不是由终结符组成\n");
            return;
        }
        // 如果ACTION表中没有对于当前状态，遇到当前字符应当执行的动作，报告出错
        if (action[id_curState][id_curChar] == NULL)
        {
            printf("error\n");
            return;
        }
        else if (action[id_curState][id_curChar][0] == 's')
        {
            // 如果动作是“移进”，则计算得到转换的状态
            if (action[id_curState][id_curChar][2] != '#')
                curState = (action[id_curState][id_curChar][1] - '0') * 10 +
                           action[id_curState][id_curChar][2] - '0';
            else
                curState = action[id_curState][id_curChar][1] - '0';
            // 当前状态与当前字符入栈
            // top_stateStack++;
            // top_symbolStack++;
            state_stack[++top_stateStack] = curState;
            symbol_stack[++top_symbolStack] = cur_char;
            top++;
            // 打印当前执行的动作
            i = 0;
            while (action[id_curState][id_curChar][i] != '#')
            {
                printf("%c", action[id_curState][id_curChar][i]);
                i++;
            }
            printf("\n");
        }
        if (action[id_curState][id_curChar][0] == 'r')
        {
            // 如果是“归约”动作，打印执行的动作
            i = 0;
            while (action[id_curState][id_curChar][i] != '#')
            {
                printf("%c", action[id_curState][id_curChar][i]);
                i++;
            }
            // 获取归约使用的产生式id
            id_proToReduce = action[id_curState][id_curChar][1] - '0';
            // 获取产生式左部id
            id_nterInPro = 0;
            while (LR[id_proToReduce][0] != non_termin[id_nterInPro])
                id_nterInPro++;
            len_proToReduce = strlen(LR[id_proToReduce]) - 4; // 4是去除了非终结符、->、#
            // 产生式右部出符号栈，状态栈弹出右部长度的个数的状态
            top_stateStack = top_stateStack - len_proToReduce + 1;
            top_symbolStack = top_symbolStack - len_proToReduce + 1;
            // 获取出栈后状态，根据GOTO表变更状态入状态栈，产生式左部入符号栈
            id_curState = state_stack[top_stateStack - 1];
            nextState = GOTOTable[id_curState][id_nterInPro];
            state_stack[top_stateStack] = nextState;
            symbol_stack[top_symbolStack] = LR[id_proToReduce][0];
            curState = nextState;
            printf("\t");
            printf("%d\n", nextState);
        }
        // 在输入串不被acc之前一直分析
    } while (action[id_curState][id_curChar] != "acc");
    printf("acc\n");
}
