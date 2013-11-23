/*
 * Questionary.cpp
 *
 *  Created on: Oct 14, 2013
 *      Author: Mathieu
 */

#include "Questionary.h"
#include "OpenQuestion.h"
#include "ChoiceQuestion.h"
#include <fstream>
#include <sstream>
#include <iostream>

/**
 * A questionary containing different Questions
 * Questionary has a unique ID and a version
 */
Questionary::Questionary(std::string id, int version) {
	id_ = id;
	version_ = version;
}

Questionary::Questionary(std::string filename) {
	loadQuestionsFromFile(filename);
}

Questionary::~Questionary() {
	// TODO Auto-generated destructor stub
	//for(std::list<Question*>::iterator it = questions_.begin(); it != questions_.end(); ++it) {
	//	delete (*it);
	//}
	//questions_.clear();
}

/**
 * get this Questionary's ID
 */
std::string Questionary::getID() {
	return id_;
}

/**
 * get the number of questions this Questionary contains
 */
int Questionary::getSteps() {
	return questions_.size();
}

/**
 * add a new question to this Questionary
 */
void Questionary::addQuestion(Question* question) {
	questions_.push_back(question);
}

void Questionary::insertQuestion(Question* question, int position) {
	std::list<Question*>::iterator it = questions_.begin();
	for(int i = 0; i < position; ++i) {
		++it;
	}
	questions_.insert(it, question);
}

void Questionary::removeQuestion(Question* question) {
	questions_.remove(question);
}

/**
 * get all the questions
 */
std::list<Question*> Questionary::getQuestions() {
	return questions_;
}

/**
 * Save the answers of all this Questionary's questions to a file
 * @filename the name of the file
 */
void Questionary::saveAnswersToFile(std::string filename) {
	std::ofstream file(filename.c_str());
	file << "ID " << getID() << std::endl;
	int i = 1;
	for(std::list<Question*>::iterator it = questions_.begin(); it != questions_.end(); ++it) {
		file << i << " " << (*it)->getAnswer() << std::endl;
		i++;
	}
}

void Questionary::saveQuestionsToFile(std::string filename) {
	std::ofstream file(filename.c_str());
	file << "VERSION " << version_ << std::endl;
	file << "ID " << getID() << std::endl;
	file << "STEPS " << getSteps();
	int i = 1;
	for(std::list<Question*>::iterator it = questions_.begin(); it != questions_.end(); ++it) {
		file << std::endl << i << " ";
		if(typeid(*(*it)).name() == typeid(OpenQuestion).name()) {
			file << "TEXT " << (*it)->getQuestion();
		}
		else if (typeid(*(*it)).name() == typeid(ChoiceQuestion).name()) {
			file << "CHOICE " << ((ChoiceQuestion *) (*it))->getChoices().size() << " ";
			file << (*it)->getQuestion();
			for(int j = 0; j < ((ChoiceQuestion *) (*it))->getChoices().size(); ++j){
				file << std::endl << ((ChoiceQuestion *) (*it))->getChoices().at(j);
			}
		}
		i++;
	}
}

void Questionary::loadQuestionsFromFile(std::string filename) {
	int version(0);
	std::string id("");

	std::ifstream file(filename.c_str());
	std::string line;
	int choicesLeft(0);
	ChoiceQuestion* question(NULL);

	//loop over each line until the end of file
	while(!file.eof()) {
		//get current line
		getline(file, line);
		std::istringstream lineStream(line);
		std::string word;
		//first word of current line
		lineStream >> word;
		if(word == "VERSION") {
			lineStream >> word;
			std::istringstream stoi(word);
			stoi >> version;
		}
		else if (word == "ID") {
			lineStream >> word;
			id = word;
			id_ = id;
			version_ = version;
		}
		else if (word == "STEPS") {
			//nothing to do, steps = vector<Question>.size()
		}
		//current question is multiple choice : different choices are in next lines
		else if (choicesLeft > 0) {
			question->addChoice(line);
			choicesLeft--;
			//all choices added : add question to questionary
			if(choicesLeft == 0) addQuestion(question);
		}
		else { //new question
			lineStream >> word;
			if (word == "TEXT") {
				getline(lineStream, line);
				//OpenQuestion q(line);
				addQuestion(new OpenQuestion(line.substr(1, std::string::npos)));
			}
			else { //word == choice
				lineStream >> word;
				std::istringstream itos(word);
				itos >> choicesLeft;
				getline(lineStream, line);
				question = new ChoiceQuestion(line.substr(1, std::string::npos));
			}
		}
	}
}
