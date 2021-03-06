/*
 * Editor.h
 *
 *  Created on: Nov 15, 2013
 *      Author: Mathieu
 */

#ifndef EDITOR_H_
#define EDITOR_H_

#include "Questionary.h"
#include "OpenQuestion.h"
#include "ChoiceQuestion.h"

class Editor {
public:
	Editor(std::string filename);
	virtual ~Editor();

	void run();

private:
	bool processCommand();
	void list();
	void questionDispatch(std::stringstream& stream, QuestionList::Iterator next);
	void insert(Question *question, QuestionList::Iterator position);
	void insertChoice(ChoiceQuestion *question, QuestionList::Iterator position);
	void insertScale(std::string, QuestionList::Iterator position);
	void editQuestion(Question *question);
	void editChoices(ChoiceQuestion *question);
	void remove(Question *question);
	void test();
	void save();
	void quit();

	bool setChoices(ChoiceQuestion *question);
	bool isValidPosition(int position);

	Questionary questionary_;
	std::string filename_;
	bool unsavedChanges;
};

#endif /* EDITOR_H_ */
