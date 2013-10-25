/*
 * Questionary.h
 *
 *  Created on: Oct 14, 2013
 *      Author: Mathieu
 */

#ifndef QUESTIONARY_H_
#define QUESTIONARY_H_

#include<vector>
#include<string>
#include "Question.h"

class Questionary {
public:
	Questionary(std::string id, int version);
	virtual ~Questionary();

	int getSteps();
	std::string getID();
	void addQuestion(Question* question);
	std::vector<Question*> getQuestions();
	void saveAnswersToFile(std::string filename);

private:
	int version_;
	std::string id_;
	std::vector<Question*> questions_;
};

Questionary loadQuestionsFromFile(std::string filename);
void print(Questionary);

#endif /* QUESTIONARY_H_ */
