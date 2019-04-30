/* 
	Title: Jellyfish - streams.cpp
	Author: "Giacomo Trudu" aka "Wicker25" - wicker25[at]gmail[dot]com
	Site: http://www.hackyourmind.org/
	License: (CC) BY-NC-SA - http://creativecommons.org/licenses/by-nc-sa/2.5/it/
	Description: Jellyfish is a free download application
*/

#ifndef STREAMS_CPP
#define STREAMS_CPP

namespace JE {

using namespace std;

template < class T_Engine >
Streams::Streams( T_Engine *ObjEngine ) {

	// Collego il gestore delle espressioni regolari
	this->ObjRegExps = ObjEngine->get_ObjRegExps();

	// Collego le strutture
	this->data_list = ObjEngine->get_data_list();
}

Streams::~Streams() {

}

inline void Streams::check_directory() {

	struct stat dir_stat;
	if ( stat( JELLYFISH_DIRECTORY, &dir_stat ) != 0 ) mkdir( JELLYFISH_DIRECTORY, S_IRWXU );
	if ( stat( DOWNLOADS_DIRECTORY, &dir_stat ) != 0 ) mkdir( DOWNLOADS_DIRECTORY, S_IRWXU );
}

void Streams::write_settings() {  

	// Apro il file
	ofstream out( SETTINGS_PATH );

	if ( !out.is_open() ) return;

	// Puntatori alle strutture delle opzioni
	void *options[] = {

		&E_SOCKET_CONNECT_TIMEOUT,
		&E_SOCKET_TIMEOUT,
		&E_SOCKET_REPEAT,
		&E_SOCKET_WAIT_REPEAT,
		&E_SOCKET_WAIT_READ,
		&E_SOCKET_PROXY,
		&E_SOCKET_PROXY_PORT,
		&E_MAX_SPIDERS,
		&E_MAX_DOWNLOADS,
		&E_FILE_MANAGER,
		&E_FOUNDS_LIMIT,
	};

	// Scrivo tutti i dati
	for( unsigned int i = 0; i < 11; i++ ) {

		if ( i != 5 && i != 9 ) {

			long int *value = (long int *) options[i];
			out << *value << endl;

		} else {

			std::string *value = (std::string *) options[i];
			if ( value->empty() ) out << '-' << endl;
			else out << *value << endl;
		}
	}

	// Chiudo il file
	out.close();
}

void Streams::read_settings() {  

	string tmp;

	unsigned int t = 0;

	// Apro il file
	ifstream in( SETTINGS_PATH );

	if ( !in.is_open() ) {

		fprintf( stderr, E_MSG_OPEN_SETTINGS_ERROR );	
		return;
	}

	// Puntatori alle strutture delle opzioni
	void *options[] = {

		&E_SOCKET_CONNECT_TIMEOUT,
		&E_SOCKET_TIMEOUT,
		&E_SOCKET_REPEAT,
		&E_SOCKET_WAIT_REPEAT,
		&E_SOCKET_WAIT_READ,
		&E_SOCKET_PROXY,
		&E_SOCKET_PROXY_PORT,
		&E_MAX_SPIDERS,
		&E_MAX_DOWNLOADS,
		&E_FILE_MANAGER,
		&E_FOUNDS_LIMIT,
	};

	// Legge una riga alla volta
	while ( getline( in, tmp ) ) {

		if ( t > 10 ) break;

		// Ignoro le righe vuote o identificate come commento
		if ( tmp.length() && tmp.at(0) != COMMENT_CHAR ) {

			if ( t != 5 && t != 9 ) {

				long int *value = (long int *) options[t];
				*value = atol( tmp.c_str() );

			} else {

				std::string *value = (std::string *) options[t];
				if ( tmp == "-" ) *value = "";
				else *value = tmp.c_str();
			}

			t++;
		}
	}

	// Chiudo il file
	in.close();
}

int Streams::write_servers_list() {  

	// Iteratori
	DataList::iterator i = this->data_list->begin();
	DataList::iterator end = this->data_list->end();

	unsigned int size = 0;

	// Apro il file
	ofstream out( SERVERS_LIST_PATH );

	if ( !out.is_open() ) return STREAMS_NO_FILE;

	// Piccola intestazione
	out << E_HEADER_SERVERS_LIST;

	// Scrivo tutti i dati sui server
	while ( i != end ) {

		out << i->url << SERVER_SEPARATOR;
		out << i->enabled << SERVER_SEPARATOR;
		out << i->ping << endl;
		size++; i++;
	}

	// Chiudo il file
	out.close();

	return size;
}

int Streams::read_servers_list() {  

	Server server;
	string tmp;

	unsigned int i, t = 0, line = 0;
	bool exist;

	// Apro il file
	ifstream in( SERVERS_LIST_PATH );

	if ( !in.is_open() ) {

		fprintf( stderr, E_MSG_OPEN_SERVERS_LIST_ERROR );	
		return STREAMS_NO_FILE;
	}

	// Legge una riga alla volta
	while ( getline( in, tmp ) ) {

		// Conta le righe
		line++;

		// Ignoro le righe vuote o identificate come commento
   		if ( tmp.length() && tmp.at(0) != COMMENT_CHAR ) {

			// Estraggo il server dalla lista
			if ( this->ObjRegExps->get_ReServersList()->search( tmp ) ) {

				// Ricavo i dati
				server.url = this->ObjRegExps->get_ReServersList()->get_match(0);
				server.enabled = (bool) atoi( this->ObjRegExps->get_ReServersList()->get_match(1).c_str() );
				server.ping = atof( replace_dot( this->ObjRegExps->get_ReServersList()->get_match(2) ).c_str() );

				exist = false;

				// Controllo se esiste già in lista
				DataList::iterator i = this->data_list->begin();
				DataList::iterator end = this->data_list->end();

				while ( i != end ) {
				
					if ( i->url == server.url ) {

						fprintf( stderr, E_MSG_DUPLICATE_SERVER, SERVERS_LIST_PATH, line, server.url.c_str() );
						exist = true;
						break;
					}
					i++;
				}

				// Se non esiste, lo aggiungo in lista
				if ( !exist ) this->data_list->push_back( server );

			} else { fprintf( stderr, E_MSG_SYNTAX_ERROR, SERVERS_LIST_PATH, line ); continue; }

   			t++;
   		}
	}

	// Chiudo il file
	in.close();

	// Controllo se è stato trovato almeno un server
	if ( t == STREAMS_EMPTY ) fprintf( stderr, E_MSG_EMPTY_SERVERS_LIST );

	return t;
}

int Streams::write_files_list() {  

	// Iteratori
	DataList::iterator server_iterator = this->data_list->begin();
	DataList::iterator server_end = this->data_list->end();
	FilesList::iterator files_iterator, files_end;

	unsigned int size;

	// Apro il file
	//ofstream out( FILES_LIST_PATH );
	gzFile fout = gzopen( FILES_LIST_PATH, "wb6" );

	//if ( !out.is_open() ) return STREAMS_NO_FILE;
	if ( !fout ) return STREAMS_NO_FILE;

	// Piccola intestazione
	//out << E_HEADER_FILES_LIST;
	gzprintf( fout, E_HEADER_FILES_LIST );

	while ( server_iterator != server_end ) {

		// Scrivo l'intestazione del server
		//out << "[:" << server_iterator->url << ":]" << endl; // l'hoster dei file
		gzprintf( fout, SERVER_HEADER, server_iterator->url.c_str() );

		// Scrivo il corpo della lista
		files_iterator = server_iterator->files_list.begin();
		files_end = server_iterator->files_list.end();

		while ( files_iterator != files_end ) {

			gzprintf( fout, FILE_DATA,  files_iterator->name.c_str(),
										files_iterator->type.c_str(),
										files_iterator->path.c_str() );
			/*out << files_iterator->name << FILE_SEPARATOR;
			out << files_iterator->type << FILE_SEPARATOR;
			out << files_iterator->path << endl;*/
			files_iterator++;
			size++;
		}
		server_iterator++;
	}

	// Chiudo il file
	//out.close();
	gzclose( fout );

	return size;
}
			
int Streams::read_files_list() {  

	// Strutture di lavoro
	File file;
	string hoster;
	char buffer[FILE_DATA_BUFFER]; // Buffer di lettura

	// Iteratori
	DataList::iterator n = this->data_list->begin();
	DataList::iterator end = this->data_list->end();

	unsigned int t = 0, line = 0;

	bool exist = false;

	//ifstream in( FILES_LIST_PATH );
	gzFile fin = gzopen( FILES_LIST_PATH, "rb" );

	//if ( !in.is_open() ) {
	if ( !fin ) {

		fprintf( stderr, E_MSG_OPEN_FILES_LIST_ERROR );	
		return STREAMS_NO_FILE;
	}

	// Leggo una riga alla volta
	while( gzgets( fin, buffer, FILE_DATA_BUFFER ) ) {

		// Conto le righe
		line++;

		// Ignora le righe vuote o identificate come commento
		//if ( tmp.empty() || tmp.at(0) == COMMENT_CHAR ) continue;
		if ( buffer[0] == '\n' || buffer[0] == COMMENT_CHAR ) continue; // !!!!!

		// Ricerco l'intestazione della lista dei file
		if ( this->ObjRegExps->get_ReFilesList()[0]->search( buffer ) ) {

			// Estraggo il nuovo indirizzo dell'hoster
			hoster = this->ObjRegExps->get_ReFilesList()[0]->get_match(0);

			// Segno il server come inesistente (previo controllo)
			exist = false;

			// Reimposto l'iteratore
			n = this->data_list->begin();

			// Controllo se il server esiste
			while ( n != end ) {

				if ( n->url == hoster ) {

					// Memorizzo l'indice del server corrente
					exist = true;
					break;
				}
				n++;
			}

			// Avverto l'utente che il server non è stato definito
			if ( !exist ) fprintf( stderr, E_MSG_UNKNOWN_SERVER, FILES_LIST_PATH, line, hoster.c_str() );

		// Ricerco il corpo della lista dei file
		} else if ( exist ) {

			if ( this->ObjRegExps->get_ReFilesList()[1]->search( buffer ) ) {

				// Ricava i dati del file
				file.name = this->ObjRegExps->get_ReFilesList()[1]->get_match(0);
				file.type = this->ObjRegExps->get_ReFilesList()[1]->get_match(1);
				file.path = this->ObjRegExps->get_ReFilesList()[1]->get_match(2);

				// Aggiungo il file al gruppo
				n->files_list.push_back( file );
				// Incremento il numero dei file indicizzati
				t++;

			} else fprintf( stderr, E_MSG_SYNTAX_ERROR, FILES_LIST_PATH, line );
		}
	}

	// Chiudo il file
	//in.close();
	gzclose( fin );

	// Controllo se è stato indicizzato almeno un file
	if ( t == STREAMS_EMPTY ) fprintf( stderr, E_MSG_EMPTY_FILES_LIST );

	return t;
}

int Streams::write_downloads_list( DownloadsList &downloads_list ) {  

	// Iteratori
	DownloadsList::iterator i = downloads_list.begin();
	DownloadsList::iterator end = downloads_list.end();

	unsigned int size = STREAMS_EMPTY;

	// Apro il file
	ofstream out( DOWNLOADS_LIST_PATH );

	if ( !out.is_open() ) return STREAMS_NO_FILE;

	// Piccola intestazione
	out << E_HEADER_DOWNLOADS_LIST;

	// Scrivo tutti i dati sui server
	while ( i != end ) {

		out << i->name << DOWNLOADS_SEPARATOR;
		out << i->type << DOWNLOADS_SEPARATOR;
		out << i->path << DOWNLOADS_SEPARATOR;
		out << i->url << DOWNLOADS_SEPARATOR;
		out << i->status << endl;
		size++; i++;
	}

	// Chiudo il file
	out.close();

	return size;
}

int Streams::read_downloads_list( DownloadsList &downloads_list ) {  

	DownloadInfo download;
	string tmp;

	unsigned int i, t = 0, line = 0;
	bool exist;

	// Apro il file
	ifstream in( DOWNLOADS_LIST_PATH );

	if ( !in.is_open() ) {

		fprintf( stderr, E_MSG_OPEN_DOWNLOADS_LIST_ERROR );	
		return STREAMS_NO_FILE;
	}

	// Legge una riga alla volta
	while ( getline( in, tmp ) ) {

		// Conta le righe
		line++;

		// Ignoro le righe vuote o identificate come commento
   		if ( tmp.length() && tmp.at(0) != COMMENT_CHAR ) {

			// Estraggo il download dalla lista
			if ( this->ObjRegExps->get_ReDownloadsList()->search( tmp ) ) {

				// Ricavo i dati
				download.name = this->ObjRegExps->get_ReDownloadsList()->get_match(0);
				download.type = this->ObjRegExps->get_ReDownloadsList()->get_match(1);
				download.path = this->ObjRegExps->get_ReDownloadsList()->get_match(2);
				download.url = this->ObjRegExps->get_ReDownloadsList()->get_match(3);
				download.status = (DownloadStatus) atoi( this->ObjRegExps->get_ReDownloadsList()->get_match(4).c_str() );

				exist = false;

				// Controllo se esiste già in lista
				DownloadsList::iterator i = downloads_list.begin();
				DownloadsList::iterator end = downloads_list.end();

				while ( i != end ) {
				
					if ( i->url == download.url ) {

						fprintf( stderr, E_MSG_DUPLICATE_DOWNLOAD, DOWNLOADS_LIST_PATH, line, download.url.c_str() );
						exist = true;
						break;
					}
					i++;
				}

				// Se non esiste, lo aggiungo in lista
				if ( !exist ) downloads_list.push_back( download );

			} else { fprintf( stderr, E_MSG_SYNTAX_ERROR, DOWNLOADS_LIST_PATH, line ); continue; }

   			t++;
   		}
	}

	// Chiudo il file
	in.close();

	return t;
}

} // namespace Engine

#endif
