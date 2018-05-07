#ifndef VECTORARRAY_H
#define VECTORARRAY_H

class VectorArray {

private:
	const int ARRAYSIZE_DEFAULT = 50;
	void * m_items;
	int m_capacity;
	int m_length;
	int m_itemsize;

public:
	VectorArray();
	int AddItem(void *);
	void * GetItem(int index) const;
	int GetLength();
	void createArray(int size);
	void deleteItem(int index);
	

public:
	inline void * operator[](int index) const { return GetItem(index); }
	
};

#endif