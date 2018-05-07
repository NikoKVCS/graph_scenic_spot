#include "stdafx.h"
#include "NGraph.h"
#include "malloc.h"
#include "queue.h"
#include "string.h"
#include "stack.h"


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


void CGraph::dfsTraverseIteration() {


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

	if (vex->m_arc.GetLength() == 0)
		return 1;

	int deepest = 0;

	for (int i = 0; i < vex->m_arc.GetLength(); i++) {
		int depth = findLongestPath(((CArc *)vex->m_arc[i])->m_to);
		if (depth > deepest)
			deepest = depth;
	}
	return (1 + deepest);
}

