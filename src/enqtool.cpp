/*
 * main.cpp
 *
 *  Created on: Dec 21, 2013
 *      Author: Mathieu
 */

#include"Asker.h"
#include"Editor.h"
#include <iostream>

void help() {
	std::cout << "Welkom :" << std::endl;
	std::cout << "Om gebruik te maken van de Editor tool, geef argument :" << std::endl;
	std::cout << "   1 : Naam van het vragenbestand" << std::endl;
	std::cout << "Om gebruik te maken van de Asker tool, geef argument :" << std::endl;
	std::cout << "   1 : Naam van het vragenbestand" << std::endl;
	std::cout << "   2 : Naam van het antwoordbestand" << std::endl;
}
/*
int main(int argc, char **argv) {
	if(argc == 2) {
		Editor editor(argv[1]);
		editor.run();
	}
	else if(argc == 3) {
		Asker asker(argv[1], argv[2]);
		asker.ask();
		asker.save();
	}
	else {
		help();
	}
}*/
