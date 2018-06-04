#include "stdafx.h"
#include "NGraph.h"
#include "malloc.h"
#include "queue.h"
#include "string.h"
#include "stack.h"
#include "scenic_spot.h"
#include "VectorArray.h"
#include "vertex.h"

CGraph::CGraph() {
	m_vertices.createArray(sizeof(CVertex));
	m_headvertices_id_table.createArray(sizeof(int));
	m_bDirection = false;
}


void CGraph::setHasDirection(bool _hasDirection) {
	m_bDirection = _hasDirection;
}


CVertex * CGraph::getVertex(int index) {
	if (index >= m_vertices.GetLength())
		return NULL;
	else
		return (CVertex *)m_vertices[index];
}

int CGraph::getVerticesNum() {
	return m_vertices.GetLength();
}

void CGraph::addVertex(CVertex * vertex) {
	m_vertices.AddItem(vertex);
}

// 建立顶点. 加入头顶点中
CVertex* CGraph::CreateVertex(void * data) {
	int index = m_vertices.GetLength();
	CVertex *v = new CVertex(index, data);
	m_vertices.AddItem(v);
	m_headvertices_id_table.AddItem(&index);
	delete v;
	return (CVertex*)m_vertices[index];
}

// 建立弧 from出度+1  to入度+1  从 m_headvertices_id_table 中删除from
void CGraph::CreateArc(CVertex *from, CVertex *to,  int weight) {

	if (m_bDirection) {

		CArc *arc = new CArc;
		arc->m_weight = weight;
		arc->m_to = to;

		to->m_inDegree++;
		from->m_arc.AddItem(arc);
		from->m_outDegree++;

		delete arc;
	}
	else {
		CArc *arc = new CArc;
		arc->m_weight = weight;
		arc->m_to = to;

		from->m_inDegree++;
		from->m_outDegree++;
		from->m_arc.AddItem(arc);


		delete arc;

		arc = new CArc;
		arc->m_weight = weight;
		arc->m_to = from;

		to->m_inDegree++;
		to->m_outDegree++;
		to->m_arc.AddItem(arc);

		delete arc;
	}

	for (int i = 0; i < m_headvertices_id_table.GetLength(); i++) {
		int* id =(int *) m_headvertices_id_table[i];
		if (*id == to->m_index) {
			m_headvertices_id_table.deleteItem(i);
			break;
		}
	}

}

void CGraph::dfsTraverseRecursion() {
	for (int i = 0; i < m_headvertices_id_table.GetLength(); i++) {
		int * id =(int *) m_headvertices_id_table[i];
		CVertex* v = (CVertex *)m_vertices.GetItem(*id);
		v->visit();
	}

	for (int i = 0; i < m_vertices.GetLength(); i++) {
		CVertex* v = (CVertex *)m_vertices[i];
		v->m_bVisited = false;
	}

}

void CGraph::clearVisitedFlag() {
	for (int i = 0; i < m_vertices.GetLength(); i++)
	{
		((CVertex*)m_vertices[i])->m_bVisited = false;
	}

}

void CGraph::dfsTraverseIteration(CVertex *begin) {

	CStack stack;
	stack.push(begin);

	int deepest = findLongestPath(begin);
	int depth = 0;
	clearVisitedFlag();

	CQueue *path = new CQueue;

	while (!stack.stackEmpty()) 
	{
		CVertex *node = (CVertex*)stack.pop();
		if (node->m_bVisited)
			continue;

		node->m_bVisited = true;
		depth++;
		path->enqueue(node);

		bool bEnd = true;
		for (int i = 0; i < node->m_arc.GetLength(); i++)
		{
			CVertex* neighborhood = ((CArc*)node->m_arc[i])->m_to;
			if (NULL != neighborhood && false == neighborhood->m_bVisited)
			{
				bEnd = false;
				stack.push(neighborhood);
			}
		}
		if (bEnd)
		{
			if (depth == deepest)
			{
				printf("\n");
				while(!path->queueEmpty())
				{
					CVertex* v = (CVertex*)path->delqueue();
					printf("%s-", ((CScenicSpot*)v->m_data)->m_name);
				}
			}
			depth = 1;
			free(path);
			path = new CQueue;
			path->enqueue(begin);
			clearVisitedFlag();
			begin->m_bVisited = true;
		}
		
	}

}

void CGraph::bfsTraverseIteration() {
	for (int i = 0; i < m_headvertices_id_table.GetLength(); i++) {
		int * id = (int *)m_headvertices_id_table[i];
		CVertex* v = (CVertex *)m_vertices.GetItem(*id);
		//v->visit();

		CQueue queue;
		queue.enqueue(v);

		while (!queue.queueEmpty()) {
			CVertex* vex = (CVertex*)queue.delqueue();
			if (vex->m_bVisited)
				continue;
			vex->m_bVisited = true;
			printf("breadth first traverse id : %d", vex->m_index);
			for (int i = 0; i < vex->m_arc.GetLength(); i++) {
				CArc* a = (CArc *)vex->m_arc[i];
				CVertex *to = a->m_to;
				queue.enqueue(to);
			}
		}
	}


	for (int i = 0; i < m_vertices.GetLength(); i++) {
		CVertex* v = (CVertex *)m_vertices[i];
		v->m_bVisited = false;
	}

}

// 建立节点CreateVertex(). 如果有先修课程 建立弧CreateArc()
void CGraph::insertCourse(char * course_no, char * prev_course, int course_credit) {
#ifdef insertCourse
	CVertex *vex = CreateVertex(course_credit);
	sprintf(vex->m_course_no, "%s", course_no);

	if (prev_course[0] != '\0') {
		CVertex * prev_vex = NULL;

		for (int i = 0; i < m_vertices.GetLength(); i++) {
			prev_vex = (CVertex *)m_vertices[i];
			if (!strcmp(prev_vex->m_course_no, prev_course))
				break;
		}
		if (prev_vex == NULL)
			throw "出错, 找不到课程指定的先修课程.";

		CreateArc(prev_vex, vex, 1);
	}
#endif
}


void CGraph::dijkstra(CVertex *vex1, CVertex *vex2)
{
	typedef struct dis_s {
		CStack paths;
		int depth;
	}dis_t;

	int size = m_vertices.GetLength();

	dis_t *dis = new dis_t[size];

	int src;
	int dst;

	int*T = new int[size];
	for (int i = 0; i < size; i++)
	{
		T[i] = -1;
		dis[i].depth = 2000000000; // 模拟正无穷

		if (m_vertices[i] == vex1)
			src = i;
		 if (m_vertices[i] == vex2)
			dst = i;
	}

	
	dis[src].depth = 0;
	int top = 0;
	T[top] = src;
	top++;

	//初始化dis数组
	for (int j = 0; j < vex1->m_arc.GetLength(); j++)
	{
		CArc * arc = (CArc*)vex1->m_arc[j];
		int index = 0;
		for (int i = 0; i < size; i++)
		{
			if (m_vertices[i] == arc->m_to)
			{
				index = i;
				break;
			}
		}

		dis[index].depth = arc->m_weight;
		CQueue * q = new CQueue;
		q->enqueue(arc->m_to);
		dis[index].paths.push(q);
	}



	while (T[size - 1] == -1)
	{
		int min = 2147483647;
		int min_pos = src;
	
		for (int i = 0; i < size; i++)
		{
			bool skip = false;
			for (int j = 0; j <= top; j++)
			{
				if (T[j] == i)
					skip = true;
			}

			if (true == skip)
				continue;

			if (dis[i].depth < min)
			{
				min_pos = i;
				min = dis[i].depth;
			}
		}

		T[top] = min_pos;
		top++;

		CVertex* v = (CVertex*)m_vertices[min_pos];
		CQueue * q = (CQueue *)dis[min_pos].paths.pop();

		for (int i = 0; i < v->m_arc.GetLength(); i++) {
			CArc * arc = (CArc*)v->m_arc[i];
			if (arc->m_to == vex1)
				continue;
		}

	}

}

int CGraph::getVertexIndex(CVertex *vex)
{
	int size = m_vertices.GetLength();

	for (int i = 0; i < size; i++)
	{
		if (m_vertices[i] == vex)
			return i;
	}
	return -1;
}

void CGraph::findPathOut(CVertex *vex) {
	CStack stack;
	CVertex* node= vex;

	int deepest = 0;
	CVertex * deepest_node = node;

	while (true) {

		if (node->m_bVisited)
			break;
	
		printf("%d_%s ", ((CScenicSpot*)node->m_data)->m_id, ((CScenicSpot*)node->m_data)->m_name);
		node->m_bVisited = true;

		for (int i = 0; i < node->m_arc.GetLength(); i++) {
			int depth = findLongestPath(((CArc *)node->m_arc[i])->m_to);
			if (depth >= deepest) {
				deepest = depth;
				deepest_node = ((CArc *)node->m_arc[i])->m_to;
			}
		}

		if (deepest == 0)
			break;

		node = deepest_node;
		deepest = 0;
		
	}

}

void CGraph::sortingCourse(VectorArray* course_group, int solution) {
//#define sortingCourse
#ifdef sortingCourse
	CStack stack;


	for (int i = m_headvertices_id_table.GetLength()-1; i >= 0; i--)
	{
		int* id = (int *)m_headvertices_id_table[i];
		stack.push(m_vertices[*id]);
	}

	while (!stack.stackEmpty()) {

		CVertex * node = (CVertex *)stack.pop();

		if (solution == 1) {
			if (findLongestPath(node) > course_group->GetLength())
				throw "学期数太少以至于无法安排课程";

			CourseGroup* group = NULL;
			for (int i = 0; i < course_group->GetLength(); i++) {
				group = (CourseGroup *)course_group->GetItem(i);
				if (group->isFull() || group->isBalance() || group->getTerm() < node->m_term)
					continue;
				else
					break;
			}
			if (group == NULL)
				throw "可分配的学期数量不足";
			else
				group->addCourse(node->m_course_no, node->m_data);

			CVertex *ve = node;
			for (int i = 0; i < ve->m_arc.GetLength(); i++) {
				CArc *a = (CArc *)ve->m_arc.GetItem(i);
				a->m_to->m_inDegree--;
				if(a->m_to->m_term <= group->getTerm())
					a->m_to->m_term = group->getTerm() + 1;
				if (a->m_to->m_inDegree == 0) {
					stack.push(a->m_to);
				}
			}

		}
		else {
			if (findLongestPath(node) > course_group->GetLength())
				throw "学期数太少以至于无法安排课程";

			CourseGroup* group = NULL;
			for (int i = 0; i < course_group->GetLength(); i++) {
				group = (CourseGroup *)course_group->GetItem(i);
				if (group->isFull() || group->getTerm() < node->m_term)
					continue;
				else
					break;
			}
			if (group == NULL)
				throw "可分配的学期数量不足";
			else
				group->addCourse(node->m_course_no, node->m_data);

			CVertex *ve = node;
			for (int i = 0; i < ve->m_arc.GetLength(); i++) {
				CArc *a = (CArc *)ve->m_arc.GetItem(i);
				a->m_to->m_inDegree--;
				if (a->m_to->m_term <= group->getTerm())
					a->m_to->m_term = group->getTerm() + 1;
				if (a->m_to->m_inDegree == 0) {
					stack.push(a->m_to);
				}
			}
		}
	}
#endif
} 

int CGraph::findLongestPath(CVertex *vex) {

	if (vex->m_bVisited)
		return 0;

	if (vex->m_arc.GetLength() == 0)
		return 1;

	bool visited = vex->m_bVisited;
	vex->m_bVisited = true;
	int deepest = 0;

	bool *a = new bool[vex->m_arc.GetLength()] ;

	for (int i = 0; i < vex->m_arc.GetLength(); i++) {
		a[i] = ((CArc *)vex->m_arc[i])->m_to->m_bVisited;
		int depth = findLongestPath(((CArc *)vex->m_arc[i])->m_to);
		if (depth > deepest)
			deepest = depth;
	}

	//for (int i = 0; i < vex->m_arc.GetLength(); i++) {
		//((CArc *)vex->m_arc[i])->m_to->m_bVisited = a[i];
	//}

	vex->m_bVisited = visited;

	return (1 + deepest);
}

