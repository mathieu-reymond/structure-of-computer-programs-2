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

	std::string getChoice(int i) const;
	int numberOfChoices() const;
	void setAnswer(std::string answer);
	void addChoice(std::string choice);
	void removeChoices();

	virtual ChoiceQuestion* copy();

	virtual void ask();
	virtual std::ostream& save(std::ostream& out);

private:
	std::vector<std::string> choices_;

	bool isValidAnswer(std::string a);
};

std::ostream& operator<<(std::ostream& out, const ChoiceQuestion& choiceQuestion);

#endif /* CHOICEQUESTION_H_ */
