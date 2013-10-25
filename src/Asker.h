/*
 * Asker.h
 *
 *  Created on: Oct 14, 2013
 *      Author: Mathieu
 */

#ifndef ASKER_H_
#define ASKER_H_

#include<string>

class Asker {
public:
	Asker(std::string qFileName, std::string aFileName);
	virtual ~Asker();

	void ask();
private:
	std::string qFileName_; //where questionary is written
	std::string aFileName_; //where answers are written
};

#endif /* ASKER_H_ */
