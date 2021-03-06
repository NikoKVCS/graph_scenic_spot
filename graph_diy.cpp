// graph_diy.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "NGraph.h"
#include "string.h"
#include "scenic_spot.h"
#include "stdlib.h"
#include "file_operator.h"
#include "VectorArray.h"
#include "vertex.h"
#include "NGraph.h"
#include "malloc.h"
#include "queue.h"
#include "string.h"
#include "stack.h"
#include "scenic_spot.h"


CGraph graph;


int main()
{
	graph.setHasDirection(false); // 设置为无向图

	try {
		file_readVex(&graph);
		file_readEdge(&graph);

		printf("\n");
		printf("想要查询的景点的编号\n");

		int a = 0;
		scanf("%d", &a);

		CVertex *vex = graph.getVertex(a);
		printf("\n名称为:%s", ((CScenicSpot *)vex->m_data)->m_name);

		for (int i = 0; i < vex->m_arc.GetLength(); i++) {
			CVertex *temp = ((CArc *)vex->m_arc[i])->m_to;
			CScenicSpot *scene = (CScenicSpot *)temp->m_data;
			printf("\n相邻节点名称为:%s", ((CScenicSpot *)temp->m_data)->m_name);
		}

		printf("\n深度为%d", graph.findLongestPath(vex));

		printf("\n");
		printf("\n");
		printf("想要查询景区无回路访问的方式的景点编号\n");

		scanf("%d", &a);

		vex = graph.getVertex(a);
		char sz[128];
		sz[0] = 0;
		vex->visit(0, graph.findLongestPath(vex), sz);

		printf("\n");
		printf("\n");
		printf("想要查询路径的两个景点编号\n");

		int b;
		scanf("%d %d", &a, &b);
		vex = graph.getVertex(a);
		CVertex* vex2 = graph.getVertex(b);
		graph.dijkstra(vex, vex2);
	}
	catch (char * errorInfo) {
		printf("\n%s", errorInfo);
		return 0;
	}

    return 0;
}
