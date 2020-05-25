#ifndef LOCKFREE_H__
#define LOCKFREE_H__
#include <iostream>
using namespace std;
#include "node.h"
#include <mutex> // std::mutex, std::lock_guard
#include <omp.h>
#include <stdint.h>

#include "setlist.h"
#include "Window.h"

#include <memory>

#define FLAG_POS 63
#define MASK 1ULL<<FLAG_POS




template <typename T> class LockFree : public SetList<T> { // The class
  private:
	T item;
	std::mutex mtx;
	Window_at_t<nodeAtom<T>> find(T item);
	nodeAtom<T>* getPointer(nodeAtom<T>* pointer);
	void setFlag(nodeAtom<T>** pointer);
	void resetFlag(nodeAtom<T>** pointer);
	void resetFlag(atomic<nodeAtom<T>*>* pointer);
	bool getFlag(nodeAtom<T>* pointer);

  public:
	nodeAtom<T>* head;
	LockFree();
	~LockFree();
	bool add(T item);
	bool remove(T item);
	bool contains(T item);
};
#endif