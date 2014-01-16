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
#include<Wt/WTable>
#include<Wt/WPushButton>
#include<string>
#include<list>

class SurveyFiller : public Wt::WGroupBox {
public:
	SurveyFiller(std::string title, std::string username);
	virtual ~SurveyFiller();

private:
	void fillQuestionTable();
	void save();

	bool isCompleted(QuestionList& ql);

	std::string title_;
	std::string user_;
	Questionary *questionary_;
	Wt::WTable* questionTable;
	Wt::WPushButton* save_;
	Wt::WPushButton* back_;
};


#endif /* SURVEYFILLER_H_ */
