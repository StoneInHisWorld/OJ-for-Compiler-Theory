#include "Follow_Graph.h"

Follow_Graph::Follow_Graph()
{
	// ��ʼ��ͷ���
	AdjList VL = (AdjList)malloc(sizeof(VNode));
	VL->arcList = NULL;
	VL->infor = NULL;
	VL->pos = 0;
	VL->type = T;
	VL->next = NULL;
	G.adjList = VL;	// �ڽӱ�
	G.rear = VL;
	G.vexNum = 0;
}

VNode *Follow_Graph::insertVertex(NodeType type, pair<TER, NTER> &infor)
{
	// �������Ѵ��ڣ���ֹͣ����
	VNode *ret = getVertex(type, infor);
	if (ret != NULL)
	{
		return ret;
	}
	// ����һ���½��
	VNode *newNode = new VNode;
	newNode->arcList = NULL;
	newNode->infor = new pair<TER, NTER>();
	newNode->infor = &infor;
	newNode->next = NULL;
	newNode->pos = this->G.rear->pos + 1;
	newNode->type = type;
	this->G.rear->next = newNode;
	this->G.rear = newNode;
	this->G.vexNum++;
	return newNode;
}

VNode *Follow_Graph::insertVertex(NodeType type, TER ter, NTER nter)
{
	// �������Ѵ��ڣ���ֹͣ����
	VNode *ret = getVertex(type, ter, nter);
	if (ret != NULL)
	{
		return ret;
	}
	// ͨ�����ɽ�����
	VNode *newNode = freeNode(type, ter, nter);
	newNode->pos = this->G.rear->pos + 1;
	this->G.rear->next = newNode;
	this->G.rear = newNode;
	this->G.vexNum++;
	return newNode;
}

AdjList Follow_Graph::getAdjList()
{
	return this->G.adjList;
}

 VNode *Follow_Graph::getVertex(NodeType type, pair<TER, NTER> infor)
{
	VNode *p = this->G.adjList->next;
	while (p != NULL)
	{
		if (p->type == type &&
			infor.first == p->infor->first && infor.second == p->infor->second)
		{
			break;
		}
		p = p->next;
	}
	return p;
}

 VNode * Follow_Graph::getVertex(NodeType type, TER ter, NTER nter)
 {
	 pair<TER, NTER> temp(ter, nter);
	 return getVertex(type, temp);
 }

void Follow_Graph::addArc(VNode *head, const VNode *tail)
{
	if (head == this->G.adjList || tail == this->G.adjList)
	{
		throw exception("����Ϊͷ������ӱ�");
	}
	if (head->pos == tail->pos)
	{
		throw exception("�������ͷβ��ͬ�ı�");
	}
	// ����Ƿ��м����������
	if (this->hasNeighbour(head, tail))
	{
		return;
	}
	ArcNode *newArc = new ArcNode;
	newArc->adjvex = tail->pos;
	newArc->next = NULL;
	ArcNode *iterator = head->arcList;
	if (iterator == NULL)
	{
		head->arcList = newArc;
		return;
	}
	// ������������β
	while (iterator->next != NULL)
	{
		iterator = iterator->next;
	}
	iterator->next = newArc;
}

bool Follow_Graph::hasNeighbour(VNode * head, const VNode * tail)
{
	ArcNode *p = head->arcList;
	while (p != NULL)
	{
		if (p->adjvex == tail->pos)
		{
			return true;
		}
		p = p->next;
	}
	return false;
}

VNode * Follow_Graph::getVertex(int pos)
{
	if (pos >= 0 && pos <= this->G.vexNum)
	{
		VNode *p = this->G.adjList;
		for (int iterator = 0; iterator < pos; iterator++)
		{
			p = p->next;
		}
		return p;
	}
	return nullptr;
}

bool Follow_Graph::isEmpty()
{
	// ����ڽӱ���ֻ��ͷ���
	if (this->G.adjList->next == NULL)
	{
		return true;
	}
	return false;
}

void Follow_Graph::printGraph()
{
	VNode *v = this->G.adjList->next;
	while (v != NULL)
	{
		cout << v->pos << '\t'<< Type_toString(v->type) << '\t'
			<< v->infor->first << v->infor->second;
		ArcNode *a = v->arcList;
		while (a != NULL)
		{
			cout << "->" << a->adjvex;
			a = a->next;
		}
		cout << endl;
		v = v->next;
	}
}

bool Follow_Graph::DFS(VNode * start, FOLLOW & follow)
{
	bool *visited = new bool[G.vexNum + 1];
	memset(visited, false, G.vexNum + 1);
	this->DFS(start, follow, visited);
	return true;
}

// visited�������飬��¼����Ƿ��ѷ��ʣ�����������
bool Follow_Graph::DFS(VNode * start, FOLLOW & follow, bool visited[])
{
	// �����ʼ�ڵ��������NULL
	if (start == NULL)
	{
		return false;
	}
	// ����ÿ����
	ArcNode *p = start->arcList;
	while (p != NULL)
	{
		VNode *v_toTraverse = this->getVertex(p->adjvex);
		// ������ʹ�������
		if (visited[p->adjvex])
		{
			p = p->next;
			continue;
		}
		this->visit(v_toTraverse, follow, visited);
		this->DFS(v_toTraverse, follow, visited);
		p = p->next;
	}
	return true;
}

bool Follow_Graph::visit(VNode * v_toTraverse, FOLLOW & follow, bool visited[])
{
	if (v_toTraverse == NULL)
	{
		return false;
	}
	visited[v_toTraverse->pos] = true;
	if (v_toTraverse->type == T)
	{
		TER ter = v_toTraverse->infor->first;
		if (ter == NULL)
		{
			throw exception("�ս�����洢���ս����");
		}
		follow.selfFOLLOW.insert(ter);
	}
	return true;
}

string Type_toString(NodeType type)
{
	switch (type)
	{
	case T: return "TER";
		break;
	case First: return "FIRST";
		break;
	case Follow: return "FOLLOW";
		break;
	default:
		break;
	}
	throw exception("����Ľ�����ͣ�");
}