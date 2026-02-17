/* 
	Title: Jellyfish - extra.h
	Author: "Giacomo Trudu" aka "Wicker25" - wicker25[at]gmail[dot]com
	Site: https://github.com/Wicker25
	License: (CC) BY-NC-SA - http://creativecommons.org/licenses/by-nc-sa/2.5/it/
	Description: Jellyfish is a free download application
*/

#ifndef EXTRA_H
#define EXTRA_H

#include <iostream>
#include <cstdlib>
#include <fstream> 
#include <ctime>
#include <sys/stat.h>
#include <sys/types.h>

#include "msgs.h"

#ifndef DOWNLOADS_DIRECTORY
#define DOWNLOADS_DIRECTORY "Downloads/"
#endif

using namespace std;

namespace JE {

const char *get_time() { 

	time_t rawtime;
	struct tm * timeinfo;

	time( &rawtime );
	timeinfo = localtime( &rawtime );

	string out = asctime(timeinfo);
	out.erase( out.end() - 1 );

	return out.c_str();
}

inline string replace_dot( string str ) {

	size_t pos = 0;
	while ( ( pos = str.find_first_of( '.', pos + 1 ) ) != -1 )
		str.replace( pos++, 1, "," );

	return str;
}

} // namespace JE

#endif
