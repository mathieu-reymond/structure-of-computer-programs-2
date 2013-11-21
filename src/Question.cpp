/*
 * Question.cpp
 *
 *  Created on: Oct 16, 2013
 *      Author: Mathieu
 */

#include "Question.h"
#include <iostream>

/**
 * A Question class containing a question and an answer
 */
Question::Question(std::string question) {
	question_ = question;
	answer_ = "";
}

Question::~Question() {
	// TODO Auto-generated destructor stub
}

std::string Question::getQuestion() {
	return question_;
}
void Question::setQuestion(std::string q) {
	question_ = q;
}
std::string Question::getAnswer() {
	return answer_;
}
void Question::setAnswer(std::string answer) {
	answer_ = answer;
}

/**
 * ask the user to give an answer to this Question
 */
void Question::ask() {
	std::cout << "Q : " << getQuestion() << std::endl << "A : ";
	std::string s;
	getline(std::cin, s);
	//std::cin >> s;
	setAnswer(s);
}

