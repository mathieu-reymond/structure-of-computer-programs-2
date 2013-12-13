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
OpenQuestion::OpenQuestion(std::string question) : AnswerQuestion<std::string>(question) {

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

void OpenQuestion::ask() {
	std::cout << "Q : " << getQuestion() << std::endl << "A : ";
	std::string s;
	getline(std::cin, s);
	setAnswer(s);
}

std::ostream& OpenQuestion::save(std::ostream& out) {
	return operator <<(out, *this);
}

std::string OpenQuestion::convertString(std::string s) const{
	return s;
}

std::string OpenQuestion::print() const {
	std::string type("TEXT ");
	type.append(AnswerQuestion<std::string>::print());

	return type;
}

bool OpenQuestion::isValidAnswer(std::string a) const {
	return true;
}

std::ostream& operator<<(std::ostream& out, const OpenQuestion& openQuestion) {
	return out << openQuestion.print();
}


