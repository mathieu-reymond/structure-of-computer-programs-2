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
 * An iterator that gives the first Question of this Questionary
 */
Questionary::Iterator Questionary::begin(){
	return questions_.begin();
}

Questionary::Iterator Questionary::end(){
	return questions_.end();
}

/**
 * A questionary containing different Questions
 * Questionary has a unique ID and a version
 */
Questionary::Questionary(std::string id, int version) {
	id_ = id;
	version_ = version;
}
/**
 * Load a Questionary from a given filename
 */
Questionary::Questionary(std::string filename) {
	loadQuestionsFromFile(filename);
}
/**
 * Destructor : delete all questions in this Questionary
 */
Questionary::~Questionary() {
	//delete all questions
	for(Iterator it = begin(); it != end(); ++it) {
		delete (*it);
	}
	questions_.clear();
}
/**
 * Assignment operator
 */
Questionary& Questionary::operator=(const Questionary& questionary) {
	if(this == &questionary) return *this;
	id_ = questionary.id_;
	version_ = questionary.version_;
	//delete old questions
	for(Iterator it = begin(); it != end(); ++it) {
		delete (*it);
	}
	//clear question list
	questions_.clear();
	//fill questions with copies of questions from param
	for(std::list<Question*>::const_iterator it = questionary.questions_.begin();
		it != questionary.questions_.end();
		++it) {
		addQuestion((*it)->copy());
	}
	return *this;
}
/**
 * Gives this Questionary's version
 */
int Questionary::getVersion() const {
	return version_;
}

/**
 * get the number of questions this Questionary contains
 */
int Questionary::getSteps() const{
	return questions_.size();
}

/**
 * get this Questionary's ID
 */
std::string Questionary::getID() const{
	return id_;
}

/**
 * add a new question to this Questionary
 */
void Questionary::addQuestion(Question* question) {
	questions_.push_back(question);
}
/**
 * insert a Question after the Question pointed by Iterator
 */
void Questionary::insertQuestion(Question* question, Iterator previous) {
	questions_.insert(previous, question);
}
/**
 * remove a given Question
 */
void Questionary::removeQuestion(Question* question) {
	questions_.remove(question);
}

/**
 * Save the answers of all this Questionary's questions to a file
 * @filename the name of the file
 */
void Questionary::saveAnswersToFile(std::string filename) {
	std::ofstream file(filename.c_str());
	file << "ID " << getID();
	int i = 1;
	for(Questionary::Iterator it = begin(); it != end(); ++it) {
		file << std::endl << i << " " << (*it)->getAnswer();
		i++;
	}
}
/**
 * Save the Question's to a file
 */
void Questionary::saveQuestionsToFile(std::string filename) {
	std::ofstream file(filename.c_str());
		file << "VERSION " << version_ << std::endl;
		file << "ID " << getID() << std::endl;
		file << "STEPS " << getSteps();
		int i = 1;
		for(Questionary::Iterator it = begin(); it != end(); ++it) {
			file << std::endl << i << " ";
			(*it)->save(file);
			i++;
		}
}
/**
 * Load a Questionary from a given file
 */
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
