/*
 * ScaleQuestion.cpp
 *
 *  Created on: Dec 1, 2013
 *      Author: Mathieu
 */

#include "ScaleQuestion.h"

#include <boost/bind.hpp>
#include <iostream>
//#include <sstream>

#include "Wt/WContainerWidget"
#include "Wt/WSignal"
#include "Wt/WSlider"
#include "Wt/WTreeTableNode"

/**
 * Makes a new ScaleQuestion
 * @param question the text
 * @param lowerLimit the lower limit this ScaleQuestion
 * @param upperLimit the upper limit this ScaleQuestion
 */
ScaleQuestion::ScaleQuestion(std::string question, int lowerLimit, int upperLimit) :
AnswerQuestion<int>(question), lower_(lowerLimit), upper_(upperLimit) {

}

ScaleQuestion::~ScaleQuestion() {
}
/**
 * get the lower limit
 * @return the lower limit
 */
int ScaleQuestion::getLowerLimit() const {
	return lower_;
}
/**
 * get the upper limit
 * @return the upper limit
 */
int ScaleQuestion::getUpperLimit() const {
	return upper_;
}
/**
 * Gives a copy of this Question
 * @return a copy
 */
ScaleQuestion* ScaleQuestion::copy() {
	ScaleQuestion* q = new ScaleQuestion(getQuestion(), getLowerLimit(), getUpperLimit());
	q->setAnswer(getAnswer());
	return q;
}

/**
 * ask the user to give an answer to this Question
 */
void ScaleQuestion::ask() {
	std::cout << "Q : " << getQuestion() << std::endl;
	std::string s;
	do {
		std::cout << "A (" << getLowerLimit() << " - " << getUpperLimit() << ") : ";
		s = input();
	} while (!isValidAnswer(s));
	setAnswer(convertString(s));
}
/**
 * save this Question
 * @param out the stream where this Question will be saved
 * @return the modified stream
 */
std::ostream& ScaleQuestion::save(std::ostream& out) {
	out << "SCALE " << getLowerLimit() << " " << getUpperLimit() << " " << AnswerQuestion<int>::print();
	return out;
}
/**
 * convert a string to an integer
 * @param s the string to convert
 * @return the converted integer
 */
int ScaleQuestion::convertString(std::string s) const{
	std::istringstream strm(s);
	int conv;
	strm >> conv;
	return conv;
}
/**
 * Convert this Question to a string
 * @return a string representing this Question
 */
std::string ScaleQuestion::print() const {
	std::string type("SCALE ");
	type.append(AnswerQuestion<int>::print());

	return type;
}
/**
 * Checks if a is a valid answer
 * @param a the answer to check
 * @return true if between lower and upper limits, false otherwise
 */
bool ScaleQuestion::isValidAnswer(std::string a) const {
	int conv(convertString(a));
	return conv >= getLowerLimit() && conv <= getUpperLimit();
}
/**
 * Write this Question on a stream
 * @param out the stream where this Question will be written
 * @param question the Question to write
 * @return the modified stream
 */
std::ostream& operator<<(std::ostream& out, const ScaleQuestion& question) {
	return out << question.print();
}

Wt::WContainerWidget* ScaleQuestion::widget() {

	Wt::WContainerWidget* container = new Wt::WContainerWidget();
	Wt::WSlider* slider = new Wt::WSlider(container);

	slider->setMinimum(getLowerLimit());
	slider->setMaximum(getUpperLimit());
	slider->setTickInterval(1);
	slider->setTickPosition(Wt::WSlider::TicksAbove);
	slider->setValue(getLowerLimit());

	slider->valueChanged().connect(boost::bind(&ScaleQuestion::setAnswer, this, slider->value()));

	return container;
}

