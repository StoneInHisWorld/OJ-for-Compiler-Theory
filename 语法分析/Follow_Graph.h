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
	int adjvex;	// 该条弧所指向的顶点位置
	struct ArcNode *next;	// 指向下一条弧的指针
}ArcNode, *ArcList;

typedef struct VNode {
	int pos;		// 存储自己在邻接表中的位置
	NodeType type;
	// TER为NULL则存储的是NTER
	pair<TER, NTER> *infor;
	ArcList arcList;		//指向第一条依附该顶点的弧 
	VNode *next;
}VNode, *AdjList;

// 使用带头结点的链表存储邻接表
typedef struct Graph{
	AdjList adjList; // 邻接表
	VNode *rear; // 尾指针用于方便插入链表
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

// 生成一个尚未插入邻接表的结点
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