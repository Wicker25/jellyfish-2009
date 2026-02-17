/* 
	Title: Jellyfish - socket.h
	Author: "Giacomo Trudu" aka "Wicker25" - wicker25[at]gmail[dot]com
	Site: https://github.com/Wicker25
	License: (CC) BY-NC-SA - http://creativecommons.org/licenses/by-nc-sa/2.5/it/
	Description: Jellyfish is a free download application
*/

#ifndef SOCKET_H
#define SOCKET_H

#include "consts.h"

#include <iostream>
#include <fstream>
#include <sigc++/sigc++.h>
#include <curl/curl.h>
#include <pthread.h>
#include <ctime>

#include "structs.h"
#include "msgs.h"
#include "regexps.h"
#include "extra.h"

namespace JE {

class Socket {

	/* * * * * * * * * * * * * * *
	 * Gestore delle connessioni *
	 * * * * * * * * * * * * * * */ 

protected:

	// Gestore dei downloads
	CURLM *multi_curl;
	// Lista delle connessioni
	ptrDownloadTrackList sockets_list;
	// Variabile di controllo
	bool thread_is_running;

	// Scrive in una stringa i dati scaricati
	static int writer( char *data, size_t size, size_t nmemb, std::string *buffer );
	// Scrive in un file i dati scaricati
	static int file_writer( char *data, size_t size, size_t nmemb, dfstream *stream );
	// Aggiorna i progressi del download
	static int progress( DownloadTrack *track, double dltotal, double dlnow,
											double ultotal, double ulnow );
	// Gestisce le connessioni
	static CURLMcode multi_socket( CURLM * multi_handle, curl_socket_t sockfd, 
								int action, void *userp, void *socketp );

	// Se il thread dei downloads non è attivo, ne crea uno nuovo
	inline void start_downloader_thread();

public:

	Socket();
	~Socket();

	// Crea un oggetto cUrl preconfigurato
	CURL *create_socket( const std::string &url );
	// Configura la connessione per lo scaricamento di un file
	inline void set_connection_opts( DownloadTrack *track );
	// Scarica una pagina web
	CURLcode download_page( const std::string &url, std::string *buffer );
	// Aggiunge un download alla coda
	DownloadTrack *add_download( DownloadTrack *track );
	// Ferma momentaneamente la connessione
	inline void pause_download( DownloadTrack *track );
	// Ripristina la connessione
	inline void resume_download( DownloadTrack *track );
	// Rimuove un download alla coda
	inline void remove_download( DownloadTrack *track );
	// Scaricatore dei file
	static void *files_downloader( void *pThis );
	// Attiva tutti i download possibili
	void active_sockets();
	// Ritorna il numero dei sockets attivi
	unsigned int get_active_sockets();
	// Interpreta l'errore della connessione ed avvisa l'utente
	void print_warning( CURLcode code );

	// Funzione d'aggiornamento dei progressi
	sigc::signal< void, DownloadTrack *, double, double, double, bool > progress_function;
	// Funzione d'aggiornamento dello stato della connessione
	sigc::signal< void, DownloadTrack *, JE::DownloadStatus > status_function;

	// Metodi di accesso sicuro ai membri
	ptrDownloadTrackList *get_sockets_list() { return &this->sockets_list; }
};

} // namespace JE

#include "socket.cpp"

#endif
