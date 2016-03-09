#include <string>
#include <iostream>
#include <fstream>
#include "csvpp.h"
#include "stringhelper.h"

using namespace std;
using namespace csvpp;

CSV::CSV(string file)
{
	this->open(file);
}

void CSV::open(string file)
{
	string line;
	this->filedata = "";
	ifstream csvfile(file.c_str());
    if (csvfile.is_open())
    {
        while (! csvfile.eof() )
        {
            getline(csvfile, line);
			//if (line.find("\r") != string::npos)
			//	line.substr(0, line.find("\r"));
			//if (line.find("\n") != string::npos)
			//	line.substr(0, line.find("\n"));
			this->filedata += line + "\n";
        }
        csvfile.close();
    }
}

void CSV::set(string str)
{
	this->filedata = str;
}

rows CSV::read()
{
	rows retrows;
	ROW tmprow;
	string element;
	vector<string> splitrows;
	string headermap;
	vector<string> headermap_arr;
	string subheadermap;
//	bool read = true;
//	size_t foundcomma = 1;
//	size_t nextcomma = 0;
//	size_t lastfoundcomma = 0;
	//size_t foundquote;
	/* Attempt to discover terminating character */
	string newlinechar;
	if ( this->filedata.at(this->filedata.size() - 2) == '\r' )
		newlinechar = "\r\n";
	else
		newlinechar = "\n";
	
	splitrows = split(this->filedata, newlinechar);
	headermap = splitrows.at(0);
	headermap_arr = split(headermap, ",");
	
	for (vector<string>::iterator i=headermap_arr.begin(), e=headermap_arr.end(); i!=e; ++i)
	{
		if (i->size() >= 2 && i->at(0) == '"' && i->at(i->size()-1) == '"')
			*i = i->substr(1, i->size()-2);
	}
	
	//size_t newlinepos;
	//size_t newlinepos2;
	//size_t quotepos;
	if ( newlinechar == "\r\n" )
		subheadermap = this->filedata.substr(headermap.length() + 2, this->filedata.length() - headermap.length());
	else
		subheadermap = this->filedata.substr(headermap.length() + 1, this->filedata.length() - headermap.length());
	int numelements = headermap_arr.size();
	int currele = 0;
	int pointer = 0;
	int nextpointer = 0;
	while ( pointer+1 <= (signed int)subheadermap.size() )
	{
		if ( currele == numelements )
		{
			currele = 0;
			retrows.push_back(tmprow);
			tmprow.clear();
		}
		if ( subheadermap.at(pointer) == '"' )
		{
			nextpointer = subheadermap.find("\",", pointer);
			if ( nextpointer == -1 )
				nextpointer = subheadermap.find("\"\n", pointer);
			element = subheadermap.substr(pointer+1, nextpointer-pointer-1);
			if ( element.find("\n") != -1 && element.find(",") == -1)
			{
				nextpointer = subheadermap.find("\n", pointer);
				element = subheadermap.substr(pointer, nextpointer-pointer);
				if ( element.size() > 0 && element.at(element.size()-1) == '\r' )
					element.erase(element.size()-1);
			}
			tmprow[headermap_arr.at(currele)] = element;
			if ( nextpointer != -1 )
				pointer = nextpointer + 2;
			else
				pointer += 2;
			//if ( subheadermap.at(pointer) == ',' )
			//	pointer += 2;
			++currele;
		} else {
			nextpointer = subheadermap.find(",", pointer);
			if ( nextpointer == -1 )
			{
				nextpointer = subheadermap.find("\n", pointer);
			}
			element = subheadermap.substr(pointer, nextpointer-pointer);
			if ( element.find("\n") != -1 )
			{
				nextpointer = subheadermap.find("\n", pointer);
				element = subheadermap.substr(pointer, nextpointer-pointer);
				if ( element.size() > 0 && element.at(element.size()-1) == '\r' )
					element.erase(element.size()-1);
			}
			
//			if (element.size() >= 2 && element.at(0) == '"' && element.at(element.size()-1) == '"')
//				element = element.substr(1, element.size()-2);
			tmprow[headermap_arr.at(currele)] = element;
			if ( nextpointer != -1 )
				pointer = nextpointer+1;
			else
				pointer++;
			++currele;
		}
	}
	retrows.push_back(tmprow);
	return retrows;
}
