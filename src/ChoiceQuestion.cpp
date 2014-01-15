/*
 * ChoiceQuestion.cpp
 *
 *  Created on: Oct 14, 2013
 *      Author: Mathieu
 */

#include "ChoiceQuestion.h"

#include <boost/bind.hpp>
#include <iostream>
//#include <sstream>

#include "Wt/WComboBox"
#include "Wt/WContainerWidget"
#include "Wt/WSignal"
#include "Wt/WTreeTableNode"

/**
 * a subclass of Question, the answer must be one of the specified choices
 */
ChoiceQuestion::ChoiceQuestion(std::string question) : AnswerQuestion<int>(question), choices_() {
}

ChoiceQuestion::~ChoiceQuestion() {
}
/**
 * Get the choice at a given index i
 * @param i the index
 * @return the choice at i
 */
std::string ChoiceQuestion::getChoice(int i) const {
	return choices_.at(i);
}
/**
 * The number of choices this ChoiceQuestion has
 * @return the number of choices
 */
int ChoiceQuestion::numberOfChoices() const {
	return choices_.size();
}

/**
 * add a choice
 * @param the choice to add
 */
void ChoiceQuestion::addChoice(std::string choice) {
	choices_.push_back(choice);
}
/**
 * remove all the choices
 */
void ChoiceQuestion::removeChoices() {
	choices_.clear();
}
/**
 * Get a copy from this ChoiceQuestion
 * @return a copy
 */
ChoiceQuestion* ChoiceQuestion::copy() {
	ChoiceQuestion* q = new ChoiceQuestion(getQuestion());
	q->answer_ = getAnswer();
	q->choices_ = choices_;
	return q;
}

/**
 * Ask the user to answer the Question
 * only accept a valid answer (one of the choices)
 */
void ChoiceQuestion::ask() {
	std::cout << "Q : " << getQuestion() << std::endl;
	for(int i = 0; i < numberOfChoices(); ++i) {
		std::cout << " " << (i+1) << ") : " << getChoice(i) << std::endl;
	}

	std::string s;
	do {
		std::cout << "A (1 - " << numberOfChoices() << ") : ";
		s = input();
	} while (!isValidAnswer(s));
	setAnswer(convertString(s));
}

/**
 * Checks if a string is a valid answer to this Question
 * @param a the string to check
 * @return true if valid, false otherwise
 */
bool ChoiceQuestion::isValidAnswer(std::string a) const{
	int conv(convertString(a));
	return conv >=1 && conv <= numberOfChoices();
}
/**
 * Convert a string to an integer
 * @param the string to convert
 * @return the converted integer
 */
int ChoiceQuestion::convertString(std::string s) const{
	std::istringstream strm(s);
	int conv;
	strm >> conv;
	return conv;
}
/**
 * save this Question
 * @param out the stream where this Question will be saved
 * @return the modified stream
 */
std::ostream& ChoiceQuestion::save(std::ostream& out) {
	out << "CHOICE " << numberOfChoices() << AnswerQuestion<int>::print() << printChoices();
	return out;
}
/**
 * Convert this Question to a string
 * @return a string representing this Question
 */
std::string ChoiceQuestion::print() const {
	std::string type("CHOICE ");
	type.append(AnswerQuestion<int>::print());

	return type;
}
/**
 * Convert the choices to a string
 * @return a string containing all the choices
 */
std::string ChoiceQuestion::printChoices() const {
	std::string str(" ");
	for(int i = 0; i < numberOfChoices(); ++i) {
		str.append("\n");
		str.append(getChoice(i));
	}
	return str;
}
/**
 * Write this Question on a stream
 * @param out the stream where this Question will be written
 * @param question the Question to write
 * @return the modified stream
 */
std::ostream& operator<<(std::ostream& out, const ChoiceQuestion& choiceQuestion) {

	return out << choiceQuestion.print() << choiceQuestion.printChoices();
}

Wt::WContainerWidget* ChoiceQuestion::widget() {

	Wt::WContainerWidget* container = new Wt::WContainerWidget();
	Wt::WComboBox* comboBox = new Wt::WComboBox(container);
	for(int i = 0; i < numberOfChoices(); ++i) {
		comboBox->addItem(getChoice(i));
	}
	comboBox->changed().connect(boost::bind(&ChoiceQuestion::setAnswer, this, comboBox->currentIndex()+1));

	return container;
}
