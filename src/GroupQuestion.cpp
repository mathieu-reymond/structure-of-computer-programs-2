/*
 * GroupQuestion.cpp
 *
 *  Created on: Dec 4, 2013
 *      Author: Mathieu
 */

#include "GroupQuestion.h"
#include <sstream>
#include <string>
/**
 * Makes a GroupQuestion
 * A GroupQuestion is a list containing other Questions
 * @param question the text fo this Question
 */
GroupQuestion::GroupQuestion(std::string question): Question(question) {
}

GroupQuestion::~GroupQuestion() {

}

void GroupQuestion::ask() {
	//ask nothing
}
/**
 * Get a copy from this GroupQuestion
 * @return a copy
 */
GroupQuestion* GroupQuestion::copy() {
	GroupQuestion* gq = new GroupQuestion(getQuestion());
	for(std::list<Question*>::iterator it = list_.begin(); it != list_.end(); ++it) {
		gq->add((*it)->copy());
	}
	return gq;
}
/**
 * save this Question
 * This does not save the Questions this GroupQuestion contains.
 * @param out the stream where this Question will be saved
 * @return the modified stream
 */
std::ostream& GroupQuestion::save(std::ostream& out) {
	return operator<<(out, *this);
}
/**
 * Convert this Question to a string
 * @return a string representing this Question
 */
std::string GroupQuestion::print() const {
	std::string type("GROUP ");
	std::stringstream strm;
	strm << list_.size();
	strm << " ";
	type.append(strm.str());
	type.append(Question::print());

	return type;
}

void GroupQuestion::add(Question* q) {
	QuestionList::add(q);
	if(isOptional() && !q->isOptional()) {
		std::string text(q->getQuestion());
		text.append("#opt");
		q->setQuestion(text);
	}
}
void GroupQuestion::insert(Question* q, Iterator previous) {
	QuestionList::insert(q, previous);
	if(isOptional() && !q->isOptional()) {
		std::string text(q->getQuestion());
		text.append("#opt");
		q->setQuestion(text);
	}
}

Wt::WTreeTableNode* GroupQuestion::widget() {
	Wt::WTreeTableNode* node = Question::widget();
	for(Iterator it = begin(); it != end(); it.levelForward()) {
		Wt::WTreeTableNode* child = (*it)->widget();
		node->addChildNode(child);
	}

	return node;
}
