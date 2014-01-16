/*
 * SurveyFiller.cpp
 *
 *  Created on: Dec 24, 2013
 *      Author: Mathieu
 */

#include "SurveyFiller.h"

#include <iostream>
#include <map>
#include <set>

#include "Wt/WGlobal"
#include "Wt/WLink"
#include "Wt/WSignal"
#include "Wt/WText"
#include "Wt/WCssDecorationStyle"
#include "Wt/WColor"
#include "Wt/WLineEdit"
#include "Wt/WMessageBox"

#include "Question.h"

#include <functional>


SurveyFiller::SurveyFiller(std::string title, std::string username) : title_(title),user_(username) {
	std::string filename("./");
	filename.append(title);
	filename.append("/questions.ens");
	std::cout << "filename : " << filename << std::endl;
	//Questionary
	questionary_ = new Questionary(filename);
	for(Questionary::Iterator it = questionary_->begin(); it != questionary_->end(); ++it) {
		std::cout << it.getPath().print() << std::endl;
		//std::cout << " #opt:" << (*it)->isOptional();
		//std::cout << " Q:" << (*it)->getQuestion() << std::endl;
	}
	std::cout << "printed" << std::endl;
	//Title
	new Wt::WText(title_, this);
	//Tree
	questionTable = new Wt::WTable(this);
	fillQuestionTable();
	//Buttons
	save_ = new Wt::WPushButton("Save", this);
	save_->clicked().connect(this, &SurveyFiller::save);
	back_ = new Wt::WPushButton("Back", this);
	back_->setLink(Wt::WLink(Wt::WLink::InternalPath, "/select"));
}

SurveyFiller::~SurveyFiller() {
	delete questionary_;
}

void SurveyFiller::save() {
	if(isCompleted(*questionary_)) {
		// "./title/user.ena"
		std::string file("./");
		file.append(title_);
		file.append("/");
		file.append(user_);
		file.append(".ena");
		questionary_->saveAnswersToFile(file);
		Wt::WMessageBox* message = new Wt::WMessageBox("Status",
				"Answers saved.",
				Wt::Information,
				Wt::Ok);
		message->buttonClicked().connect(std::bind([=] () {
			delete message;
		}));
		message->show();
	}
	else {
		Wt::WMessageBox* message = new Wt::WMessageBox("Warning",
				"Answer every question.",
				Wt::Warning,
				Wt::Ok);
		message->buttonClicked().connect(std::bind([=] () {
			delete message;
		}));
		message->show();
	}
}

void SurveyFiller::fillQuestionTable() {
	//default columns
	questionTable->elementAt(0, 0)->addWidget(new Wt::WText("Path"));
	questionTable->elementAt(0, 1)->addWidget(new Wt::WText("Question"));
	questionTable->elementAt(0, 2)->addWidget(new Wt::WText("Answer"));

	int i(1);
	//add questions
	for(Questionary::Iterator it = questionary_->begin(); it != questionary_->end(); ++it) {
		questionTable->elementAt(i, 0)->addWidget(new Wt::WText(it.getPath().print()));
		Wt::WCssDecorationStyle style;
		style.setForegroundColor(Wt::WColor(0,255,0));
		Wt::WText* question = new Wt::WText((*it)->getQuestion());
		if((*it)->isOptional())  question->setDecorationStyle(style);
		questionTable->elementAt(i, 1)->addWidget(question);
		questionTable->elementAt(i, 2)->addWidget((*it)->widget());

		++i;
	}
}

/**
 * Checks if all the Questions of ql where answered
 * @param ql the QuestionList to check
 * @return true if completed, false otherwise
 */
bool SurveyFiller::isCompleted(QuestionList& ql) {
	for(QuestionList::Iterator it = ql.begin(); it != ql.end(); it.levelForward()) {
		if(dynamic_cast<GroupQuestion*>(*it) == NULL) {
			if(!(*it)->isOptional()) {
				AnswerQuestion<int>* i = dynamic_cast<AnswerQuestion<int>* >(*it);
				AnswerQuestion<bool>* b = dynamic_cast<AnswerQuestion<bool>* >(*it);
				AnswerQuestion<std::string>* s = dynamic_cast<AnswerQuestion<std::string>* >(*it);
				if (i != NULL && !i->isAnswered()) return false;
				if (b != NULL && !b->isAnswered()) return false;
				if (s != NULL && !s->isAnswered()) return false;
			}
		}
	}

	return true;
}
