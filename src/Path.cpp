/*
 * Path.cpp
 *
 *  Created on: Dec 6, 2013
 *      Author: Mathieu
 */

#include "Path.h"
#include <iostream>
#include <sstream>
#include <string>
#include <sstream>
/**
 * Makes a new, empty Path
 */
Path::Path() {

}

Path::~Path() {
}
/**
 * Checks if this Path is on the same level as path.
 * The Path must be the same except the last value.
 * @param path the compared path
 * @return true if on same level, false otherwise
 */
bool Path::isOnSameLevel(const Path& path) const {
	if(path.size() != size()) return false;
	else {
		const_iterator it = begin();
		const_iterator pathIt = path.begin();
		bool result = true;
		for(int i = 1; i < size(); ++i) {
			result = result && (*(it++) == *(pathIt++));
		}
		return result;
	}
}
/**
 * Makes a string representation of this Path
 * @return the string representation
 */
std::string Path::print() {
	if(!empty()) {
		const_iterator it = begin();
		std::stringstream strm;
		strm << (*it);
		++it;
		for(const_iterator i = it; i !=  end(); ++i) {
			strm << ".";
			strm << (*i);
		}

		return strm.str();
	}
	else return "";
}
/**
 * Convert what's in the input stream to a Path
 * @param in the input stream
 * @param p the converted Path
 * @return the modified stream
 */
std::istream& operator>>(std::istream& in, Path& p) {
	std::string nb;
	std::string word;
	in >> word;
	std::stringstream stream(word);
	while(std::getline(stream, nb, '.')) {
		int num;
		std::stringstream s(nb);
		s >> num;
		p.push_back(num);
	}

	return in;
}

