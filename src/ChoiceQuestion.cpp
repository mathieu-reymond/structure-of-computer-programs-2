/*
 * ChoiceQuestion.cpp
 *
 *  Created on: Oct 14, 2013
 *      Author: Mathieu
 */

#include "ChoiceQuestion.h"
#include <iostream>
#include <sstream>

/**
 * a subclass of Question, the answer must be one of the specified choices
 */
ChoiceQuestion::ChoiceQuestion(std::string question) : AnswerQuestion<int>(question), choices_() {
}

ChoiceQuestion::~ChoiceQuestion() {
}
/**
 * Get the choice at a given index i
 */
std::string ChoiceQuestion::getChoice(int i) const {
	return choices_.at(i);
}
/**
 * The number of choices this ChoiceQuestion has
 */
int ChoiceQuestion::numberOfChoices() const {
	return choices_.size();
}

/**
 * add a choice
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
		getline(std::cin, s);
	} while (!isValidAnswer(s));
	setAnswer(convertString(s));
}

/**
 * private method to check an answer's validity (by converting answer to integer)
 */
bool ChoiceQuestion::isValidAnswer(std::string a) const{
	int conv(convertString(a));
	return conv >=1 && conv <= numberOfChoices();
}

int ChoiceQuestion::convertString(std::string s) const{
	std::istringstream strm(s);
	int conv;
	strm >> conv;
	return conv;
}

std::ostream& ChoiceQuestion::save(std::ostream& out) {
	return operator <<(out, *this);
}

std::string ChoiceQuestion::print() const {
	std::string type("CHOICE ");
	type.append(AnswerQuestion<int>::print());

	return type;
}

std::string ChoiceQuestion::printChoices() const {
	std::string str(" ");
	for(int i = 0; i < numberOfChoices(); ++i) {
		str.append("\n");
		str.append(getChoice(i));
	}
	return str;
}

std::ostream& operator<<(std::ostream& out, const ChoiceQuestion& choiceQuestion) {

	return out << choiceQuestion.print() << choiceQuestion.printChoices();
}
