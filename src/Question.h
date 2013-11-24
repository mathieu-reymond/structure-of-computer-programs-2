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

	std::string getQuestion() const;
	std::string getAnswer() const;
	void setQuestion(std::string q);
	virtual void setAnswer(std::string answer);

	virtual void ask();
	virtual std::ostream& save(std::ostream& out);

	virtual Question* copy();

protected:
	std::string question_;
	std::string answer_;
};

std::ostream& operator<<(std::ostream& out, const Question& question);

#endif /* QUESTION_H_ */
