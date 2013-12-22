/*
 * QuestionList.h
 *
 *  Created on: Dec 6, 2013
 *      Author: Mathieu
 */

#ifndef QUESTIONLIST_H_
#define QUESTIONLIST_H_

#include"Question.h"
#include "Path.h";
#include<list>
#include<stack>
#include<stdexcept>

class QuestionList {

public:
	QuestionList();
	virtual ~QuestionList();

	class Iterator {
	public:
		friend class QuestionList;
		Iterator();
		Iterator(const Iterator& qList);
		virtual ~Iterator();

		void operator=(const Iterator& it);
		Iterator operator++();
		Iterator levelForward();
		friend const Iterator operator+(Iterator out, const int offset);
		Iterator operator--();
		Iterator levelBackward();
		friend const Iterator operator-(Iterator out, const int offset);
		Question* operator*();
		bool operator!=(const Iterator& it) const;
		bool operator==(const Iterator& it) const;

		Path getPath() const;
	private:
		//Iterator* previous_;
		std::stack<std::list<Question*>::iterator> previous_;
		std::list<Question*>::iterator current_;
		int position_;
		Path path_;
	};

	void add(Question* q);
	void insert(Question* q, Iterator previous);
	void remove(Question* q);
	int size() const;

	Iterator begin();
	Iterator end();
	Iterator iteratorForPath(Path path);

protected:
	std::list<Question*> list_;

};

#endif /* QUESTIONLIST_H_ */
