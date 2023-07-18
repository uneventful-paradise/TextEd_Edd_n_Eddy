#pragma once
#include <exception>

class out_of_bounds : public std::exception
{
	virtual const char* what()const throw() {
		return "Argument is invalid : out of bounds\n";
	}
};
