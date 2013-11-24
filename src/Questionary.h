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
	typedef std::list<Question*>::iterator Iterator;
	Iterator begin();
	Iterator end();

	Questionary(std::string id, int version);
	Questionary(std::string filename);
	virtual ~Questionary();
	Questionary& operator=(const Questionary& questionary);

	int getVersion() const;
	int getSteps() const;
	std::string getID() const;
	void addQuestion(Question* question);
	void insertQuestion(Question* question, Iterator previous);
	void removeQuestion(Question* question);

	void saveAnswersToFile(std::string filename);
	void saveQuestionsToFile(std::string filename);

private:
	void loadQuestionsFromFile(std::string filename);

	int version_;
	std::string id_;
	std::list<Question*> questions_;
};


#endif /* QUESTIONARY_H_ */
