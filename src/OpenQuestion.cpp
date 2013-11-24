/*
 * OpenQuestion.cpp
 *
 *  Created on: Oct 14, 2013
 *      Author: Mathieu
 */

#include "OpenQuestion.h"
#include <iostream>

/**
 * a subclass of Question, the answer is a line of text
 */
OpenQuestion::OpenQuestion(std::string question) : Question(question) {

}

OpenQuestion::~OpenQuestion() {
	// TODO Auto-generated destructor stub
}
/**
 * Get a copy of this OpenQuestion
 */
OpenQuestion* OpenQuestion::copy() {
	OpenQuestion *q = new OpenQuestion(question_);
	q->answer_ = getAnswer();
	return q;
}

std::ostream& OpenQuestion::save(std::ostream& out) {
	return operator <<(out, *this);
}

std::ostream& operator<<(std::ostream& out, const OpenQuestion& openQuestion) {
	return out << "TEXT " << (Question&) openQuestion;
}


