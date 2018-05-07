// graph_diy.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "NGraph.h"
#include "string.h"
//#include "vertex.h"
#include "stdlib.h"
#include "term_courses.h"

CGraph graph;


int readfile() {
	FILE *f = fopen("course.txt", "rt");

	if (f == NULL)
		throw "no file loaded !";

	int course_num = 0;

	char szBuff[64];
	while (!feof(f)) {
		fgets(szBuff, sizeof(szBuff) - 1, f);

		if (!szBuff[0] || szBuff[0] == '\n')
			continue;
		if (szBuff[strlen(szBuff) - 1] == '\n')
			szBuff[strlen(szBuff) - 1] = '\0';

		char course_no[32];
		char course_credit[32];
		char course_pre[32];



		char* token = strtok(szBuff, "/");
		sprintf(course_no, "%s", token);
		int i = 1;
		while (token != NULL)
		{
			token = strtok(NULL, "/");
			if (1 == i) {
				i++;
				sprintf(course_credit, "%s", token);
			}
			else {
				if (token == NULL)
				{
					course_pre[0] = '\0';
					break;
				}
				else {
					sprintf(course_pre, "%s", token);
					break;
				}
			}
		}
		graph.insertCourse(course_no, course_pre, atoi(course_credit) );
		course_num++;
	}

	return course_num;
}

void printCourse(VectorArray * group) {
	CourseGroup * term = NULL;

	for (int i = 0; i < group->GetLength(); i++) {
		term = (CourseGroup *)group->GetItem(i);
		term->printCourse();
	}
}

int main()
{

	int terms_total;
	int credits_per_term;
	int solution;

	printf("请输入学期数量:");
	scanf("%d", &terms_total);
	printf("\n请输入每学期的学分上限:");
	scanf("%d", &credits_per_term);
	printf("\n请选择方案1(均匀排序)或者方案2(紧凑排序)进行编排");
	scanf("%d", &solution);
	printf("\n正在排序");


	try {
		int course_num = readfile();
		int course_per_term = course_num / terms_total + 1;


		VectorArray group;
		group.createArray(sizeof(CourseGroup));
		for (int i = 1; i <= terms_total; i++) {
			CourseGroup *course_group = new  CourseGroup(i, credits_per_term, course_per_term);
			group.AddItem(course_group);
		}


		graph.sortingCourse(&group, solution);
		printCourse(&group);
	}
	catch (char * errorInfo) {
		printf("\n%s", errorInfo);
		return 0;
	}


    return 0;
}
