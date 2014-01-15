/*
 * Question.h
 *
 *  Created on: Oct 16, 2013
 *      Author: Mathieu
 */

#ifndef QUESTION_H_
#define QUESTION_H_

#include"Wt/WTreeTableNode"
#include <string>

class Question {
public:
	Question(std::string question);
	virtual ~Question();

	std::string getQuestion() const;
	void setQuestion(std::string q);
	virtual bool isOptional();

	virtual void ask() = 0;
	virtual std::ostream& save(std::ostream& out);

	virtual Question* copy() = 0;

	virtual Wt::WTreeTableNode* widget();

	friend std::ostream& operator<<(std::ostream& out, const Question& question);

protected:
	virtual std::string print() const;

	std::string question_;
	bool optional_;

};

#endif /* QUESTION_H_ */
