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

};

#endif /* OPENQUESTION_H_ */
