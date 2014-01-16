/*
 * Question.h
 *
 *  Created on: Oct 16, 2013
 *      Author: Mathieu
 */

#ifndef ANSWERQUESTION_H_
#define ANSWERQUESTION_H_

#include "Wt/WTreeTableNode"
#include "Question.h"
#include <string>
#include <iostream>

template<typename T>
class AnswerQuestion : public Question {
public:
	AnswerQuestion(std::string question);
	virtual ~AnswerQuestion();

	T getAnswer() const;
	virtual void setAnswer(T answer);
	virtual bool isAnswered() const;

	virtual void ask() = 0;
	virtual std::ostream& save(std::ostream& out);

	virtual AnswerQuestion<T>* copy() = 0;

	friend std::ostream& operator<<(std::ostream& out, const AnswerQuestion<T>& question) {
		return out << question.print();
	}

protected:
	virtual T convertString(std::string) const = 0;
	virtual std::string print() const;
	virtual bool isValidAnswer(std::string a) const = 0;
	std::string input();

	T answer_;
	bool answered_;
};

/**
 * A Question class containing a question and an answer
 */
template<typename T>
AnswerQuestion<T>::AnswerQuestion(std::string question) : Question(question){
	//answer_ = void;
	answered_ = false;
}

template<typename T>
AnswerQuestion<T>::~AnswerQuestion() {
}

/**
 * Get the answer to the Question
 * @return the answer
 */
template<typename T>
T AnswerQuestion<T>::getAnswer() const {
	return answer_;
}

/**
 * Set the answer to new given answer
 * @param answer the new answer
 */
template<typename T>
void AnswerQuestion<T>::setAnswer(T answer) {
	answered_ = true;
	answer_ = answer;

	std::cout << "answer changed to : " << answer << std::endl;
}

template<typename T>
bool AnswerQuestion<T>::isAnswered() const{
	return answered_;
}

/**
 * save this Question
 * @param out the stream where this Question will be saved
 * @return the modified stream
 */
template<typename T>
std::ostream& AnswerQuestion<T>::save(std::ostream& out) {
	return operator<<(out, *this);
}
/**
 * Convert this Question to a string
 * @return a string representing this Question
 */
template<typename T>
std::string AnswerQuestion<T>::print() const {
	return getQuestion();
}

/**
 * Get input from the user
 * @throw std::string the string if it begins with ':'
 * @return a string qith the user input
 */
template<typename T>
std::string AnswerQuestion<T>::input() {
	std::string s;
	getline(std::cin, s);
	if(s.at(0) == ':') throw(s);
	else return s;
}


#endif /* ANSWERQUESTION_H_ */
