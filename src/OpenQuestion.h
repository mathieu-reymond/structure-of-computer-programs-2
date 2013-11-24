/*
 * OpenQuestion.h
 *
 *  Created on: Oct 14, 2013
 *      Author: Mathieu
 */

#ifndef OPENQUESTION_H_
#define OPENQUESTION_H_

#include "Question.h"
#include<string>

class OpenQuestion: public Question {
public:
	OpenQuestion(std::string question);
	virtual ~OpenQuestion();

	virtual OpenQuestion* copy();

	virtual std::ostream& save(std::ostream& out);

};

std::ostream& operator<<(std::ostream& out, const OpenQuestion& openQuestion);

#endif /* OPENQUESTION_H_ */
