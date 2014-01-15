/*
 * SurveyFiller.h
 *
 *  Created on: Dec 24, 2013
 *      Author: Mathieu
 */

#ifndef SURVEYFILLER_H_
#define SURVEYFILLER_H_

#include"Questionary.h"
#include "QuestionList.h"
#include<Wt/WGroupBox>
#include<Wt/WTreeTable>
#include<Wt/WPushButton>
#include<string>
#include<list>

class SurveyFiller : public Wt::WGroupBox {
public:
	SurveyFiller(std::string title, std::string username);
	virtual ~SurveyFiller();

private:
	void fillQuestionTree();
	void save();

	bool isCompleted(QuestionList& ql);
	void updateAnswered();

	std::string title_;
	std::string user_;
	std::set<Question*> answered_;
	Questionary *questionary_;
	Wt::WTreeTable* questionTree_;
	Wt::WPushButton* save_;
	Wt::WPushButton* back_;
};


#endif /* SURVEYFILLER_H_ */
