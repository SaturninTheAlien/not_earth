#include "utils.h"

#include <string>
#include <fstream>
#include <sstream>
#include <GL/glew.h>

std::string readFile(const std::string&filename){
	std::ifstream input_file(filename, std::ios::in);
	if(!input_file.good()){
        throw FileNotFoundException(filename);
	}

	std::ostringstream os;
	os << input_file.rdbuf();
	return os.str();
}