#pragma once

typedef struct queue_node_s{
	void *item;
	queue_node_s *next;
	queue_node_s *prev;


} queue_node_t;

class CQueue {
public:
	//CQueue(int itemsize);
	void enqueue(void *);
	void * delqueue();
	bool queueEmpty();

private:
	queue_node_t *m_queue_front = NULL;
	queue_node_t *m_queue_rear = NULL;
};