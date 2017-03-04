#include <fstream>
#include <string>
#include <iostream>
#include <cstring>

class PPM
{
	std::string magicNum;
	int width, height, maxval;
	char * pixmap;
	
	public:
		unsigned char * readFile(std::string, int&, int&);
		void writeFile(char *, int, int, std::string);
};

