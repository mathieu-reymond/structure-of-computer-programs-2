/*
 * ScaleQuestion.h
 *
 *  Created on: Dec 1, 2013
 *      Author: Mathieu
 */

#ifndef SCALEQUESTION_H_
#define SCALEQUESTION_H_

#include"AnswerQuestion.h"
#include<string>

class ScaleQuestion : public AnswerQuestion<int> {
public:
	ScaleQuestion(std::string question, int lowerLimit, int upperLimit);
	virtual ~ScaleQuestion();

	int getLowerLimit() const;
	int getUpperLimit() const;

	virtual ScaleQuestion* copy();

	virtual void ask();
	virtual std::ostream& save(std::ostream& out);

	virtual Wt::WContainerWidget* widget();

	friend std::ostream& operator<<(std::ostream& out, const ScaleQuestion& question);

protected:
	virtual int convertString(std::string) const;
	virtual std::string print() const;
	virtual bool isValidAnswer(std::string a) const;

	int lower_;
	int upper_;
};

#endif /* SCALEQUESTION_H_ */
