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
 * @return the question text
 */
std::string Question::getQuestion() const {
	return question_;
}
/**
 * Set the question text to q
 * @param q the string to set
 */
void Question::setQuestion(std::string q) {
	question_ = q;
}
/**
 * save this Question
 * @param out the stream where this Question will be saved
 * @return the modified stream
 */
std::ostream& Question::save(std::ostream& out) {
	return operator<<(out, *this);
}
/**
 * Convert this Question to a string
 * @return a string representing this Question
 */
std::string Question::print() const{
	return getQuestion();
}
/**
 * Write this Question on a stream
 * @param out the stream where this Question will be written
 * @param question the Question to write
 * @return the modified stream
 */
std::ostream& operator<<(std::ostream& out, const Question& question) {
	return out << question.print();
}

