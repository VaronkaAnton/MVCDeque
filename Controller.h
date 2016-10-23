/*Controller for MVC pattern
Accepts input and converts it to commands for the model and view*/

#pragma once
#include "Model.h"
#include "View.h"

template <class T>
class Controller {
	KDeque<T> *deque;
	View<T> *view;
public:
	Controller(KDeque<T> *deque, View<T> *v) : deque(deque), view(v) {}

	void push_front() {
		int i = view->toPush();
		deque->push_front(i);
		view->update();
	}
	void push_back() {
		int i = view->toPush();
		deque->push_back(i);
		view->update();
	}

	void pop_front() {
		deque->pop_front();
		view->update();
	}
	void pop_back() {
		deque->pop_back();
		view->update();
	}
	void showSize() {
		int size = deque->size();
		view->showSize(size);
	}
	void showIfEmpty() {
		bool isEmpty = deque->isEmpty();
		view->showIfEmpty(isEmpty);
	}
	void clear() {
		deque->clear();
		view->update();
	}
	~Controller() {
		delete deque;
		delete view;
	}
};