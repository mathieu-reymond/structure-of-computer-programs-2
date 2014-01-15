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
#include "Wt/WTree"
#include "Wt/WTreeTableNode"

#include "Question.h"


SurveyFiller::SurveyFiller(std::string title, std::string username) : title_(title),user_(username) {
	std::string filename("./");
	filename.append(title);
	filename.append("/questions.ens");
	std::cout << "filename : " << filename << std::endl;
	//Questionary
	questionary_ = new Questionary(filename);
	//Title
	new Wt::WText(title_, this);
	//Tree
	questionTree_ = new Wt::WTreeTable(this);
	fillQuestionTree();
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
	updateAnswered();
	//if(isCompleted(*questionary_)) {
		// "./title/user.ena"
		std::string file("./");
		file.append(title_);
		file.append("/");
		file.append(user_);
		file.append(".ena");
		questionary_->saveAnswersToFile(file);
	//}
}

void SurveyFiller::fillQuestionTree() {
	//one column for answers
	questionTree_->tree()->setSelectionMode(Wt::ExtendedSelection);
	questionTree_->addColumn("Answers", 120);

	//rootnode
	Wt::WTreeTableNode* root = new Wt::WTreeTableNode("Survey");
	questionTree_->setTreeRoot(root, "Questions");
	//add questions
	for(Questionary::Iterator it = questionary_->begin(); it != questionary_->end(); it.levelForward()) {
		root->addChildNode((*it)->widget());
	}
	root->expand();

	//add optional to answered
	for(Questionary::Iterator it = questionary_->begin(); it != questionary_->end(); ++it) {
		if((*it)->isOptional()) answered_.insert(*it);
	}
	updateAnswered();
}

/**
 * Checks if all the Questions of ql where answered
 * @param ql the QuestionList to check
 * @return true if completed, false otherwise
 */
bool SurveyFiller::isCompleted(QuestionList& ql) {
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
void SurveyFiller::updateAnswered() {
	int size = answered_.size();
	for(QuestionList::Iterator it = questionary_->begin(); it != questionary_->end(); ++it) {
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
