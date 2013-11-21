/*
 * ChoiceQuestion.h
 *
 *  Created on: Oct 14, 2013
 *      Author: Mathieu
 */

#ifndef CHOICEQUESTION_H_
#define CHOICEQUESTION_H_

#include "Question.h"
#include<vector>
#include<string>

class ChoiceQuestion: public Question {
public:
	ChoiceQuestion(std::string question);
	virtual ~ChoiceQuestion();

	void setAnswer(std::string answer);
	void addChoice(std::string choice);
	void removeChoices();
	std::vector<std::string> getChoices();
	virtual void ask();

private:
	std::vector<std::string> choices_;

	bool isValidAnswer(std::string a);
};

#endif /* CHOICEQUESTION_H_ */
