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

Questionary::~Questionary() {
	// TODO Auto-generated destructor stub
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

/**
 * get all the questions
 */
std::vector<Question*> Questionary::getQuestions() {
	return questions_;
}

/**
 * Save the answers of all this Questionary's questions to a file
 * @filename the name of the file
 */
void Questionary::saveAnswersToFile(std::string filename) {
	std::ofstream file(filename.c_str());
	file << "ID " << getID() << std::endl;
	for(int i = 0; i < getSteps(); ++i) {
		file << i+1 << " " << getQuestions().at(i)->getAnswer() << std::endl;
	}
}

/**
 * returns a questionary from a specified file
 * @filename the name of the questionary file
 */
Questionary loadQuestionsFromFile(std::string filename) {
	Questionary* questionary(NULL);
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
			Questionary q(id, version);
			questionary = &q;
		}
		else if (word == "STEPS") {
			//nothing to do, steps = vector<Question>.size()
		}
		//current question is multiple choice : different choices are in next lines
		else if (choicesLeft > 0) {
			question->addChoice(line);
			choicesLeft--;
			//all choices added : add question to questionary
			if(choicesLeft == 0) questionary->addQuestion(question);
		}
		else { //new question
			lineStream >> word;
			if (word == "TEXT") {
				getline(lineStream, line);
				OpenQuestion q(line);
				questionary->addQuestion(new OpenQuestion(line));
			}
			else { //word == choice
				lineStream >> word;
				std::istringstream itos(word);
				itos >> choicesLeft;
				getline(lineStream, line);
				question = new ChoiceQuestion(line);
			}
		}
	}

	return *questionary;
}
