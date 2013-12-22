/*
 * Path.h
 *
 *  Created on: Dec 6, 2013
 *      Author: Mathieu
 */

#ifndef PATH_H_
#define PATH_H_

#include <deque>
#include <iostream>

class Path : public std::deque<int>{
public:
	Path();
	virtual ~Path();

	bool isOnSameLevel(const Path& path) const;
	std::string print();

	friend std::istream& operator>>(std::istream& in, Path& p);
};

#endif /* PATH_H_ */
