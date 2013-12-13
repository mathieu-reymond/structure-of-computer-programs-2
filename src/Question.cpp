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
}

Question::~Question() {
}

/**
 * Get the question text
 */
std::string Question::getQuestion() const {
	return question_;
}
/**
 * Set the question text to q
 */
void Question::setQuestion(std::string q) {
	question_ = q;
}

std::ostream& Question::save(std::ostream& out) {
	return operator<<(out, *this);
}

std::string Question::print() const{
	return getQuestion();
}

std::ostream& operator<<(std::ostream& out, const Question& question) {
	return out << question.print();
}

