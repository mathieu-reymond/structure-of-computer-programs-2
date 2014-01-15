/*
 * OpenQuestion.h
 *
 *  Created on: Oct 14, 2013
 *      Author: Mathieu
 */

#ifndef OPENQUESTION_H_
#define OPENQUESTION_H_

#include "Wt/WContainerWidget"
#include "AnswerQuestion.h"
#include<string>

class OpenQuestion: public AnswerQuestion<std::string> {
public:
	OpenQuestion(std::string question);
	virtual ~OpenQuestion();

	virtual OpenQuestion* copy();

	virtual void ask();
	virtual std::ostream& save(std::ostream& out);

	virtual Wt::WContainerWidget* widget();

	friend std::ostream& operator<<(std::ostream& out, const OpenQuestion& openQuestion);

private:
	virtual std::string convertString(std::string) const;
	virtual std::string print() const;
	virtual bool isValidAnswer(std::string a) const;

};

#endif /* OPENQUESTION_H_ */
