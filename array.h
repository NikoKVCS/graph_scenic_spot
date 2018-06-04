#pragma once

typedef struct array_node_s{
	void *item;
	array_node_s *next;
	array_node_s *prev;

} array_node_t;

class CArray {
public:
	void add(void *);
	void * get(int i);
	int length();

private:
	array_node_t *m_memblock = NULL;
	int m_length = 0;
	int m_size = 0;
};