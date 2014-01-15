/*
 * ChoiceQuestion.h
 *
 *  Created on: Oct 14, 2013
 *      Author: Mathieu
 */

#ifndef CHOICEQUESTION_H_
#define CHOICEQUESTION_H_

#include "AnswerQuestion.h"
#include<vector>
#include<string>

class ChoiceQuestion: public AnswerQuestion<int> {
public:
	ChoiceQuestion(std::string question);
	virtual ~ChoiceQuestion();

	std::string getChoice(int i) const;
	int numberOfChoices() const;
	void addChoice(std::string choice);
	void removeChoices();

	virtual ChoiceQuestion* copy();

	virtual void ask();
	virtual std::ostream& save(std::ostream& out);

	virtual Wt::WContainerWidget* widget();

	friend std::ostream& operator<<(std::ostream& out, const ChoiceQuestion& choiceQuestion);

private:
	std::vector<std::string> choices_;

	virtual std::string print() const;
	virtual std::string printChoices() const;
	virtual int convertString(std::string) const;
	bool isValidAnswer(std::string a) const;
};

#endif /* CHOICEQUESTION_H_ */
