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
ChoiceQuestion::ChoiceQuestion(std::string question) : Question(question), choices_() {
}

ChoiceQuestion::~ChoiceQuestion() {
	// TODO Auto-generated destructor stub
}


void ChoiceQuestion::setAnswer(std::string answer) {
	answer_ = answer;
}

/**
 * add a choice
 */
void ChoiceQuestion::addChoice(std::string choice) {
	choices_.push_back(choice);
}

std::vector<std::string> ChoiceQuestion::getChoices() {
	return choices_;
}

/**
 * Ask the user to answer the Question
 * only accept a valid answer (one of the choices)
 */
void ChoiceQuestion::ask() {
	std::cout << "Q : " << getQuestion() << std::endl;
	for(int i = 0; i < getChoices().size(); ++i) {
		std::cout << " " << (i+1) << ") : " << getChoices().at(i) << std::endl;
	}

	std::string s;
	do {
		std::cout << "A (1 - " << getChoices().size() << ") : ";
		getline(std::cin, s);
	} while (!isValidAnswer(s));
	setAnswer(s);
}

/**
 * private method to check an answer's validity (by converting answer to integer)
 */
bool ChoiceQuestion::isValidAnswer(std::string a) {
	std::istringstream strm(a);
	int conv;
	strm >> conv;
	return conv >=1 && conv <= getChoices().size();
}
