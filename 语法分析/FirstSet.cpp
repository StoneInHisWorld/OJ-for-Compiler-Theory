//#include "FirstSet.h"
//
////void show()
////{
////	int i;
////	printf("�û�������Ĳ���ʽΪ��\n");
////	for (i = 1; i <= n; i++)
////		printf("(%d)  %c->%s\n", i, p[i].left, p[i].right);
////}
////
//int char_id(char x)
//{
//	int i;
//	// ����Ƿ��ս��
//	if (!isTer(x)) {
//		for (i = 1; i <= VnNum; i++)
//			if (non_termin[i] == x)
//				return i;
//	}
//	// ������ս����>1000
//	for (i = 1; i <= VtNum; i++)
//		if (termin[i] == x)
//			return i + 1000;
//	return -1;
//}
////�Ƿ��ڼ���st��
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
//		// �������в���ʽ
//		for (i = 1; i <= n; i++) {
//			// ��ȡ�󲿵�id
//			idl = char_id(p[i].left);
//			// �����Ҳ�
//			for (j = 0; p[i].right[j]; j++) {
//				// ��ȡ����ǰ�ַ���id
//				idr = char_id(p[i].right[j]);
//				// ������ս��
//				if (idr > 1000) {
//					// ������ս�����ڼ�����������󲿵�FIRST
//					if (!in(first[idl], p[i].right[j])) {
//						add(first[idl], p[i].right[j]);
//						inc = true;
//					}
//					// ���ս����FIRST���������
//					break;
//				}
//				// ����Ƿ��ս��
//				else { 
//					// �����÷��ս����first��
//					for (k = 1; k <= first[idr].n; k++) {
//						// ����÷��ս����first���ĸ�Ԫ�ز����󲿵�FIRST����
//						// �Ҹ�Ԫ�ز�����epsilon
//						if (!in(first[idl], first[idr].elm[k]) && first[idr].elm[k] != '~') {
//							// ����Ԫ�ؼ����󲿵�FIRST������
//							add(first[idl], first[idr].elm[k]);
//							inc = true;
//						}
//					}
//					// ����󲿵�FIRST����û��epsilon
//					if (!in(first[idl], '~'))
//						break;
//				}
//			}
//			// �������ʽ�Ҳ�Ϊepsilon
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
