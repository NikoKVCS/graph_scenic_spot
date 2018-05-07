#ifndef ARC_H
#define ARC_H

class CVertex;

class CArc {

public:

	int m_weight;
	CVertex * m_to;

	CArc() {
		m_weight = -1;
		m_to = NULL;
		//m_from = NULL;
	}

	//CVertex *m_from;
};

#endif