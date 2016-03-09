#ifndef CSVPP_H
#define CSVPP_H

#include <string>
#include <vector>
#include <map>

#define VERSION "1.0"


namespace csvpp {

typedef std::map<std::string, std::string> ROW;
typedef std::vector<ROW> rows;
typedef std::map<std::string, std::string>::iterator rowiterator;

class CSV {
	private:
		std::string filedata;
	public:
		CSV() { }
		CSV(std::string file);
		void open(std::string file);
		void set(std::string str);
		rows read();

};

}

#endif
