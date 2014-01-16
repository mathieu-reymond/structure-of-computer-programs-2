/*
 * Asker.cpp
 *
 *  Created on: Oct 14, 2013
 *      Author: Mathieu
 */

#include "Asker.h"
#include "Path.h"
#include <iostream>
#include <sstream>
#include <list>

/**
 * Asker will ask the user to fill a Questionary and write the answers to a file
 * @param qFileName the file where the Questionary is written
 * @param aFileName the file where the answers will be written
 */
Asker::Asker(std::string qFileName, std::string aFileName) : questionary_(qFileName), aFileName_(aFileName){

}
/**
 * Asker will ask the user to fill a Questionary and write the answers to a file
 * @param questionary the Questionary to fill
 */
Asker::Asker(Questionary& questionary) :questionary_("id", 1), aFileName_("") {
	questionary_ = questionary;
}

Asker::~Asker() {
}

/**
 * Save the questionary to answer-file.
 * Only save if all Questions where answered
 */
void Asker::save() {
	if(isCompleted(questionary_)) {
		questionary_.saveAnswersToFile(aFileName_);
		std::cout << "Antwoorden werden opgeslagen in " << aFileName_ << std::endl;
	}
	else std::cout << "Niet opgeslagen : er zijn onbeantwoorde vragen" << std::endl;
}

/**
 * Ask the Questions to the user
 */
void Asker::ask() {
	std::cout << "Deze enquete bestaat uit " << questionary_.size() << " vragen :" <<std::endl << std::endl;

	bool cont = true;
	Questionary::Iterator it = questionary_.begin();
	while(cont) {
		if(dynamic_cast<QuestionList*>(*it) == NULL)
			std::cout << it.getPath().print() << " - ";
		try {
			(*it)->ask();
			if(dynamic_cast<QuestionList*>(*it) == NULL) {
				answered_.insert(*it);
				updateAnswered();
				if(isCompleted(questionary_)) std::cout << "Alle vragen werden ingevuld" << std::endl;
			}
			++it;
			if(it == questionary_.end()) {
				std::cout << "Einde van vragen, terug naar begin" << std::endl;
				it = questionary_.begin();
			}
		}
		catch(std::string& command) {
			//input begins with ':'
			cont = processCommand(command, it);
		}
	}

	std::cout << "Bedankt voor je deelname" << std::endl;
}
/**
 * Show a list of the Questionary
 */
void Asker::list() {
	for(QuestionList::Iterator it = questionary_.begin(); it != questionary_.end(); ++it) {
		for(int i = 1; i < it.getPath().size(); ++i) {
			std::cout << "  ";
		}
		std::cout << it.getPath().print() << " ";
		if(answered_.find(*it) == answered_.end()) std::cout << "NOK ";
		else std::cout << "OK ";
		std::cout << (*it)->getQuestion() << std::endl;
	}
}

/**
 * Checks if all the Questions of ql where answered
 * @param ql the QuestionList to check
 * @return true if completed, false otherwise
 */
bool Asker::isCompleted(QuestionList& ql) {
	for(QuestionList::Iterator it = ql.begin(); it != ql.end(); it.levelForward()) {
		if(answered_.find(*it) == answered_.end()) {
			return false;
		}
	}

	return true;
}

/**
 * Check for each QuestionList if all it's Questions where answered.
 * If so, add this QuestionList to the answered Questions
 */
void Asker::updateAnswered() {
	int size = answered_.size();
	for(QuestionList::Iterator it = questionary_.begin(); it != questionary_.end(); ++it) {
		QuestionList* ql = dynamic_cast<QuestionList*>(*it);
		if(ql) {
			if(isCompleted(*ql)) {
				answered_.insert(*it); //groupquestion completed, add to answered
			}
		}
		if(size < answered_.size()) {
			updateAnswered(); //by adding to answered, maybe another groupquestion completed
			break;
		}
	}
}

/**
 * Process a command.
 * Current commands are :next, :next N, :back, :back N, :goto P, :list, :quit
 *  with N=int and P=Path
 * @param command the command to process
 * @param it iterator pointing to the current Question
 * @return true of command != :quit, false otherwise
 */
bool Asker::processCommand(std::string command, QuestionList::Iterator& it) {
	bool cont(true);
	std::istringstream stream(command);
	std::string word;
	stream >> word;
	if(word == ":next") {
		int n;
		stream >> word;
		if(word == ":next") n = 1; //word didn't change
		else { //:next n
			std::istringstream convert(word);
			convert >> n;
		}
		if (it + n == questionary_.end()) {
			std::cout << "Geen vraag meer, annuleer commando" << std::endl;
		}
		else it = it +n;
	}
	else if(word == ":back") {
		stream >> word;
		int n;
		if(word == ":back") n = 1; //word didn't change
		else { //:next n
			std::stringstream convert(word);
			convert >> n;
		}
		if(it - n == questionary_.end()) {
			std::cout << "Geen vraag meer, annuleer commando" << std::endl;
		}
		else it = it - n;
	}
	else if(word == ":goto") {
		Path path;
		stream >> path;
		try {
			it = questionary_.iteratorForPath(path);
		}
		catch (std::out_of_range& oor) {
			std::cout << "Geen geldig pad" << std::endl;
		}
	}
	else if(word == ":list") {
		list();
	}
	else if(word == ":quit") {
		cont = false;
	}
	else std::cout << "Ongeldig commando" << std::endl;

	return cont;
}
