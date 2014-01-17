/*
 * SurveySelector.cpp
 *
 *  Created on: Dec 24, 2013
 *      Author: Mathieu
 */

#include "SurveySelector.h"
#include "Wt/WLink"
#include "Wt/WApplication"
#include<dirent.h>

SurveySelector::SurveySelector() {
	//Username
	user_ = new Wt::WLineEdit(this);
	user_->setEmptyText("Enter Username");
	user_->changed().connect(this, &SurveySelector::checkUser);
	//Surveys
	surveys_ = new Wt::WSelectionBox(this);
	fillSurveys();
	surveys_->changed().connect(this, &SurveySelector::checkUser);
	//Begin
	begin_ = new Wt::WPushButton("Begin", this);
	begin_->setDisabled(true);
	begin_->clicked().connect(this, &SurveySelector::startSurvey);
	//begin_->setLink(Wt::WLink(Wt::WLink::InternalPath, "/admin"));

}

SurveySelector::~SurveySelector() {
	//delete user_;
	//delete surveys_;
	//delete begin_;
	//TODO removed in destructor from base class ?
}

void SurveySelector::fillSurveys() {
	DIR *dir;
	struct dirent *ent;
	//"." is current directory
	if ((dir = opendir(".")) != NULL) {
		//all files in directory
		while ((ent = readdir (dir)) != NULL) {
			DIR *subdir;
			struct dirent *subent;
			std::string path("./");
			path.append(ent->d_name);
			if((subdir = opendir(path.c_str())) != NULL) {
				while((subent = readdir (subdir)) != NULL) {
					std::string extension(".ens");
					std::string filename(subent->d_name);
					//has questionary extension ?
					if(filename.length() > 4 && filename.substr(filename.length()-extension.length(), std::string::npos) == extension) {
						//add to table
						surveys_->addItem(ent->d_name);
						break;
					}
				}
				closedir (subdir);
			}
		}
		closedir (dir);
	}
}

void SurveySelector::startSurvey() {
	std::cout << "Hello " << user_->text() << ", please fill in " << surveys_->currentText() << std::endl;
	Wt::WApplication *app = Wt::WApplication::instance();
	std::string path("/fill/");
	path.append(user_->text().toUTF8());
	path.append("/");
	path.append(surveys_->currentText().toUTF8());
	app->setInternalPath(path, true);
}

void SurveySelector::checkUser() {
	if(isValidUsername(user_->text().toUTF8())) {
		begin_->setEnabled(true);
	}
	else {
		begin_->setDisabled(true);
	}
}

bool SurveySelector::isValidUsername(std::string username) {
	std::string space(" ");
	bool valid = username.length() > 0 && username.find(space) >= username.length();
	DIR *dir;
	struct dirent* ent;
	std::string path("./");
	path.append(surveys_->currentText().toUTF8());
	if((dir = opendir(path.c_str())) != NULL) {
		std::string userFilename(username);
		userFilename.append(".ena");
		while((ent = readdir(dir)) != NULL) {
			std::string current(ent->d_name);
			valid &= (userFilename != current);
		}
		closedir(dir);
	}

	return valid;
}
