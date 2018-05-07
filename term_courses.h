#pragma once

#include "VectorArray.h"

class CourseGroup {
private:
	int m_term = 0;
	VectorArray m_courses;
	int m_credit_limit;
	int m_credit;
	int m_course_ave = 0;
	int course_num = 0;

public:
	CourseGroup(int term, int credit_amout, int ave) {
		m_courses.createArray(sizeof(char) * 32);
		m_term = term;
		m_credit_limit = credit_amout;
		m_course_ave = ave;
	}

	bool isFull() {
		if (m_credit >= m_credit_limit)
			return true;
		return false;
	}

	bool isBalance() {
		if (course_num >= m_course_ave)
			return true;
		return false;
	}
	
	void addCourse(char * course_no, int credit) {
		m_courses.AddItem(course_no);
		m_credit += credit;
		course_num++;
	}

	void printCourse() {
		printf("µÚ %d Ñ§ÆÚ\n", m_term);
		for (int i = 0; i < m_courses.GetLength(); i++)
			printf("%s\n", (char *)m_courses[i]);
	}

	int getTerm() {
		return m_term;
	}


};