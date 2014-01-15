/*
 * AnswersResource.h
 *
 *  Created on: Dec 31, 2013
 *      Author: Mathieu
 */

#ifndef ANSWERSRESOURCE_H_
#define ANSWERSRESOURCE_H_

#include"Wt/WResource"
#include"Wt/Json/Array"
#include"Wt/Json/Value"
#include"Wt/Http/Request"
#include"Wt/Http/Response"
#include<string>

class AnswersResource : public Wt::WResource {
public:
	AnswersResource(std::string title);
	virtual ~AnswersResource();

	void handleRequest(const Wt::Http::Request &request, Wt::Http::Response &response);
	Wt::Json::Array answers_;
private:
	void getAnswers();
	const Wt::Json::Value AnswersForFilename(std::string filename);
	std::string title_;

};

#endif /* ANSWERSRESOURCE_H_ */
