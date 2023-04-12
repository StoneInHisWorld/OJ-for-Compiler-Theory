#include "Follow_Graph.h"

Follow_Graph::Follow_Graph()
{
	// 初始化头结点
	AdjList VL = (AdjList)malloc(sizeof(VNode));
	VL->arcList = NULL;
	VL->infor = NULL;
	VL->pos = 0;
	VL->type = T;
	VL->next = NULL;
	G.adjList = VL;	// 邻接表
	G.rear = VL;
	G.vexNum = 0;
}

VNode *Follow_Graph::insertVertex(NodeType type, pair<TER, NTER> &infor)
{
	// 如果结点已存在，则停止插入
	VNode *ret = getVertex(type, infor);
	if (ret != NULL)
	{
		return ret;
	}
	// 建立一个新结点
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
	// 如果结点已存在，则停止插入
	VNode *ret = getVertex(type, ter, nter);
	if (ret != NULL)
	{
		return ret;
	}
	// 通过自由结点插入
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
		throw exception("不能为头结点增加边");
	}
	if (head->pos == tail->pos)
	{
		throw exception("不能添加头尾相同的边");
	}
	// 检查是否有加入过这条边
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
	// 遍历到弧链表尾
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
	// 如果邻接表里只有头结点
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

// visited访问数组，记录结点是否已访问，访问则跳过
bool Follow_Graph::DFS(VNode * start, FOLLOW & follow, bool visited[])
{
	// 如果开始节点输入的是NULL
	if (start == NULL)
	{
		return false;
	}
	// 遍历每条弧
	ArcNode *p = start->arcList;
	while (p != NULL)
	{
		VNode *v_toTraverse = this->getVertex(p->adjvex);
		// 如果访问过则跳过
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
			throw exception("终结符结点存储非终结符！");
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
	throw exception("错误的结点类型！");
}