/*
 * AnswersResource.cpp
 *
 *  Created on: Dec 31, 2013
 *      Author: Mathieu
 */

#include "AnswersResource.h"
#include "Questionary.h"
#include "Path.h"
#include <Wt/Json/Serializer>
#include <dirent.h>
#include <fstream>

AnswersResource::AnswersResource(std::string title) : Wt::WResource(), title_(title) {
	std::string filename(title);
	filename.append("-answers.txt");
	suggestFileName(filename);
	getAnswers();
}

AnswersResource::~AnswersResource() {
	// TODO Auto-generated destructor stub
}

void AnswersResource::handleRequest(const Wt::Http::Request &request, Wt::Http::Response &response) {
	response.setMimeType("plain/text");
	response.out() << Wt::Json::serialize(answers_) << std::endl;
}

void AnswersResource::getAnswers() {
	DIR* dir;
	struct dirent *ent;
	std::string folder("./");
	folder.append(title_);
	Wt::Json::Array answers;
	if((dir = opendir(folder.c_str())) != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			std::string file(folder);
			file.append("/");
			file.append(ent->d_name);
			std::string extension(".ena");
			if(file.length() > 4) std::cout << file.substr(file.length() - extension.length(), std::string::npos) << std::endl;
			if(file.length() > 4 && file.substr(file.length() - extension.length(), std::string::npos) == extension) {
				answers.push_back(AnswersForFilename(file));
			}
		}
	}

	answers_ = answers;
}
const Wt::Json::Value AnswersResource::AnswersForFilename(std::string filename) {
	std::string questions("./");
	questions.append(title_);
	questions.append("/questions.ens");
	Questionary questionary(questions);

	std::fstream file(filename.c_str());
	std::string line;
	//first line is ID
	getline(file, line);
	//json array
	Wt::Json::Value v(Wt::Json::ArrayType);
	Wt::Json::Array& array = v;
	//answers
	while(getline(file, line)) {
		std::istringstream strm(line);
		Path path;
		strm >> path;
		getline(strm, line); //line now contains answer
		//get question of answer to know type of answer
		if(line == " NULL") {
			Wt::Json::Value json; //NULL value
			array.push_back(json);
		}
		else {
			Questionary::Iterator it = questionary.iteratorForPath(path);
			AnswerQuestion<std::string>* s = dynamic_cast<AnswerQuestion<std::string>*>(*it);
			if(s) {
				Wt::Json::Value json(line);
				array.push_back(json);
			}
			AnswerQuestion<int>* i = dynamic_cast<AnswerQuestion<int>*>(*it);
			if(i) {
				int answer;
				std::istringstream astrm(line);
				astrm >> answer;
				Wt::Json::Value json(answer);
				array.push_back(json);
			}
			AnswerQuestion<bool>* b = dynamic_cast<AnswerQuestion<bool>*>(*it);
			if(b) {
				bool answer;
				std::istringstream astrm(line);
				astrm >> answer;
				Wt::Json::Value json(answer);
				array.push_back(json);
			}
		}
	}

	return v;
}
