/*
 * SurveyApplication.h
 *
 *  Created on: Dec 28, 2013
 *      Author: Mathieu
 */

#ifndef SURVEYAPPLICATION_H_
#define SURVEYAPPLICATION_H_

#include "Wt/WApplication"

class SurveyApplication : public Wt::WApplication {
public:
	SurveyApplication(const Wt::WEnvironment& env);
	virtual ~SurveyApplication();

private:
	void handlePathChange();
};

#endif /* SURVEYAPPLICATION_H_ */
