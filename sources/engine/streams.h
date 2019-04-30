/* 
	Title: Jellyfish - streams.h
	Author: "Giacomo Trudu" aka "Wicker25" - wicker25[at]gmail[dot]com
	Site: http://www.hackyourmind.org/
	License: (CC) BY-NC-SA - http://creativecommons.org/licenses/by-nc-sa/2.5/it/
	Description: Jellyfish is a free download application
*/

#ifndef STREAMS_H
#define STREAMS_H

#include "consts.h"

#include <iostream>
#include <fstream> 
#include <cstring>
#include <zlib.h>

#include "structs.h"
#include "msgs.h"
#include "regexps.h"
#include "extra.h"

namespace JE {

// Codici di controllo
typedef enum {

	STREAMS_EMPTY = 0,
	STREAMS_NO_FILE = -1,
	
} StreamCode;

class Streams {

	/* * * * * * * * * * * * * * * * * * * * * * * * *\
	 * Gestore di accesso ai file di configurazione  *
	\* * * * * * * * * * * * * * * * * * * * * * * * */

public:

	template < class T_Engine >
	Streams( T_Engine *ObjEngine );
	virtual ~Streams();

	// Verifica che la directory di jellyfish esista
	inline void check_directory();

	// Carica le impostazioni dell'Engine
	void read_settings();
	// Salva le impostazioni dell'Engine
	void write_settings();
	// Salva la lista dei server
	int write_servers_list();
	// Carica la lista dei server
	int read_servers_list();
	// Salva la lista dei file
	int write_files_list();
	// Carica la lista dei file
	int read_files_list();
	// Salva la coda dei download
	int write_downloads_list( DownloadsList &downloads_list );
	// Carica la coda dei download
	int read_downloads_list( DownloadsList &downloads_list );

protected:

	RegExps *ObjRegExps; // Puntatore al gestore delle RegExps
	DataList *data_list; // Puntatore alla lista dei server
};

} // namespace JE

#include "streams.cpp"

#endif
