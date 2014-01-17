/*
 * project.cpp
 *
 *  Created on: Dec 24, 2013
 *      Author: Mathieu
 */

#include <Wt/WApplication>
#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WText>
#include "Wt/WServer"
#include "SurveySelector.h"
#include "SurveyFiller.h"
#include "SurveyAdmin.h"
#include "SurveyApplication.h"
#include "AnswersResource.h"

class HelloApplication : public Wt::WApplication
{
public:
	HelloApplication(const Wt::WEnvironment& env) :
		WApplication(env)
{
		root()->addWidget(new Wt::WText("Hello World"));
		//root()->addWidget(new SurveySelector());
		//root()->addWidget(new SurveyFiller("questions.txt"));
		root()->addWidget(new SurveyAdmin());
}
};

Wt::WApplication *createApplication(const Wt::WEnvironment& env)
{
	//return new HelloApplication(env);
	return new SurveyApplication(env);
}

int main(int argc, char *argv[]) {
	const char *custom_argv[] = {
			"hello",
			"--docroot=/usr/share/Wt/resources/",
			"--http-address=127.0.0.1",
			"--http-port=8080"
	};

	AnswersResource res("surv");
	return WRun(4, (char **)(custom_argv), &createApplication);
}
