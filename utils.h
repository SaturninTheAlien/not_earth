#pragma once

#include <string>
#include <sstream>
#include <exception>

class FileNotFoundException:public std::exception{
public:
    FileNotFoundException(const std::string& filename):
    filename(filename){
        std::ostringstream os;
        os<<"File not found exception: \""<<filename<<"\"";
        this->msg = os.str();
    }

    virtual const char* what() const noexcept{
        return msg.c_str();
    }
    std::string filename;
private:
    std::string msg;
};

std::string readFile(const std::string&filename);