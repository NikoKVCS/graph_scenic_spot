#include "stdafx.h"
#include "queue.h"
bool CQueue:: queueEmpty() {
	if (NULL == m_queue_front)
		return true;
	return false;
}

void CQueue::enqueue(void *p) {
	if (NULL == m_queue_rear) {
		queue_node_t *node = new queue_node_t;
		node->item = p;
		node->next = NULL;
		node->prev = NULL;

		m_queue_rear = node;
		m_queue_front = node;
	}
	else {

		queue_node_t *node = new queue_node_t;
		node->item = p;
		node->next = NULL;
		node->prev = m_queue_rear;

		m_queue_rear->next = node;
		m_queue_rear = node;
	}

}

void * CQueue:: delqueue() {
	if (m_queue_front == NULL) {
		return NULL;
	}
	queue_node_t *q = m_queue_front;
	void *data = q->item;

	m_queue_front = q->next;
	if (NULL != m_queue_front) {
		m_queue_front->prev = NULL;
	}
	else {
		m_queue_rear = NULL;
	}

	delete q;
	return data;
}