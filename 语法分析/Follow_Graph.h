#pragma once
#include "declaration.h"

enum NodeType
{
	T, First, Follow
};

typedef struct FOLLOW {
	NTER nter;
	set<TER> selfFOLLOW;
}FOLLOW;

typedef struct ArcNode {
	int adjvex;	// ��������ָ��Ķ���λ��
	struct ArcNode *next;	// ָ����һ������ָ��
}ArcNode, *ArcList;

typedef struct VNode {
	int pos;		// �洢�Լ����ڽӱ��е�λ��
	NodeType type;
	// TERΪNULL��洢����NTER
	pair<TER, NTER> *infor;
	ArcList arcList;		//ָ���һ�������ö���Ļ� 
	VNode *next;
}VNode, *AdjList;

// ʹ�ô�ͷ��������洢�ڽӱ�
typedef struct Graph{
	AdjList adjList; // �ڽӱ�
	VNode *rear; // βָ�����ڷ����������
	int vexNum;
}G_FOLLOW;

class Follow_Graph
{
public:
	Follow_Graph();
	VNode *insertVertex(NodeType type, pair<TER, NTER> &infor);
	VNode *insertVertex(NodeType type, TER ter, NTER nter);
	AdjList getAdjList();
	void deleteVertex(NodeType type, pair<TER, NTER> infor);
	void addArc(VNode *head, const VNode *tail);
	bool hasNeighbour(VNode *head, const VNode *tail);
	void deleteArc(VNode &head, VNode &tail);
	VNode *getVertex(int pos);
	VNode *getVertex(NodeType type, pair<TER, NTER> infor);
	VNode *getVertex(NodeType type, TER ter, NTER nter);
	bool isEmpty();
	void printGraph();
	bool DFS(VNode *start, FOLLOW &follow);
	bool visit(VNode *v_toTraverse, FOLLOW &follow, bool visited[]);

private:
	G_FOLLOW G;
	bool DFS(VNode *start, FOLLOW &follow, bool visited[]);
};

// ����һ����δ�����ڽӱ�Ľ��
inline VNode *freeNode(NodeType type, TER ter, NTER nter)
{
	VNode *freeNode = (VNode *)malloc(sizeof(VNode));
	freeNode->arcList = NULL;
	freeNode->infor = new pair<TER, NTER>();
	freeNode->infor->first = ter;
	freeNode->infor->second = nter;
	freeNode->next = NULL;
	freeNode->pos = NULL;
	freeNode->type = type;
	return freeNode;
}

string Type_toString(NodeType type);