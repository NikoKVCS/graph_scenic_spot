#include "stdafx.h"
#include "string.h"
#include "NGraph.h"
#include "scenic_spot.h"
#include <stdlib.h>

VectorArray vex_vector;

int file_readVex(CGraph *graph) {

	FILE *f = fopen("vex.txt", "rt");

	if (f == NULL)
		throw "no file loaded !";

	int vex_num = 0;
	vex_vector.createArray(sizeof(CVertex *));

	char szBuff[64];
	while (!feof(f)) {
		fgets(szBuff, sizeof(szBuff) - 1, f);

		if (!szBuff[0] || szBuff[0] == '\n')
			continue;
		if (szBuff[strlen(szBuff) - 1] == '\n')
			szBuff[strlen(szBuff) - 1] = '\0';

		char *p = strtok(szBuff, " ");
		char data[2][32];
		int i = 0;
		while (p != NULL)
		{
			if (i >= 2)
				break;

			sprintf(data[i], p);
			i++;
			p = strtok(NULL, " ");
		}

		CScenicSpot *spot = new CScenicSpot(atoi(data[0]), data[1]);
		CVertex * vex =  graph->CreateVertex(spot);
		vex_vector.AddItem(&vex);

		printf("编号:%d   名称:%s\n", spot->m_id, spot->m_name);
		
		vex_num++;
	}

	printf("一共有景点:%d\n", vex_num);

	return vex_num;
}

void file_readEdge(CGraph *graph) {

	FILE *f = fopen("edge.txt", "rt");

	if (f == NULL)
		throw "no file loaded !";

	char szBuff[64];

	while (!feof(f)) {
		fgets(szBuff, sizeof(szBuff) - 1, f);

		if (!szBuff[0] || szBuff[0] == '\n')
			continue;
		if (szBuff[strlen(szBuff) - 1] == '\n')
			szBuff[strlen(szBuff) - 1] = '\0';

		char *p = strtok(szBuff, " ");
		char data[3][32];
		int i = 0;
		while (p != NULL)
		{

			sprintf(data[i], p);
			i++;
			p = strtok(NULL, " ");
		}

		int index1 = atoi(data[0]);
		int index2 = atoi(data[1]);
		int index3 = atoi(data[2]);

		CVertex * vex1 = *((CVertex **)vex_vector[index1]);
		CVertex * vex2 = *((CVertex **)vex_vector[index2]);
	
		printf("左端点:%d 名称:%s   右端点:%d 名称:%s   距离:%d\n", 
			((CScenicSpot*)vex1->m_data)->m_id, ((CScenicSpot*)vex1->m_data)->m_name,
			((CScenicSpot*)vex2->m_data)->m_id, ((CScenicSpot*)vex2->m_data)->m_name,
			index3);

		graph->CreateArc(vex1, vex2, index3);
	}

	return;
}
