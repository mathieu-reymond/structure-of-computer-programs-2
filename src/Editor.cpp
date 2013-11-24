/*
 * Editor.cpp
 *
 *  Created on: Nov 15, 2013
 *      Author: Mathieu
 */

#include "Editor.h"
#include "Question.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <uuid/uuid.h>
#include <typeinfo>

/**
 * An Editor allows to modify or create a Questionary
 */
Editor::Editor(std::string filename) : questionary_("id", 1), filename_(filename), unsavedChanges(false){
	std::ifstream file(filename.c_str());
	//file exists ?
	if(file) {
		questionary_ = Questionary(filename);
	}
	else {
		uuid_t id;
		uuid_generate(id);
		char ch[36];
		memset(ch, 0, 36);
		uuid_unparse(id, ch);
		questionary_ = Questionary(std::string(ch), 1);
	}
}

Editor::~Editor() {
	// TODO Auto-generated destructor stub
}
/**
 * a loop that process instructions and stops after 'quit' is typed
 */
void Editor::run() {
	while(processCommand());
}
/**
 * process a single command
 */
bool Editor::processCommand() {
	bool cont(true);
	std::string command;
	getline(std::cin, command);
	std::istringstream lineStream(command);
	std::string word;
	lineStream >> word;
	std::transform(word.begin(), word.end(), word.begin(), ::tolower);
	if(word == "list") {
		list();
	}
	else if (word == "add" || word=="insert") {
		int position = questionary_.getSteps()+1;
		bool validPos = true;
		if (word == "insert") {
			lineStream >> word;
			std::istringstream strm(word);
			strm >> position;
			validPos = isValidPosition(position);
		}
		if(validPos) {
			lineStream >> word;
			std::transform(word.begin(), word.end(), word.begin(), ::tolower);
			std::string line;
			getline(lineStream, line);
			if(word == "text") {
				insertText(new OpenQuestion(line.substr(1, std::string::npos)), position);
			}
			else if(word == "choice") {
				insertChoice(new ChoiceQuestion(line.substr(1, std::string::npos)), position);
			}
			else {
				std::cout << "Geen geldig type vraag.";
			}
		}
		else {
			std::cout << "Ongeldige invoer, N=" << questionary_.getSteps() << std::endl;
		}

	}
	else if (word == "edit") {
		int pos;
		lineStream >> word;
		std::transform(word.begin(), word.end(), word.begin(), ::tolower);
		bool isChoice = (word == "choice");
		if(isChoice) lineStream >> word;

		std::istringstream strm(word);
		strm >> pos;
		if(isValidPosition(pos)) {
			Questionary::Iterator it = questionary_.begin();
			for(int i = 0; i < pos-1; ++i) {
				++it;
			}

			if(isChoice) {
				editChoices((ChoiceQuestion*) (*it));
			}
			else {
				editQuestion(*it);
			}
		}
		else {
			std::cout << "Ongeldige invoer, N=" << questionary_.getSteps() << std::endl;
		}
	}
	else if (word == "remove") {
		int pos;
		lineStream >> word;
		std::istringstream strm(word);
		strm >> pos;
		if(isValidPosition(pos)) {
			Questionary::Iterator it = questionary_.begin();
			for(int i = 0; i < pos-1; ++i) {
				++it;
			}
			remove(*it);
		}
		else {
			std::cout << "Ongeldige invoer, N=" << questionary_.getSteps() << std::endl;
		}
	}
	else if (word == "save") {
		save();
	}
	else if (word == "quit") {
		quit();
		cont = false;
	}
	else {
		std::cout << "Ongeldig commando." << std::endl;
	}

	return cont;
}

/**
 * Shows a list of all the Questions in this Questionary
 */
void Editor::list() {
	if(questionary_.getSteps() == 0) {
		std::cout << "Deze Enquete bevat nog geen vragen." << std::endl;
	}
	else {
		int i = 1;
		for(Questionary::Iterator it = questionary_.begin(); it != questionary_.end(); ++it) {
			std::cout << i;
			typeid(*(*it)).name() == typeid(OpenQuestion).name() ? std::cout << " TEXT ": std::cout << " CHOICE ";
			std::cout << (*it)->getQuestion() << std::endl;
			i++;
		}
	}
}
/**
 * insert an OpenQuestion at a given position
 * 1 <= position <= N with N=number of Questions
 */
void Editor::insertText(OpenQuestion *question, int position) {
	Questionary::Iterator it = questionary_.begin();
	for(int i = 0; i < position-1; ++i) {
		++it;
	}
	questionary_.insertQuestion(question, it);
	std::cout << "Vraag (" << question->getQuestion() << ") toegevoegd op plaats " << position << std::endl;
	unsavedChanges = true;
}
/**
 * insert a ChoiceQuestion at a given position
 * 1 <= position <= N with N=number of Questions
 */
void Editor::insertChoice(ChoiceQuestion *question, int position) {
	Questionary::Iterator it = questionary_.begin();
	for(int i = 0; i < position-1; ++i) {
		++it;
	}
	if(setChoices(question)) {
		questionary_.insertQuestion(question, it);
		std::cout << "Vraag (" << question->getQuestion() << ") toegevoegd op plaats " << position << std::endl;
		unsavedChanges = true;
	}
}
/**
 * Edit the question text of a given Question
 */
void Editor::editQuestion(Question *question) {
	std::string questionText;
	std::cout << "Nieuwe vraagtext voor Vraag (" << question->getQuestion() << ")" << std::endl;
	getline(std::cin, questionText);
	question->setQuestion(questionText);
	std::cout << "Vraagtext aangepast" << std::endl;
	unsavedChanges = true;
}
/**
 * Edit the choices of a given ChoiceQuestion
 */
void Editor::editChoices(ChoiceQuestion *question) {
	std::cout << "Nieuwe antwoorden voor Vraag (" << question->getQuestion() << ")" << std::endl;
	if(setChoices(question)) {
		std::cout << "Antwoorden aangepast" << std::endl;
		unsavedChanges = true;
	}

}
/**
 * Remove a question
 */
void Editor::remove(Question *question) {
	std::cout << "Vraag (" << question->getQuestion() << ") verwijderd." << std::endl;
	questionary_.removeQuestion(question);
	unsavedChanges = true;
}
/**
 * Save the current Questionary to the file
 */
void Editor::save() {
	questionary_.saveQuestionsToFile(filename_);
	std::cout << "Bestanden bewaard." << std::endl;
	unsavedChanges = false;
}
/**
 * Quit this editor.
 * If there are any unsaved changes, ask to save the Questionary
 */
void Editor::quit() {
	if(unsavedChanges) {
		std::cout << "Er zijn onbewaarde wijzingen. Moeten deze opgeslagen worden? (j/n)" << std::endl;
		std::string response;
		std::cin >> response;
		if(response == "j") save();
	}
	std::cout << "Bye !" << std::endl;
}
/**
 * Set the choices of a ChoiceQuestion to new choices.
 * Only set if there is a valid number of choices
 */
bool Editor::setChoices(ChoiceQuestion *question) {

	std::list<std::string> choices;
	std::string choice;
	getline(std::cin, choice);
	int nbOfChoices = 0;
	while(choice != ".") {
		choices.push_back(choice);
		nbOfChoices++;
		getline(std::cin, choice);
	}
	if(nbOfChoices < 2) {
		std::cout << "Niet genoeg geldige antwoorden." << std::endl;
		return false;
	}
	else {
		question->removeChoices();
		for(std::list<std::string>::iterator it = choices.begin(); it != choices.end(); ++it) {
			question->addChoice(*it);
		}
		return true;
	}
}
/**
 * Checks if  1 <= position <= N with N=number of Questions
 */
bool Editor::isValidPosition(int position) {
	return position >= 1 && position <= questionary_.getSteps();
}

int main(int argc, char **argv) {
	if(argc != 2) {
		std::cout << "ongeldig aantal argumenten : verwacht=1, aantal=" << argc-1 << std::endl;
	}
	else {
		Editor editor(argv[1]);
		editor.run();
	}
}

