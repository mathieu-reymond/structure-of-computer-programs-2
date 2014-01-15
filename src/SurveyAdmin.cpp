/*
 * SurveyAdmin.cpp
 *
 *  Created on: Dec 27, 2013
 *      Author: Mathieu
 */

#include "SurveyAdmin.h"
#include "Wt/WAnchor"
#include "Wt/WLink"
#include "Wt/WObject"
#include <string>
#include <dirent.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <vector>
#include <fstream>
#include <boost/bind.hpp>

SurveyAdmin::SurveyAdmin() {
	table_ = new Wt::WTable(this);
	fillTable();
	file_ = new Wt::WFileUpload(this);
	upload_ = new Wt::WPushButton("Upload", this);
	title_ = new Wt::WLineEdit(this);

	file_->uploaded().connect(this, &SurveyAdmin::upload);
	upload_->clicked().connect(file_, &Wt::WFileUpload::upload);
	title_->setEmptyText("Survey Title");

}

SurveyAdmin::~SurveyAdmin() {
	// TODO Auto-generated destructor stub
}

void SurveyAdmin::refresh() {
	Wt::WGroupBox::refresh();
	fillTable();
}

void SurveyAdmin::remove(std::string title) {
	DIR* dir;
	struct dirent *ent;
	std::string folder("./");
	folder.append(title);
	if((dir = opendir(folder.c_str())) != NULL) {
		//remove folder content
		while((ent = readdir (dir)) != NULL) {
			std::string file(folder);
			file.append("/");
			file.append(ent->d_name);
			std::remove(file.c_str());
		}
		closedir(dir);
		//remove folder
		rmdir(folder.c_str());
	}

	refresh();
}


void downloadAnswers(std::string title) {

}

void SurveyAdmin::fillTable() {
	table_->clear();
	DIR *dir;
	struct dirent *ent;
	//"." is current directory
	if ((dir = opendir(".")) != NULL) {
		//all files in directory
		int row(0);
		while ((ent = readdir (dir)) != NULL) {
			DIR *subdir;
			struct dirent *subent;
			std::string path("./");
			path.append(ent->d_name);
			if((subdir = opendir(path.c_str())) != NULL) {
				while((subent = readdir(subdir)) != NULL) {
					std::string extension(".ens");
					std::string filename(subent->d_name);
					//has questionary extension ?
					if(filename.length() > 4 && filename.substr(filename.length()-extension.length(), std::string::npos) == extension) {
						//add to table
						std::string filepath(path);
						filepath.append("/");
						filepath.append(filename);
						Wt::WAnchor* anchor = new Wt::WAnchor(Wt::WLink(filepath), ent->d_name);
						table_->elementAt(row,0)->addWidget(anchor);
						//download button
						Wt::WPushButton* download = new Wt::WPushButton("Download");
						AnswersResource* answers = new AnswersResource(std::string(ent->d_name));
						//link answers to button
						download->setLink(Wt::WLink(answers));
						table_->elementAt(row, 1)->addWidget(download);
						//remove button
						Wt::WPushButton* remove = new Wt::WPushButton("delete");
						remove->clicked().connect(boost::bind(&SurveyAdmin::remove, this, std::string(ent->d_name)));
						table_->elementAt(row, 2)->addWidget(remove);
						row++;
					}
				}
				closedir (subdir);
			}
			else std::cout << "could not open subdir : "<< path << std::endl;
		}
		closedir (dir);
	}
}

void SurveyAdmin::upload() {
	//copy paste in desired location
	std::string destPath("./");
	destPath.append(title_->text().toUTF8());
	//create dir
	const std::string dirPath(destPath);
	umask(000);
	mkdir(dirPath.c_str(), 0777);
	//copy file in dir
	destPath.append("/questions.ens");
	std::ifstream src(file_->spoolFileName().c_str(), std::ios::binary);
	std::ofstream dst(destPath.c_str(), std::ios::binary);
	dst << src.rdbuf();
	//refresh page
	refresh();
}
