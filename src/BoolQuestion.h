/*
 * YesNoQuestion.h
 *
 *  Created on: Dec 1, 2013
 *      Author: Mathieu
 */

#ifndef BOOLQUESTION_H_
#define BOOLQUESTION_H_

#include "Wt/WContainerWidget"
#include "AnswerQuestion.h"
#include <string>

class BoolQuestion : public AnswerQuestion<bool> {
public:
	BoolQuestion(std::string question);
	virtual ~BoolQuestion();

	virtual BoolQuestion* copy();

	virtual void ask();
	virtual std::ostream& save(std::ostream& out);

	virtual Wt::WTreeTableNode* widget();

	friend std::ostream& operator<<(std::ostream& out, const BoolQuestion& question);

private:
	virtual std::string print() const;
	virtual bool convertString(std::string) const;
	virtual bool isValidAnswer(std::string a) const;
};



#endif /* BOOLQUESTION_H_ */
