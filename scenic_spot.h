#pragma once

class CScenicSpot {

public:
	int m_id;
	char m_name[64];

	CScenicSpot(int _id, char *_name) {
		m_id = _id;
		sprintf(m_name, "%s", _name);
	}
};