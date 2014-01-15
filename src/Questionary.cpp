/*
 * Questionary.cpp
 *
 *  Created on: Oct 14, 2013
 *      Author: Mathieu
 */

#include "Questionary.h"
#include "OpenQuestion.h"
#include "ChoiceQuestion.h"
#include "AnswerQuestion.h"
#include <fstream>
#include <sstream>
#include <iostream>


/**
 * A questionary containing different Questions
 * Questionary has a unique ID and a version
 * @param id the unique ID
 * @param version the Questionary's version
 */
Questionary::Questionary(std::string id, int version) {
	id_ = id;
	version_ = version;
}
/**
 * Load a Questionary from a given filename
 * @param filename the name of the file
 */
Questionary::Questionary(std::string filename) {
	loadQuestionsFromFile(filename);
}
/**
 * Destructor : delete all questions in this Questionary
 */
Questionary::~Questionary() {
}
/**
 * Assignment operator
 * @param questionary the questionary this will be assigned to
 */
Questionary& Questionary::operator=(const Questionary& questionary) {
	if(this == &questionary) return *this;
	id_ = questionary.id_;
	version_ = questionary.version_;
	//delete old questions
	for(std::list<Question*>::iterator it = list_.begin(); it != list_.end(); ++it) {
		delete(*it);
	}
	//clear question list
	list_.clear();
	//fill questions with copies of questions from param
	for(std::list<Question*>::const_iterator it = questionary.list_.begin();
			it != questionary.list_.end();
			++it) {
		add((*it)->copy());
	}
	return *this;
}
/**
 * Get this Questionary's version
 * @return the version
 */
int Questionary::getVersion() const {
	return version_;
}

/**
 * get this Questionary's ID
 * @return the ID
 */
std::string Questionary::getID() const{
	return id_;
}
/**
 * set this Questionary's version
 * @param version the version to set
 */
void Questionary::setVersion(int version) {
	version_ = version;
}

/**
 * Save the answers of all this Questionary's questions to a file
 * @filename the name of the file
 */
void Questionary::saveAnswersToFile(std::string filename) {
	std::ofstream file(filename.c_str());
	file << "ID " << getID();
	for(Questionary::Iterator it = begin(); it != end(); ++it) {
		//GroupQuestion doesn't have an answer
		if(dynamic_cast<QuestionList*>(*it) == NULL) {
			file << std::endl << it.getPath().print() << " ";
			//wow, much cast, many beautiful
			AnswerQuestion<std::string>* aqs = dynamic_cast<AnswerQuestion<std::string>*>(*it);
			if(aqs) {
				if(aqs->isAnswered()) file << aqs->getAnswer();
				else file << "NULL";
			}
			AnswerQuestion<int>* aqi = dynamic_cast<AnswerQuestion<int>*>(*it);
			if(aqi) {
				if(aqi->isAnswered()) file << aqi->getAnswer();
				else file << "NULL";
			}
			AnswerQuestion<bool>* aqb = dynamic_cast<AnswerQuestion<bool>*>(*it);
			if(aqb) {
				if(aqb->isAnswered()) file << aqb->getAnswer();
				else file << "NULL";
			}
		}
	}
}
/**
 * Save the Questions to a file
 * @param filename the file where the Questions will be saved
 */
void Questionary::saveQuestionsToFile(std::string filename) {
	std::ofstream file(filename.c_str());
	file << "VERSION " << version_ << std::endl;
	file << "ID " << getID() << std::endl;
	file << "STEPS " << size();
	for(Questionary::Iterator it = begin(); it != end(); ++it) {
		file << std::endl << it.getPath().print() << " ";
		(*it)->save(file);
	}
}
/**
 * laod a question from a file
 * @param file the file containing the Question
 * @return the loaded Question
 */
Question* Questionary::loadQuestion(std::fstream& file) {
	std::string line;
	getline(file, line);
	std::istringstream strm(line);
	std::string word;
	strm >> word; //path
	strm >> word; //question type
	if(word == "BOOL") {
		std::string text;
		getline(strm, text);
		return new BoolQuestion(text.substr(1, std::string::npos));
	}
	else if(word == "CHOICE") {
		return loadChoiceQuestion(file, strm);
	}
	else if(word == "GROUP") {
		return loadGroupQuestion(file, strm);
	}
	else if(word == "SCALE") {
		return loadScaleQuestion(strm);
	}
	else if(word == "TEXT") {
		std::string text;
		getline(strm, text);
		return new OpenQuestion(text.substr(1, std::string::npos));
	} //else error
}
/**
 * load a ChoiceQuestion
 * @param file the file containing the choices
 * @param line the stream containing the ChoiceQuestion
 * @return the loaded ChoiceQuestion
 */
ChoiceQuestion* Questionary::loadChoiceQuestion(std::fstream& file, std::istringstream& line) {
	//get number of choices
	int choicesLeft;
	std::string choices;
	line >> choicesLeft;
	//get text
	std::string text;
	getline(line, text);
	ChoiceQuestion* cq = new ChoiceQuestion(text.substr(1, std::string::npos));
	//get choices
	for(int i = 0; i < choicesLeft; ++i) {
		getline(file, text);
		cq->addChoice(text);
	}

	return cq;
}
/**
 * load a ScaleQuestion
 * @param line the stream containing the ScaleQuestion
 * @return the loaded ScaleQuestion
 */
ScaleQuestion* Questionary::loadScaleQuestion(std::istringstream& line) {
	int  lower, upper;
	//lower bound
	line >> lower;
	//upper bound
	line >> upper;
	//get text
	std::string text;
	getline(line, text);
	return new ScaleQuestion(text.substr(1, std::string::npos), lower, upper);
}
/**
 * load a GroupQuestion
 * @param file the file containing the subquestions
 * @param line the stream containing the GroupQuestion
 * @return the loaded GroupQuestion
 */
GroupQuestion* Questionary::loadGroupQuestion(std::fstream& file, std::istringstream& line) {
	//get nb of subquestions
	int subquestions;
	line >> subquestions;
	//get text
	std::string text;
	getline(line, text);
	GroupQuestion* gq = new GroupQuestion(text.substr(1, std::string::npos));
	//get subquestions
	for(int i = 0; i < subquestions; ++i) {
		gq->add(loadQuestion(file));
	}

	return gq;
}

/**
 * Load a Questionary from a given file
 * @param filename the name of the file
 */
void Questionary::loadQuestionsFromFile(std::string filename) {
	int version(0);
	std::string id("");

	std::fstream file(filename.c_str());

	for(int i = 0; i < 3; ++i) {
		std::string line;
		getline(file, line);
		std::istringstream strm(line);
		std::string word;
		strm >> word;
		if(word == "VERSION") {
			strm >> version;
			version_ = version;
		}
		else if(word == "ID") {
			strm >> id;
			id_ = id;
		}
		else if(word == "STEPS") {
			//do nothing, steps is list size
		}
	}

	//loop over each line until the end of file
	while(!file.eof()) {
		add(loadQuestion(file));
	}
}

