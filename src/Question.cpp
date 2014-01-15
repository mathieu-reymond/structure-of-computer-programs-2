/*
 * Question.cpp
 *
 *  Created on: Oct 16, 2013
 *      Author: Mathieu
 */

#include "Question.h"

#include <iostream>

#include "Wt/WColor"
#include "Wt/WCssDecorationStyle"

/**
 * A Question class containing a question and an answer
 */
Question::Question(std::string question) {
	question_ = question;
	optional_ = false;
	std::string optional("#opt");
	if(question.length() > 4 && question.substr(question.length() - optional.length(),  std::string::npos) == optional) {
		question_ = question.substr(0, question.length() - 4);
		optional_ = true;
	}

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
	std::string optional("#opt");
	if(q.length() > 4 && q.substr(q.length() - optional.length(),  std::string::npos) == optional) {
		question_ = q.substr(0, q.length() - 4);
		optional_ = true;
	}
}

bool Question::isOptional() {
	return optional_;
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

