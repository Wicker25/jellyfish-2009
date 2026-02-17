/* 
	Title: Jellyfish - engine.h
	Author: "Giacomo Trudu" aka "Wicker25" - wicker25[at]gmail[dot]com
	Site: https://github.com/Wicker25
	License: (CC) BY-NC-SA - http://creativecommons.org/licenses/by-nc-sa/2.5/it/
	Description: Jellyfish is a free download application
*/

#ifndef ENGINE_H
#define ENGINE_H

namespace JE {

	// Opzioni dell'engine
	long int		E_SOCKET_CONNECT_TIMEOUT	= 60000; // (millisecondi) 30 secondi
	long int		E_SOCKET_TIMEOUT			= 1200000; // (millisecondi) 20 minuti
	long int		E_SOCKET_REPEAT				= 5;
	long int		E_SOCKET_WAIT_REPEAT		= 3000000; // (microsecondi) 3 secondi
	long int		E_SOCKET_WAIT_READ			= 50000; // (microsecondi) 0,05 secondi
	std::string		E_SOCKET_PROXY				= "";
	long int		E_SOCKET_PROXY_PORT			= 0;
	long int		E_MAX_SPIDERS				= 15;
	long int		E_MAX_DOWNLOADS				= 5;
	std::string		E_FILE_MANAGER				= "";
	long int		E_FOUNDS_LIMIT				= 350;
}

#include "consts.h"
#include "help.h"

#include <iostream>
#include <cstdio>
#include <vector>

#include "structs.h"
#include "streams.h"
#include "socket.h"
#include "crawler.h"

namespace JE {

// Codici di controllo
typedef enum {

		ENGINE_DONE,
		ENGINE_SERVER_EXIST,

	} EngineCode;

// Identificatori dell'opzione
typedef enum {

		SOCKET_CONNECT_TIMEOUT_OPTION,
		SOCKET_TIMEOUT_OPTION,
		SOCKET_REPEAT_OPTION,
		SOCKET_WAIT_REPEAT_OPTION,
		SOCKET_WAIT_READ_OPTION,
		SOCKET_PROXY_OPTION,
		SOCKET_PROXY_PORT_OPTION,
		MAX_SPIDERS_OPTION,
		MAX_DOWNLOADS_OPTION,
		FILE_MANAGER_OPTION,
		FOUNDS_LIMIT_OPTION,

	} OPTION;

class Engine {

	/* * * * * * * * * * * * * * * * * * *\
	 *        JellyFish's Engine         *
	\* * * * * * * * * * * * * * * * * * */

public:

	Engine( int argc, char *argv[] );
	virtual ~Engine();

	// Scrive le configurazioni dell'Engine
	inline void write_settings();
	// Carica i dati relativi ai server e ai file
	void read_data_list();
	// Salva i dati relativi ai server e ai file
	void write_data_list();
	// Pulisce i dati relativi ai server e ai file
	inline void clear_data_list();
	// Salva i dati relativi agli scaricamenti
	inline void write_downloads_list();
	// Carica i dati relativi ai download
	inline void read_downloads_list( DownloadsList &downloads_list, bool resume );
	// Modifica il valore di un parametro dell'engine
	inline void set_opt( OPTION option, long int value );
	inline void set_opt( OPTION option, std::string value );
	// Ritorna il puntatore ad un parametro dell'engine
	inline void *get_opt( OPTION option );

	// Aggiunge un nuovo server alla lista
	EngineCode add_new_hoster( const std::string &hoster, Server **new_server );
	// Aggiorna il valore del ping di un server
	void new_ping_hoster( Server *server );
	// Aggiorna i file indicizzati di un server
	void new_index_files( Server *hoster );
	// Verifica la correttezza di un indirizzo
	bool validate_url( const std::string &hoster );
	// Verifica se un server e' in lista
	EngineCode hoster_exist( const std::string &hoster, Server **server );
	// Abilita/Disabilita un server
	void enable_server( Server *server, bool enabled );
	// Rimuove un server (e i suoi file) dalla lista
	void remove_hoster( const std::string &hoster );

	// Cerca un file indicizzato
	EngineCode find_file( const std::string &key, FileType type, unsigned int max );
	// Cerca i file di una directory
	EngineCode get_sub_files( FoundFile &found );
	// Controlla il tipo del file
	FileType get_file_type( const std::string &type );

	// Aggiungo un nuovo file alla coda degli scaricamenti
	void append_download( const FoundFile &found );
	// Ferma momentaneamente la connessione
	void pause_download( DownloadTrack *track );
	// Ripristina la connessione interrotta
	void resume_download( DownloadTrack *track );
	// Rimuovoe un file dalla coda degli scaricamenti
	void remove_download( DownloadTrack *track );
	// Rimuove un file dalla coda degli scaricamenti e dall'hard disk
	void remove_and_clean_download( DownloadTrack *track );
	// Apro il file scaricato
	void open_downloaded_file( DownloadTrack *track );
	
	// Scrivo la lista dei server
	inline void write_servers_list() { 

		fprintf( stdout, E_MSG_WRITE_SERVERS_LIST );
		this->ObjStreams->write_servers_list();
	}

	// Scrivo gli indici
	inline void write_files_list() { 

		fprintf( stdout, E_MSG_WRITE_FILES_LIST );
		this->ObjStreams->write_files_list();
	}

	// Funzione d'aggiornamento dei progressi
	void progress_function( DownloadTrack *track, double dlnow, double dltotal, double speed, bool complete );
	void status_function( DownloadTrack *track, DownloadStatus status );

	// Metodi di accesso sicuro ai membri
	DataList *get_data_list() const { return this->data_list; }
	FoundFilesList *get_found_files() const { return this->found_files; }

	RegExps *get_ObjRegExps() const { return this->ObjRegExps; }
	Streams *get_ObjStreams() const { return this->ObjStreams; }
	Socket *get_ObjSocket() const { return this->ObjSocket; }
	Crawler *get_ObjCrawler() const { return this->ObjCrawler; }

	// Segnali
	sigc::signal< void *, DownloadTrack *, void * > 
	*signal_connect() { return &this->ObjCrawler->connect_function; };
	sigc::signal< void, DownloadTrack *, double, double, double, bool > 
	*signal_progress() { return &this->ObjSocket->progress_function; };
	sigc::signal< void, DownloadTrack *, JE::DownloadStatus >
	*signal_status() { return &this->ObjSocket->status_function; };

protected:

	// Le strutture
	DataList *data_list; // Lista dei server
	FoundFilesList *found_files; // Lista dei riscontri

	// I gestori
	RegExps *ObjRegExps;
	Streams *ObjStreams;
	Socket *ObjSocket;
	Crawler *ObjCrawler;
};

} // namespace JE

#include "engine.cpp"

#endif
