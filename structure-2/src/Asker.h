/*
 * Asker.h
 *
 *  Created on: Oct 14, 2013
 *      Author: Mathieu
 */

#ifndef ASKER_H_
#define ASKER_H_

#include<string>
#include<set>
#include"Questionary.h"

class Asker {
public:
	Asker(std::string qFileName, std::string aFileName);
	Asker(Questionary& questionary);
	virtual ~Asker();

	void ask();
	void save();
private:
	void list();
	bool processCommand(std::string command, QuestionList::Iterator& it);
	bool isCompleted(QuestionList& ql);
	void updateAnswered();

	std::set<Question*> answered_;
	Questionary questionary_;
	std::string aFileName_; //where answers are written
};

#endif /* ASKER_H_ */
