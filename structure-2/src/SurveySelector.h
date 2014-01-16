/*
 * SurveySelector.h
 *
 *  Created on: Dec 24, 2013
 *      Author: Mathieu
 */

#ifndef SURVEYSELECTOR_H_
#define SURVEYSELECTOR_H_

#include <Wt/WGroupBox>
#include <Wt/WLineEdit>
#include <Wt/WSelectionBox>
#include <Wt/WPushButton>

class SurveySelector : public Wt::WGroupBox {
public:
	SurveySelector();
	virtual ~SurveySelector();

private:
	void fillSurveys();
	void startSurvey();
	void checkUser();
	bool isValidUsername(std::string username);

	Wt::WSelectionBox* surveys_;
	Wt::WLineEdit* user_;
	Wt::WPushButton* begin_;
};

#endif /* SURVEYSELECTOR_H_ */
