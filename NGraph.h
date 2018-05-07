#ifndef NGRAPH_H
#define NGRAPH_H


#include "VectorArray.h"
#include "vertex.h"


class CGraph {

private:
	VectorArray m_vertices;
	VectorArray m_headvertices_id_table;

public:
	CGraph();
	void addVertex(CVertex * vertex);
	CVertex * getVertex(int index);
	int getVerticesNum();

public:
	CVertex* CreateVertex(int data);
	void CreateArc(CVertex *from, CVertex *to, int weight);
	void dfsTraverseRecursion();
	void dfsTraverseIteration();
	void bfsTraverseIteration();

	void insertCourse(char * course_no, char * prev_course, int course_credit);
	void sortingCourse(VectorArray*, int solution);
	int findLongestPath(CVertex *vex);
};

#endif