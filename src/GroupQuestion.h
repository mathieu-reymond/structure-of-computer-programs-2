/*
 * GroupQuestion.h
 *
 *  Created on: Dec 4, 2013
 *      Author: Mathieu
 */

#ifndef GROUPQUESTION_H_
#define GROUPQUESTION_H_

#include "Question.h";
#include "QuestionList.h"
#include<string>

class GroupQuestion : public QuestionList, public Question{
public:
	GroupQuestion(std::string question);
	virtual ~GroupQuestion();

	virtual void ask();
	virtual GroupQuestion* copy();
	virtual std::ostream& save(std::ostream& out);

	virtual std::string print() const;

	friend std::ostream& operator<<(std::ostream& out, const GroupQuestion& question) {
		return out << question.print();
	}
};

#endif /* GROUPQUESTION_H_ */
