/* 
	Title: Jellyfish - crawler.h
	Author: "Giacomo Trudu" aka "Wicker25" - wicker25[at]gmail[dot]com
	Site: https://github.com/Wicker25
	License: (CC) BY-NC-SA - http://creativecommons.org/licenses/by-nc-sa/2.5/it/
	Description: Jellyfish is a free download application
*/

#ifndef CRAWLER_H
#define CRAWLER_H

#include "consts.h"

#include <iostream>
#include <vector>
#include <pstreams/pstream.h>
#include <pthread.h>

#include "structs.h"
#include "msgs.h"
#include "regexps.h"
#include "socket.h"
#include "extra.h"

namespace JE {

class Crawler {

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * *\
	 * Crawler per l'indicizzazione ed il download dei file *
	\* * * * * * * * * * * * * * * * * * * * * * * * * * * */

private:

	/* Struttura per il tracciamento dello spider */
	typedef struct {

		Crawler *This; // Riferimento all'oggetto
		short int thread_id; // Id del thread
		std::string url; // Url sorgente
		FilesList *files_list; // Lista dei file
		unsigned int hoster_lenght; // Dimesioni dell'indirizzo base
		unsigned short int depth; // Profondità dell'esplorazione

	} SpiderTrack;

	// Puntatori ai gestori
	Socket *ObjSocket;
	Streams *ObjStreams;
	RegExps *ObjRegExps;

	// Puntatori alle liste dei dati
	DataList *data_list;

	// Lista dei thread d'indicizzazione
	pthread_t *spiders;
	pthread_mutex_t files_list_mutex;

public:

	template < class T_Engine >
	Crawler( T_Engine *ObjEngine );
	~Crawler();

	// Indicizza i file di un server
	void start_index_files( const std::string &url,
					  FilesList *files, unsigned int &hoster_lenght, 
					  unsigned short int depth );
	// Singolo spider (Thread)
	static void *spider( void *track );
	// Decodifica il nome del file
	inline string format_name( string path );
	// Decodifica le entità HTML
	void decode_HTML_special_chars( string &path );
	// Estrae l'indirizzo del server da un percorso generico
	inline void extract_hoster( string &url );
	// Calcola il tempo di risposta di un server
	double ping( const std::string &url );
	// Cerca un file nella lista
	int find_file( const std::string &key, DataList *data_list, FoundFilesList *found_files, FileType type, unsigned int max );
	// Cerca i file di una specifica directory
	int get_sub_files( const std::string &parent, Server *server, FoundFilesList *found_files );

	// Aggiunge un riscontro alla coda degli scaricamenti
	void append_download( const FoundFile &found, DownloadTrack *parent_track );
	// Ferma momentaneamente la connessione
	inline void pause_download( DownloadTrack *track );
	// Ripristina un download interrotto
	inline void resume_download( DownloadTrack *track );
	// Ripristina i download salvati
	void resume_downloads_from_list( DownloadsList &downloads_list );
	// Rimuove un file dalla coda degli scaricamenti
	inline void remove_download( DownloadTrack *track );
	// Rimuove un file dalla coda degli scaricamenti e dall'hard disk
	inline void remove_and_clean_download( DownloadTrack *track );

	// Funzione di connessione
	sigc::signal< void *, DownloadTrack *, void * > connect_function;

	// Genera la lista dei download in corso
	DownloadsList get_downloads_list();
};

} // namespace JE

#include "crawler.cpp"

#endif
