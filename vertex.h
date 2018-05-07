#ifndef VERTEX_H
#define VERTEX_H

#include "arc.h"

class CVertex {

public:
	CVertex(int index, void * data) {
		m_data = data;
		m_arc.createArray(sizeof(CArc));
		m_bVisited = false;
		m_inDegree = 0;
		m_outDegree = 0;
		m_index = index;
		//m_course_no[0] = '\0';
		//m_term = 0;
	}

	void visit() {
		if (m_bVisited == true)
			return;
		printf("visited : id %d ", m_index);
		m_bVisited = true;
		for (int i = 0; i < m_arc.GetLength(); i++) {
			CArc* a = (CArc *)m_arc[i];
			CVertex *v = a->m_to;
			v->visit();
		}
	}

public:
	void * m_data;
	VectorArray m_arc;
	bool m_bVisited;
	int m_inDegree;
	int m_outDegree;
	int m_index;
	//char m_course_no[32];
	//int m_term;

};

#endif