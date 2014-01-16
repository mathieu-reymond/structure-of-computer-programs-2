/*
 * SurveyAdmin.h
 *
 *  Created on: Dec 27, 2013
 *      Author: Mathieu
 */

#ifndef SURVEYADMIN_H_
#define SURVEYADMIN_H_

#include "Wt/WGroupBox"
#include "Wt/WTable"
#include "Wt/WPushButton"
#include "Wt/WFileUpload"
#include "Wt/WLineEdit"
#include "AnswersResource.h"
#include <string>

class SurveyAdmin : public Wt::WGroupBox {
public:
	SurveyAdmin();
	virtual ~SurveyAdmin();
	virtual void refresh();

	void remove(std::string title);
	void downloadAnswers(std::string title);

private:
	void fillTable();
	void upload();

	Wt::WTable* table_;
	Wt::WPushButton* upload_;
	Wt::WFileUpload* file_;
	Wt::WLineEdit* title_;
};

#endif /* SURVEYADMIN_H_ */
