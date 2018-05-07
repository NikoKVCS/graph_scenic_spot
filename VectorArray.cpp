#include "stdafx.h"
#include <malloc.h>
#include <windows.h>
#include "VectorArray.h"

int VectorArray::AddItem(void * p) {
	if (m_length >= m_capacity) {
		m_items = realloc(m_items, m_capacity + ARRAYSIZE_DEFAULT);
		m_capacity += ARRAYSIZE_DEFAULT;
	}

	memcpy((char *)m_items + m_length * m_itemsize, p, m_itemsize);
	m_length++;
	return (m_length - 1);
}

void VectorArray::deleteItem(int index) {
	m_capacity++;
	memmove((char *)m_items + m_itemsize * index, (char *)m_items + m_itemsize * (index + 1), m_length - index -1);
	m_length--;
}


void * VectorArray::GetItem(int index) const {
	if (index >= m_length)
		return NULL;
	return ((char *)m_items) + index * m_itemsize;
}


int VectorArray::GetLength() {

	return m_length;
}

void VectorArray::createArray(int size) {
	m_itemsize = size;
	m_items = malloc(size * ARRAYSIZE_DEFAULT);
	m_capacity = ARRAYSIZE_DEFAULT;
}

VectorArray::VectorArray() {
	m_items = NULL;
	m_capacity = 0;
	m_length = 0;
	m_itemsize = 0 ;
}