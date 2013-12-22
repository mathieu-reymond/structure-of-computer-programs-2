/*
 * Editor.cpp
 *
 *  Created on: Nov 15, 2013
 *      Author: Mathieu
 */

#include "Editor.h"
#include "Question.h"
#include "BoolQuestion.h"
#include "ScaleQuestion.h"
#include "QuestionList.h"
#include "GroupQuestion.h"
#include "Path.h"
#include "Asker.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <uuid/uuid.h>
#include <typeinfo>

/**
 * An Editor allows to modify or create a Questionary
 */
Editor::Editor(std::string filename) : questionary_("id", 2), filename_(filename), unsavedChanges(false){
	std::ifstream file(filename.c_str());
	//file exists ?
	if(file) {
		questionary_ = Questionary(filename);
		//check version
		if(questionary_.getVersion() == 1) {
			std::cout << "Deze specificatie zal teruggeschreven worden als versie 2" << std::endl;
			questionary_.setVersion(2);
		}
		else if(questionary_.getVersion() > 2) {
			std::cout << "Deze versie is te hoog" << std::endl;
			throw(questionary_.getVersion());
		}
	}
	else {
		//make new ID
		uuid_t id;
		uuid_generate(id);
		char ch[36];
		memset(ch, 0, 36);
		uuid_unparse(id, ch);
		questionary_ = Questionary(std::string(ch), 2);
	}
}

Editor::~Editor() {
}
/**
 * a loop that process instructions and stops after 'quit' is typed
 */
void Editor::run() {
	while(processCommand());
}
/**
 * process a single command
 * @return true if command != quit, false otherwise
 */
bool Editor::processCommand() {
	bool cont(true);
	std::string command;
	getline(std::cin, command);
	std::stringstream lineStream(command);
	std::string word;
	lineStream >> word;
	std::transform(word.begin(), word.end(), word.begin(), ::tolower);
	if(word == "list") {
		list();
	}
	else if (word == "add" || word=="insert") {
		bool add = word == "add";
		Questionary::Iterator next = questionary_.end();
		try {
			//add type question-text
			std::stringstream stream(lineStream.str());
			stream >> word;
			questionDispatch(stream, next);
		}
		catch(std::invalid_argument& ia) {
			//type was not valid, try to see if it's a path
			Path path;
			lineStream >> path;
			try {
				next = questionary_.iteratorForPath(path);
				//if add, must insert at end of GROUP
				if (add) {
					if (dynamic_cast<QuestionList*>(*next) == NULL) {
						std::cout << "Pad wijst niet naar een GROUP" << std::endl;
					}
					else {
						questionDispatch(lineStream, dynamic_cast<QuestionList*>(*next)->end());
					}
				}
				//insert before next
				else questionDispatch(lineStream, next);
			}
			catch(std::out_of_range& oor) {
				std::cout << "pad out of range" << std::endl;
			}
			catch(std::invalid_argument& ia) {
				std::cout << "verkeerd type" << std::endl;
			}
		}
	}
	else if (word == "edit") {
		lineStream >> word;
		std::transform(word.begin(), word.end(), word.begin(), ::tolower);
		bool isChoice = (word == "choice");
		if(isChoice) lineStream >> word;
		Path path;
		std::istringstream strm(word);
		strm >> path;
		try {
			QuestionList::Iterator it = questionary_.iteratorForPath(path);
			if(isChoice) {
				if(dynamic_cast<ChoiceQuestion*>(*it) != NULL) editChoices((ChoiceQuestion*) (*it));
				else std::cout << "Type CHOICE verwacht" << std::endl;
			}
			else editQuestion(*it);
		}
		catch(std::out_of_range& ia) {
			std::cout << "Geen geldig pad" << std::endl;
		}
	}
	else if (word == "group") {
		try {
			//lower path
			Path lower;
			lineStream >> lower;
			//upper path
			Path upper;
			lineStream >> upper;
			if(lower.isOnSameLevel(upper)) {
				QuestionList::Iterator first = questionary_.iteratorForPath(lower);
				QuestionList::Iterator last = questionary_.iteratorForPath(upper);
				//where GROUP will be inserted, set before first,
				//because the Questions between first an last will be deleted,
				//first and last will become invalid
				QuestionList::Iterator next = first -1;
				std::string line;
				getline(lineStream, line);
				GroupQuestion* gq = new GroupQuestion(line.substr(1, std::string::npos));
				std::list<Question*> toRemove;
				//put all Questions between first an last in a GroupQuestion
				for(QuestionList::Iterator it = first; it != last; it.levelForward()) {
					gq->add((*it)->copy());
					toRemove.push_back(*it);
				}
				//last is included
				gq->add((*last)->copy());
				toRemove.push_back(*last);
				//remove those Questions from the Questionary
				for(std::list<Question*>::iterator it = toRemove.begin(); it != toRemove.end(); ++it) {
					questionary_.remove(*it);
				}
				//update next
				++next;
				//if next went a level up, it was at the end of a group,
				//insert at end of that group
				if(next.getPath().size() < last.getPath().size()) {
					Path p(next.getPath());
					int pos(p.back()-1);
					p.pop_back();
					p.push_back(pos);
					next = questionary_.iteratorForPath(p);
					next = dynamic_cast<QuestionList*>(*next)->end();
					std::cout << "insert at end of group" << std::endl;
				}
				insert(gq, next);
				std::cout << "grouped" << std::endl;
			}
			else {
				std::cout << "paden niet in dezelfde groep of op top level" << std::endl;
			}
		}
		catch(std::out_of_range& oor) {
			std::cout << "Geen geldig pad" << std::endl;
		}
	}
	else if (word == "ungroup") {
		try {
			Path path;
			lineStream >> path;
			//get GroupQuestion at path
			QuestionList::Iterator it = questionary_.iteratorForPath(path);
			QuestionList* ql = dynamic_cast<QuestionList*>(*it);
			if(ql) {
				//add all question from GroupQuestion to Questionary
				for(QuestionList::Iterator i = ql->begin(); i != ql->end(); i.levelForward()) {
					questionary_.insert((*i)->copy(), it);
				}
				//remove GroupQuestion
				remove(*it);
				std::cout << "ungrouped" << std::endl;
			}
			else {
				std::cout << "Pad wijst niet naar een GROUP" << std::endl;
			}
		}
		catch(std::out_of_range& oor) {
			std::cout << "Geen geldig pad" << std::endl;
		}
	}
	else if (word == "remove") {
		Path path;
		lineStream >> path;
		try {
			Questionary::Iterator it = questionary_.iteratorForPath(path);
			remove(*it);
		}
		catch(std::out_of_range& oor) {
			std::cout << "Geen geldig pad" << std::endl;
		}
	}
	else if (word == "test") {
		test();
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
	if(questionary_.size() == 0) {
		std::cout << "Deze Enquete bevat nog geen vragen." << std::endl;
	}
	else {
		for(Questionary::Iterator it = questionary_.begin(); it != questionary_.end(); ++it) {
			for(int i = 1; i < it.getPath().size(); ++i) {
				std::cout << "  ";
			}
			std::cout << it.getPath().print() << " ";
			std::cout << (**it) << std::endl;
		}
	}
}
/**
 * Insert the right type of Question before next
 * @throw std::invalid_argument the type does not correspond to a Question
 * @param stream the stream containing the Question
 * @param next the iterator where the new Question will be inserted
 */
void Editor::questionDispatch(std::stringstream& stream, QuestionList:: Iterator next) {
	std::string word;
	stream >> word;
	std::transform(word.begin(), word.end(), word.begin(), ::tolower);
	std::string line;
	if(word == "text") {
		getline(stream, line);
		insert(new OpenQuestion(line.substr(1, std::string::npos)), next);
	}
	else if(word == "bool") {
		getline(stream, line);
		insert(new BoolQuestion(line.substr(1, std::string::npos)), next);
	}
	else if(word == "choice") {
		getline(stream, line);
		insertChoice(new ChoiceQuestion(line.substr(1, std::string::npos)), next);
	}
	else if(word == "scale") {
		getline(stream, line);
		insertScale(line.substr(1, std::string::npos), next);
	}
	else {
		throw std::invalid_argument("geen geldig type vraag");
	}
}
/**
 * Insert a Question
 */
void Editor::insert(Question* question, QuestionList::Iterator it) {
	questionary_.insert(question, it);
	std::cout << "Vraag (" << question->getQuestion() << ") toegevoegd op plaats " << it.getPath().print() << std::endl;
	unsavedChanges = true;
}

/**
 * insert a ChoiceQuestion
 */
void Editor::insertChoice(ChoiceQuestion *question, QuestionList::Iterator it) {
	if(setChoices(question)) {
		questionary_.insert(question, it);
		std::cout << "Vraag (" << question->getQuestion() << ") toegevoegd op plaats " << it.getPath().print() << std::endl;
		unsavedChanges = true;
	}
}
/**
 * insert a ScaleQuestion
 */
void Editor::insertScale(std::string question, QuestionList::Iterator it) {
	int lower, upper;
	std::string s;
	std::cout << "Lower bound : ";
	getline(std::cin, s);
	std::istringstream lstrm(s);
	lstrm >> lower;
	std::cout << "Upper bound : ";
	getline(std::cin, s);
	std::istringstream ustrm(s);
	ustrm >> upper;
	if(lower < upper) {
		questionary_.insert(new ScaleQuestion(question, lower, upper), it);
		std::cout << "Vraag (" << question<< ") toegevoegd op plaats " << it.getPath().print() << std::endl;
		unsavedChanges = true;
	}
	else {
		std::cout << "Ongeldige lower en upper bound." << std::endl;
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
	questionary_.remove(question);
	unsavedChanges = true;
}
/**
 * Runs an Asker with this Questionary
 * Does not save any answers written in this Asker
 */
void Editor::test() {
	Asker asker(questionary_);
	asker.ask();
	std::cout << "Terug in Editor tool :" << std::endl;
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
	return position >= 1 && position <= questionary_.size();
}

