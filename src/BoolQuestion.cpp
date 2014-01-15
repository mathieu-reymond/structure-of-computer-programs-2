/*
 * YesNoQuestion.cpp
 *
 *  Created on: Dec 1, 2013
 *      Author: Mathieu
 */

#include "Wt/WText"
#include "Wt/WButtonGroup"
#include "Wt/WRadioButton"
#include "BoolQuestion.h"
#include<boost/bind.hpp>
#include<sstream>

BoolQuestion::BoolQuestion(std::string question) : AnswerQuestion<bool>(question) {

}

BoolQuestion::~BoolQuestion() {
}

/**
 * Gives a copy of this Question
 * @return a copy
 */
BoolQuestion* BoolQuestion::copy() {
	BoolQuestion* q = new BoolQuestion(*this);
	q->answer_ = getAnswer();
	return q;
}

/**
 * ask the user to give an answer to this Question
 */
void BoolQuestion::ask() {
	std::cout << "Q : " << getQuestion() << std::endl;
	std::string s;
	do {
		std::cout << "A (y, j, 1 / n, 0) : ";
		s = input();
	} while (!isValidAnswer(s));
	setAnswer(convertString(s));
}

/**
 * save this Question
 * @param out the stream where this Question will be saved
 * @return the modified stream
 */
std::ostream& BoolQuestion::save(std::ostream& out) {
	return operator<<(out, *this);
}
/**
 * Convert a string to a boolean
 * @param the string to convert
 * @return the converted boolean
 */
bool BoolQuestion::convertString(std::string s) const {
	if(s == "y" || s == "j" || s == "1" ) return true;
	else return false;
}
/**
 * Checks if a string is a valid answer to this Question
 * @param a the string to check
 * @return true if valid, false otherwise
 */
bool BoolQuestion::isValidAnswer(std::string a) const{
	return a == "y" || a == "j" || a == "1" || a == "n" || a == "0";
}
/**
 * Convert this Question to a string
 * @return a string representing this Question
 */
std::string BoolQuestion::print() const {
	std::string type("BOOL ");
	return type.append(AnswerQuestion<bool>::print());
}
/**
 * Write this Question on a stream
 * @param out the stream where this Question will be written
 * @param question the Question to write
 * @return the modified stream
 */
std::ostream& operator<<(std::ostream& out, const BoolQuestion& question) {
	return out << question.print();
}

Wt::WContainerWidget* BoolQuestion::widget() {

	Wt::WContainerWidget* container = new Wt::WContainerWidget();
	Wt::WButtonGroup* group = new Wt::WButtonGroup(container);
	Wt::WRadioButton* yes = new Wt::WRadioButton(container);
	Wt::WRadioButton* no = new Wt::WRadioButton(container);
	group->addButton(yes);
	group->addButton(no);
	yes->checked().connect(boost::bind(&BoolQuestion::setAnswer,  this, true));
	no->checked().connect(boost::bind(&BoolQuestion::setAnswer, this, false));

	return container;
}
