/*
 * OpenQuestion.cpp
 *
 *  Created on: Oct 14, 2013
 *      Author: Mathieu
 */

#include "OpenQuestion.h"

#include <boost/bind.hpp>
#include <iostream>

#include "Wt/WContainerWidget"
#include "Wt/WLineEdit"
#include "Wt/WSignal"
#include "Wt/WString"

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
 * @return a copy
 */
OpenQuestion* OpenQuestion::copy() {
	OpenQuestion *q = new OpenQuestion(question_);
	q->answer_ = getAnswer();
	return q;
}
/**
 * Ask the user to give an answer to this Question
 */
void OpenQuestion::ask() {
	std::cout << "Q : " << getQuestion() << std::endl << "A : ";
	std::string s;
	s = input();
	setAnswer(s);
}
/**
 * save this Question
 * @param out the stream where this Question will be saved
 * @return the modified stream
 */
std::ostream& OpenQuestion::save(std::ostream& out) {
	return operator <<(out, *this);
}
/**
 * @return the string, since there is no need to convert
 */
std::string OpenQuestion::convertString(std::string s) const{
	return s;
}
/**
 * Convert this Question to a string
 * @return a string representing this Question
 */
std::string OpenQuestion::print() const {
	std::string type("TEXT ");
	type.append(AnswerQuestion<std::string>::print());

	return type;
}
/**
 * Checks if a string is a valid answer to this Question
 * @param a the string to check
 * @return true, any answer is valid
 */
bool OpenQuestion::isValidAnswer(std::string a) const {
	return true;
}
/**
 * Write this Question on a stream
 * @param out the stream where this Question will be written
 * @param question the Question to write
 * @return the modified stream
 */
std::ostream& operator<<(std::ostream& out, const OpenQuestion& openQuestion) {
	return out << openQuestion.print();
}

Wt::WTreeTableNode* OpenQuestion::widget() {
	Wt::WTreeTableNode* node = Question::widget();

	Wt::WContainerWidget* container = new Wt::WContainerWidget();
	Wt::WLineEdit* text = new Wt::WLineEdit(container);
	text->changed().connect(boost::bind(&OpenQuestion::setAnswer, this, text->text().toUTF8()));

	node->setColumnWidget(1, container);
	return node;
}

