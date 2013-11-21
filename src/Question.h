/*
 * Question.h
 *
 *  Created on: Oct 16, 2013
 *      Author: Mathieu
 */

#ifndef QUESTION_H_
#define QUESTION_H_

#include <string>

class Question {
public:
	Question(std::string question);
	virtual ~Question();

	std::string getQuestion();
	void setQuestion(std::string q);
	std::string getAnswer();
	void setAnswer(std::string answer);
	virtual void ask();

	std::string toString();

protected:
	std::string question_;
	std::string answer_;
};

#endif /* QUESTION_H_ */
