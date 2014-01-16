/*
 * QuestionList.cpp
 *
 *  Created on: Dec 6, 2013
 *      Author: Mathieu
 */

#include "QuestionList.h"
#include "OpenQuestion.h"
#include <iostream>

/**
 * Makes a new, empty QuestionList
 */
QuestionList::QuestionList() {

}

QuestionList::~QuestionList() {
	for(Iterator it = end().levelBackward(); it != end(); it.levelBackward()) {
		delete (*it);
	}
	list_.clear();
}
/**
 * Makes a new Iterator
 */
QuestionList::Iterator::Iterator() : position_(1) {

}

QuestionList::Iterator::~Iterator() {

}
/**
 * Makes a new Iterator based on qList
 * @param qList the Iterator that will be copied
 */
QuestionList::Iterator::Iterator(const Iterator& qList) {
	previous_ = qList.previous_;
	current_ = qList.current_;
	position_ = qList.position_;
	path_ = qList.path_;
}
/**
 * Assignment operator
 * @param it the Iterator that will be copied
 */
void QuestionList::Iterator::operator=(const Iterator& it) {
	previous_ = it.previous_;
	current_ = it.current_;
	position_ = it.position_;
	path_ = it.path_;
}
/**
 * Iterates one step forward, if it reaches a QuestionList, first iterates through it
 * @return the next iterator
 */
QuestionList::Iterator QuestionList::Iterator::operator++() {
	Question* q = (*current_);
	if(dynamic_cast<QuestionList *> (q) != NULL) {
		QuestionList* ql = dynamic_cast<QuestionList *>(*current_);
		//previous to go back once iterated through q
		previous_.push(current_);
		previous_.push(ql->end().current_); //push list.end()
		//one level deeper
		path_.push_back(position_);
		current_ = ql->list_.begin();
		position_ = 1;
	}
	else {
		position_++;
		++current_;
	}
	//reached end of QuestionList, travel back (a level up)
	while(current_ == previous_.top()) {
		//*this = *previous_;
		previous_.pop(); //pop list.end()
		if(!previous_.empty()) { //if in sublist
			current_ = previous_.top();
			previous_.pop();
			position_ = path_.back();
			path_.pop_back();
			position_++;
			++current_;
		}
		else break;
	}
	return *this;
}
/**
 * Iterates one step forward, but if goes one level deeper, iterates through until it
 * reaches the same level again
 * @return the updated iterator
 */
QuestionList::Iterator QuestionList::Iterator::levelForward() {
	int depth(getPath().size());
	int nextDepth(depth);
	do {
		operator ++();
		nextDepth = getPath().size();
	} while (depth != nextDepth && nextDepth != 0);

	return *this;
}
/**
 * Iterates offset steps forward
 * out + offset
 * @param out the iterator that will be operated on
 * @param offset the number of steps forward
 * @return the updated Iterator
 */
const QuestionList::Iterator operator+(QuestionList::Iterator out, const int offset) {
	for(int i = 0; i < offset; ++i) {
		if(out.previous_.size() > 0) ++out; //in bounds of list ?
	}
	return out;
}
/**
 * Iterates one step backward
 * @return the updated Iterator
 */
QuestionList::Iterator QuestionList::Iterator::operator--() {
	//if  was at end of questionlist,
	//then operator-- sets iterator at last element,
	//need to push list.end() in previous
	if(previous_.empty()) {
		previous_.push(current_);
	}
	--current_;
	--position_;
	//before begin of list
	if(position_ == 0) {
		previous_.pop(); //pop list.end()
		if(!previous_.empty()) {
			current_ = previous_.top(); //group
			previous_.pop();
			position_ = path_.back();
			path_.pop_back();
		}
	}
	else {
		while(dynamic_cast<QuestionList*>(*current_) != NULL) {
			QuestionList* ql = dynamic_cast<QuestionList*>(*current_);
			previous_.push(current_);
			previous_.push(ql->end().current_); //list.end()
			path_.push_back(position_);
			position_ = ql->list_.size(); //last of list

			current_ = ql->end().current_;
			--current_; //points to last of list
		}
	}
	return *this;
}
/**
 * Iterates a step backwards. If it goes a level deeper,
 * iterates back until on same level again
 * @return the udpated Iterator
 */
QuestionList::Iterator QuestionList::Iterator::levelBackward() {
	int depth(getPath().size());
	int nextDepth(depth);
	do {
		operator --();
		nextDepth = getPath().size();
	} while (depth != nextDepth && nextDepth != 0);

	return *this;
}
/**
 * Iterates offset steps backward
 * out - offset
 * @param out the Iterator that will be operated on
 * @param offset the number of steps
 * @return the updated iterator
 */
const QuestionList::Iterator operator-(QuestionList::Iterator out, const int offset) {
	for(int i = 0; i < offset; ++i) {
		if(out.position_ > 0) --out; //in bounds of list ?
	}
	return out;
}

Question* QuestionList::Iterator::operator*() {
	return (*current_);
}
/**
 * Checks if this Iterator points to the same Question as it
 * @param it the iterator to compare
 * @return true if they point to the same, false otherwise
 */
bool QuestionList::Iterator::operator==(const Iterator& it) const {
	return current_ == it.current_;
}
/**
 * Checks if this Iterator points to another Question as it
 * @param it the iterator to compare
 * @return true if they point to different Questions, false otherwise
 */
bool QuestionList::Iterator::operator!=(const Iterator& it) const {
	return !(*this == it);
}
/**
 * Get this Iterator's Path
 * @return the Path
 */
Path QuestionList::Iterator::getPath() const {
	Path p = path_;
	p.push_back(position_);
	return p;
}
/**
 * Add a Question at the end of this QuestionList
 */
void QuestionList::add(Question* q) {
	list_.push_back(q);
}
/**
 * Insert a Question before the Question previous points to
 * @throw std::invalid_argument previous is not valid
 * @param q the Question to insert
 * @param previous q will be inserted before previous
 */
void QuestionList::insert(Question* q, QuestionList::Iterator previous) {
	Path p = previous.getPath();
	int size = p.size();
	//insert in a sublist
	if(size > 1) {
		Iterator qLIterator = begin();
		//travel deeper to find group that will contain q
		for(int i = 0; i < size -1; ++i) {
			int shift(p.front());
			p.pop_front();
			qLIterator = qLIterator + shift; //first of sublist
		}
		--qLIterator;//sublist
		if(dynamic_cast<QuestionList*>(*qLIterator) != NULL) {
			QuestionList* ql = dynamic_cast<QuestionList*>(*qLIterator);
			ql->list_.insert(previous.current_, q);
		}
		else throw(std::invalid_argument("Not a valid iterator"));
	}
	//insert in this list
	else {
		list_.insert(previous.current_, q);
	}
}

/**
 * remove a Question
 * @param q the Question to remove
 */
void QuestionList::remove(Question* q) {
	list_.remove(q);
	for(Iterator it = begin(); it != end(); ++it) {
		//found the question
		if((*it) == q) {
			//get questionlist this questions belongs to
			it.previous_.pop();
			QuestionList* ql = dynamic_cast<QuestionList*>(*(it.previous_.top()));
			ql->list_.remove(*it);
			delete (*it);
			break;
		}
	}
}
/**
 * get this QuestionList's size
 * @return the size
 */
int QuestionList::size() const {
	return list_.size();
}
/**
 * Get an Iterator pointing to the first Question of this QuestionList
 * @return the iterator
 */
QuestionList::Iterator QuestionList::begin() {
	QuestionList::Iterator it;
	it.current_ = list_.begin();
	//it.previous_ = new Iterator(end());
	it.previous_.push(list_.end());
	return it;
}
/**
 * Get an Iterator pointing to the position after the last Question of this QuestionList
 * @reurn the Iterator
 */
QuestionList::Iterator QuestionList::end() {
	QuestionList::Iterator it;
	it.current_ = list_.end();
	it.position_ = list_.size() + 1;
	return it;
}
/**
 * Get an Iterator pointing to the Question at the Path path
 * @throw std::out_of_range path doesn't point to a Question in this QuestionList
 * @param path the Path of the Question
 * @return the Iterator
 */
QuestionList::Iterator QuestionList::iteratorForPath(Path path) {
	for(Iterator it = begin(); it != end(); ++it) {
		if(path == it.getPath()) return it;
	}
	throw std::out_of_range ("No iterator for this path");
}
