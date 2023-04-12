//#include "FirstSet.h"
//
////void show()
////{
////	int i;
////	printf("用户所输入的产生式为：\n");
////	for (i = 1; i <= n; i++)
////		printf("(%d)  %c->%s\n", i, p[i].left, p[i].right);
////}
////
//int char_id(char x)
//{
//	int i;
//	// 如果是非终结符
//	if (!isTer(x)) {
//		for (i = 1; i <= VnNum; i++)
//			if (non_termin[i] == x)
//				return i;
//	}
//	// 如果是终结符则>1000
//	for (i = 1; i <= VtNum; i++)
//		if (termin[i] == x)
//			return i + 1000;
//	return -1;
//}
////是否在集合st中
//bool in(set & st, char id)
//{
//	int i;
//	for (i = 1; i <= st.n; i++)
//		if (st.elm[i] == id)
//			return true;
//	return false;
//}
////
////void add(set & st, char e)
////{
////	st.n++;
////	st.elm[st.n] = e;
////}
////
//void compute_first()
//{
//	int i, j, k, idl, idr;
//	bool inc;
//	inc = true;
//	while (inc) {
//		inc = false;
//		// 遍历所有产生式
//		for (i = 1; i <= n; i++) {
//			// 提取左部的id
//			idl = char_id(p[i].left);
//			// 遍历右部
//			for (j = 0; p[i].right[j]; j++) {
//				// 获取到当前字符的id
//				idr = char_id(p[i].right[j]);
//				// 如果是终结符
//				if (idr > 1000) {
//					// 如果该终结符不在集合中则加入左部的FIRST
//					if (!in(first[idl], p[i].right[j])) {
//						add(first[idl], p[i].right[j]);
//						inc = true;
//					}
//					// 有终结符则FIRST集扩充完毕
//					break;
//				}
//				// 如果是非终结符
//				else { 
//					// 遍历该非终结符的first集
//					for (k = 1; k <= first[idr].n; k++) {
//						// 如果该非终结符的first集的该元素不在左部的FIRST集中
//						// 且该元素不等于epsilon
//						if (!in(first[idl], first[idr].elm[k]) && first[idr].elm[k] != '~') {
//							// 将该元素加入左部的FIRST集合中
//							add(first[idl], first[idr].elm[k]);
//							inc = true;
//						}
//					}
//					// 如果左部的FIRST集合没有epsilon
//					if (!in(first[idl], '~'))
//						break;
//				}
//			}
//			// 如果产生式右部为epsilon
//			if (p[i].right[j] == 0 && !in(first[idl], '~')) {
//				add(first[idl], '~');
//				inc = true;
//			}
//		}
//	}
//}
////
////void print_first(set * st)
////{
////	int i, j;
////	puts("\n");
////	for (i = 1; i <= VnNum; i++) {
////		printf("%s(%c):   ", "FIRST", non_termin[i]);
////		for (j = 1; j <= st[i].n; j++)
////			printf("%c  ", st[i].elm[j]);
////		puts("");
////	}
////}
