/*
 * Questionary.h
 *
 *  Created on: Oct 14, 2013
 *      Author: Mathieu
 */

#ifndef QUESTIONARY_H_
#define QUESTIONARY_H_

#include<vector>
#include<list>
#include<string>
#include "Question.h"

class Questionary {
public:
	Questionary(std::string id, int version);
	Questionary(std::string filename);
	virtual ~Questionary();

	int getSteps();
	std::string getID();
	void addQuestion(Question* question);
	void insertQuestion(Question* question, int position);
	void removeQuestion(Question* question);
	std::list<Question*> getQuestions();
	void saveAnswersToFile(std::string filename);
	void saveQuestionsToFile(std::string filename);
	void loadQuestionsFromFile(std::string filename);

private:
	int version_;
	std::string id_;
	std::list<Question*> questions_;
};


#endif /* QUESTIONARY_H_ */
