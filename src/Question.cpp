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

/**
 * Get the question text
 */
std::string Question::getQuestion() const {
	return question_;
}
/**
 * Get the answer to the question
 */
std::string Question::getAnswer() const {
	return answer_;
}
/**
 * Set the question text to q
 */
void Question::setQuestion(std::string q) {
	question_ = q;
}
/**
 * Set the answer to the string answer
 */
void Question::setAnswer(std::string answer) {
	answer_ = answer;
}
/**
 * Gives a copy of this Question
 */
Question* Question::copy() {
	Question* q = new Question(*this);
	q->answer_ = getAnswer();
	return q;
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

std::ostream& Question::save(std::ostream& out) {
	return operator<<(out, *this);
}

std::ostream& operator<<(std::ostream& out, const Question& question) {
	return out << question.getQuestion();
}

