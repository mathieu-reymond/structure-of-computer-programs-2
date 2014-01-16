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
#include "QuestionList.h"
#include "Question.h"
#include "ChoiceQuestion.h"
#include "BoolQuestion.h"
#include "GroupQuestion.h"
#include "OpenQuestion.h"
#include "ScaleQuestion.h"

class Questionary : public QuestionList{
public:

	Questionary(std::string id, int version);
	Questionary(std::string filename);
	virtual ~Questionary();
	Questionary& operator=(const Questionary& questionary);

	int getVersion() const;
	std::string getID() const;

	void setVersion(int version);

	void saveQuestionsToFile(std::string filename);
	void saveAnswersToFile(std::string filename);

private:
	void loadQuestionsFromFile(std::string filename);

	Question* loadQuestion(std::fstream& file);
	ChoiceQuestion* loadChoiceQuestion(std::fstream& file, std::istringstream& line);
	ScaleQuestion* loadScaleQuestion(std::istringstream& line);
	GroupQuestion* loadGroupQuestion(std::fstream& file, std::istringstream& line);

	int version_;
	std::string id_;
};


#endif /* QUESTIONARY_H_ */
