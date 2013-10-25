/*
 * Asker.cpp
 *
 *  Created on: Oct 14, 2013
 *      Author: Mathieu
 */

#include "Asker.h"
#include "Questionary.h"
#include <iostream>

/**
 * Asker will ask the user to fill a Questionary and write the answers to a file
 * @qFileName the file where the Questionary is written
 * @aFileName the file where the answers will be written
 */
Asker::Asker(std::string qFileName, std::string aFileName) : qFileName_(qFileName), aFileName_(aFileName){

}

Asker::~Asker() {
	// TODO Auto-generated destructor stub
}

void Asker::ask() {
	Questionary questionary = loadQuestionsFromFile(qFileName_);
	std::cout << "Deze enquete bestaat uit " << questionary.getSteps() << " vragen :" <<std::endl << std::endl;

	for(int i = 0; i < questionary.getQuestions().size(); ++i){
		std::cout << i+1 << " - ";
		questionary.getQuestions().at(i)->ask();
	}
	std::cout << "Bedankt voor je deelname" << std::endl;
	questionary.saveAnswersToFile(aFileName_);
	std::cout << "Antwoorden werden opgeslagen in " << aFileName_ << std::endl;
}

int main(int argc, char **argv) {
	Asker asker(argv[1], argv[2]);
	asker.ask();
}
