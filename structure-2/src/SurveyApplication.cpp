/*
 * SurveyApplication.cpp
 *
 *  Created on: Dec 28, 2013
 *      Author: Mathieu
 */

#include "SurveyApplication.h"
#include "SurveySelector.h"
#include "SurveyFiller.h"
#include "SurveyAdmin.h"
#include <iostream>

SurveyApplication::SurveyApplication(const Wt::WEnvironment& env) : WApplication(env) {
	//root()->addWidget(new SurveySelector());
	this->internalPathChanged().connect(this, &SurveyApplication::handlePathChange);
	//setInternalPath("/select", true);
	handlePathChange();
}

SurveyApplication::~SurveyApplication() {
	// TODO Auto-generated destructor stub
}

void SurveyApplication::handlePathChange() {
	std::cout << "path : " << internalPath() << std::endl;
	if(internalPath() == "/") {
		setInternalPath("/select", true);
	}
	if(internalPath() == "/select") {
		root()->clear();
		root()->addWidget(new SurveySelector());
	}
	if(internalPath() == "/admin") {
		root()->clear();
		root()->addWidget(new SurveyAdmin());
	}
	if(internalPath().length() > 5 && internalPath().substr(0, 5) == "/fill") {
		std::cout << "filling" << std::endl;
		std::string splitted;
			std::string path(internalPath().substr(6, std::string::npos));
			std::stringstream stream(path);
			//username
			std::getline(stream, splitted, '/');
			std::string user(splitted);
			//survey name
			std::getline(stream, splitted, '/');
			std::string title(splitted);
			root()->clear();
			root()->addWidget(new SurveyFiller(title, user));

	}
}
