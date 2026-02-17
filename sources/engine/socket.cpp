/* 
	Title: Jellyfish - socket.cpp
	Author: "Giacomo Trudu" aka "Wicker25" - wicker25[at]gmail[dot]com
	Site: https://github.com/Wicker25
	License: (CC) BY-NC-SA - http://creativecommons.org/licenses/by-nc-sa/2.5/it/
	Description: Jellyfish is a free download application
*/

#ifndef SOCKET_CPP
#define SOCKET_CPP

namespace JE {

using namespace std;

Socket::Socket() {

	// Inizializzo le cUrl
	curl_global_init( CURL_GLOBAL_ALL );

	// Inizializzo le variabili di controllo
	this->thread_is_running = false;

	// Inizializzo il gestore dei downloads
	if ( ( this->multi_curl = curl_multi_init() ) == NULL ) return;

	// Configuro il gestore
	curl_multi_setopt( this->multi_curl, CURLMOPT_SOCKETFUNCTION, multi_socket );
	curl_multi_setopt( this->multi_curl, CURLMOPT_MAXCONNECTS, E_MAX_DOWNLOADS );
}

Socket::~Socket() {

}

int Socket::writer( char *data, size_t size, size_t nmemb, std::string *buffer ) {  

	register int result = 0;

	// Scrivo nel buffer
	if ( buffer != NULL ) {

		result = size * nmemb;
		buffer->append( data, result );
	}

	return result;
}

int Socket::file_writer( char *data, size_t size, size_t nmemb, dfstream *stream ) {  

	register int result = 0;

	// Scrivo nello stream del file
	if ( stream != NULL ) {

		result = size * nmemb;
		stream->write( data, result );
	}

	return result;
}

int Socket::progress( DownloadTrack *track, double dltotal, double dlnow, 
		   				double ultotal, double ulnow ) {

	double dlnow_ = dlnow + track->socket()->downloaded;
	double dltotal_ = dltotal + track->socket()->downloaded;

	// Estraggo le nuove informazioni
	double speed = 0;
	curl_easy_getinfo( track->socket()->curl, CURLINFO_SPEED_DOWNLOAD, &speed );
	bool complete = ( dlnow == dltotal && dltotal > 0 );

	// Aggiorno la quantità di dati ricevuti
	track->socket()->lastd = dlnow_;

	// In caso di cambiamenti avvio il segnale d'aggiornamento dei progressi
	// ( L'intervallo d'aggiornamento è relativo alla velocità di download )
	if ( dlnow_ > track->socket()->lastp || dlnow_ >= dltotal_ && dltotal > 0 ) { // 2.048

		if (track->info.status == STARTED_STATUS )
			((Socket *) track->socket()->downloader)->progress_function( track, dlnow_/1024, dltotal_/1024, speed/1024, complete );
		track->socket()->lastp = dlnow_ + ( speed * 1.9 );
	}

	// Se il download è terminato modifico lo stato
	if ( track->info.status != STOPED_STATUS && complete && track->info.status != INTERRUPTED_STATUS )
		track->info.status = STOPED_STATUS;

	return 0;
}

CURL *Socket::create_socket( const std::string &url ) {

	CURL *curl;

	// Inizializzo il gestore della connessione
	if ( ( curl = curl_easy_init() ) == NULL ) {

		cout << "ERROR!!!" << endl;
		return NULL;
	}

	// Configuro la connessione
	curl_easy_setopt( curl, CURLOPT_USERAGENT, USER_AGENT );
	curl_easy_setopt( curl, CURLOPT_TIMEOUT_MS, E_SOCKET_TIMEOUT );
	curl_easy_setopt( curl, CURLOPT_CONNECTTIMEOUT_MS, E_SOCKET_CONNECT_TIMEOUT );
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1 );
	//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1 );
	curl_easy_setopt( curl, CURLOPT_URL, url.c_str() );

	return curl;
}

inline void Socket::set_connection_opts( DownloadTrack *track ) {

	// Configuro la connessione per lo scaricamento di un file
	curl_easy_setopt( track->socket()->curl, CURLOPT_WRITEDATA, track->stream() );
	curl_easy_setopt( track->socket()->curl, CURLOPT_WRITEFUNCTION, &Socket::file_writer );

	curl_easy_setopt( track->socket()->curl, CURLOPT_PROGRESSFUNCTION, &Socket::progress );
	curl_easy_setopt( track->socket()->curl, CURLOPT_PROGRESSDATA, track );

	curl_easy_setopt( track->socket()->curl, CURLOPT_NOPROGRESS, false );
	curl_easy_setopt( track->socket()->curl, CURLOPT_PRIVATE, (void *) track );

	if ( !E_SOCKET_PROXY.empty() ) {

		curl_easy_setopt( track->socket()->curl, CURLOPT_PROXY, E_SOCKET_PROXY.c_str() );
		curl_easy_setopt( track->socket()->curl, CURLOPT_PROXYPORT, E_SOCKET_PROXY_PORT );
		// curl_easy_setopt( track->socket()->curl, CURLOPT_NOPROXY, false );
	}

	//curl_easy_setopt( track->socket()->curl, CURLOPT_FOLLOWLOCATION, false );
}

CURLMcode Socket::multi_socket( CURL *curl, curl_socket_t sockfd,
								int action, void *userp, void *socketp ) {

	// Estraggo le informazioni sulla connessione
	DownloadTrack *track;
	curl_easy_getinfo( curl, CURLINFO_PRIVATE, &track );

	// Se lo stato corrente è "Rimozione" ignora qualsiasi aggiornamento dello stato
	if ( track->info.status == REMOVED_STATUS ) return CURLM_OK;

	// Memorizzo lo stato corrente
	DownloadStatus old_status = track->info.status;

	// Estraggo il content-type
	char *mime_type;
	curl_easy_getinfo( track->socket()->curl, CURLINFO_CONTENT_TYPE, &mime_type );

	// Controlla il content-type
	if ( mime_type && ( strncmp( mime_type, "text/html", 9 ) == 0 ) ) {

		fprintf( stdout, E_MSG_DOWNLOAD_CONTENT_TYPE_ERROR );
		// Azzera le informazioni sul download
		track->info.status = INTERRUPTED_STATUS;
		((Socket *) track->socket()->downloader)->status_function( track, track->info.status );
		// Resetto lo stream
		track->delete_stream();
	}

	// Ricavo lo stato
	if ( action == 0 || action == 2 ) track->info.status = WAIT_STATUS;
	else if ( action == 1 ) track->info.status = STARTED_STATUS;
	else if ( action == 4 )

		if ( track->info.status != INTERRUPTED_STATUS &&
			 track->info.status != STOPED_STATUS &&
			 track->info.status != PAUSED_STATUS &&
			 track->info.status != QUEUED_STATUS ) {

			// Aggiorno lo stato
			track->info.status = INTERRUPTED_STATUS;
		}

	// Segnalo il cambiamento di stato
	if ( old_status != track->info.status || track->info.status == STOPED_STATUS )
		((Socket *) track->socket()->downloader)->status_function( track, track->info.status );

	return CURLM_OK;
}

inline void Socket::start_downloader_thread() {

	// Se il thread dei downloads non è attivo, ne creo uno nuovo
	if ( !this->thread_is_running ) {

		pthread_t tid;
		int code = pthread_create( &tid, NULL, &Socket::files_downloader, (void *) this );
		if ( code != 0 ) fprintf( stderr, E_MSG_DOWNLOAD_THREAD_ERROR );
	}
}

CURLcode Socket::download_page( const std::string &url, std::string *buffer ) {

	// Puntatori alle strutture
	CURL *curl;
	CURLcode code_curl;

	// Numero dei tentativi effettuati
	unsigned short int t = 0;

	// Ciclo dei tentativi
	do {

		// Pulisco il buffer
		buffer->clear();

		// Inizializzo il gestore della connessione
		if ( ( curl = this->create_socket( url ) ) == NULL ) {

			cout << "ERROR!!!" << endl;
			return CURLE_FAILED_INIT;
		}

		// Configuro la connessione
		curl_easy_setopt( curl, CURLOPT_WRITEDATA, buffer );
		curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, &Socket::writer );

		if ( t > 0 ) {

			fprintf( stderr, E_MSG_NEW_ATTEMPT, get_time(), t );
			usleep( E_SOCKET_WAIT_REPEAT );
		}

		// Scarico la pagina
		code_curl = curl_easy_perform( curl );

		// Pulisco la connessione
		curl_easy_cleanup( curl ); 

		t++;

	} while( ( code_curl != CURLE_OK ) && ( t <= E_SOCKET_REPEAT ) );

	// In caso di errore stampo un messaggio
	if ( code_curl != CURLE_OK ) this->print_warning( code_curl );

	return code_curl;
}

DownloadTrack *Socket::add_download( DownloadTrack *track ) {

	fprintf( stdout, E_MSG_ADD_DOWNLOAD, track->info.url.c_str() );

	// Tengo in memeoria il tracciatore
	this->sockets_list.push_back( track );

	// Configuro il tracciatore della connessione
	track->socket()->downloader	= this;

	// Byte totali scaricati
	long int total = track->socket()->downloaded;

	// Inizializzo il gestore della connessione
	if ( ( track->socket()->curl = this->create_socket( track->info.url ) ) == NULL );

	// Configuro la connessione
	this->set_connection_opts( track );

	if ( total > 0 )
		curl_easy_setopt( track->socket()->curl, CURLOPT_RESUME_FROM, total );

	// Aggiorna i progressi
	if ( track->info.status == STOPED_STATUS )
		this->progress_function( track, total/1024, total/1024, 0, true );

	// Controllo se si è superato il numero massimo di download simultanei
	if ( this->get_active_sockets() > E_MAX_DOWNLOADS ) {

		// Modifico lo stato
		track->info.status = QUEUED_STATUS;
	}

	if ( track->info.status == WAIT_STATUS ||
		 track->info.status == STARTED_STATUS ) { //track->info.status == PAUSED_STATUS

		// Aggiungo il download alla coda dei download
		if ( curl_multi_add_handle( this->multi_curl, track->socket()->curl ) != CURLM_OK );
		// Se il thread dei downloads non è attivo, ne creo uno nuovo
		this->start_downloader_thread();
	}

	// Chiamo il segnale d'aggiornamento dello stato
	this->status_function( track, track->info.status );

	return track;
}

inline void Socket::pause_download( DownloadTrack *track ) {

	fprintf( stdout, E_MSG_PAUSE_DOWNLOAD, track->info.url.c_str() );

	// Fermo momentaneamente la connessione
	if ( track->info.status != STOPED_STATUS &&
		 track->info.status != INTERRUPTED_STATUS ) {

		track->info.status = PAUSED_STATUS;
		// Rimuovo il download dalla coda del gestore
		curl_multi_remove_handle( this->multi_curl, track->socket()->curl );

		// Aggiorno lo stato
		this->status_function( track, track->info.status );
	}
}

inline void Socket::resume_download( DownloadTrack *track ) {

	fprintf( stdout, E_MSG_RESUME_DOWNLOAD, track->info.url.c_str() );

	// Ripristino la connessione
	if ( track->info.status == INTERRUPTED_STATUS ||
		 track->info.status == PAUSED_STATUS ||
		 track->info.status == QUEUED_STATUS ) {

		// Controllo che non si superi il numero massimo di download simultanei
		if ( this->get_active_sockets() < E_MAX_DOWNLOADS ) {

			// Aggiorno lo stato
			track->info.status = WAIT_STATUS;
			this->status_function( track, track->info.status );

			// Azzero i progressi
			track->socket()->downloaded = track->socket()->lastd;

			// Pulisco la connessione
			curl_multi_remove_handle( this->multi_curl, track->socket()->curl );
			curl_easy_cleanup( track->socket()->curl );

			// Reinizializzo il gestore della connessione
			if ( ( track->socket()->curl = this->create_socket( track->info.url ) ) == NULL );

			// Configuro la connessione
			this->set_connection_opts( track );
			long int size = track->socket()->lastd;
			curl_easy_setopt( track->socket()->curl, CURLOPT_RESUME_FROM, size );

			// Aggiungo il download alla coda del gestore
			if ( curl_multi_add_handle( this->multi_curl, track->socket()->curl ) != CURLM_OK );

			// Se il thread dei downloads non è attivo, ne creo uno nuovo
			this->start_downloader_thread();

		} else {

			// Modifico lo stato
			track->info.status = QUEUED_STATUS;
			this->status_function( track, track->info.status );
			this->progress_function( track, 0, 0, 0, true );
		}
	}
}

inline void Socket::remove_download( DownloadTrack *track ) {

	fprintf( stdout, E_MSG_REMOVE_DOWNLOAD, track->info.url.c_str() );

	// Modifico lo stato
	track->info.status = REMOVED_STATUS;

	// Rimuovo i dati della connessione
	curl_multi_remove_handle( this->multi_curl, track->socket()->curl );
	curl_easy_cleanup( track->socket()->curl );

	// Rimuovo la connessione dalla lista
	this->sockets_list.remove( track );

	// Notifico il cambiamento di stato
	this->status_function( track, track->info.status );
}

void *Socket::files_downloader( void *pThis ) {

	// Puntatore al gestore delle connessioni
	Socket *This = (Socket *) pThis;

	This->thread_is_running = true;

	// Codici d'errore e numero delle connessioni attive
	CURLMcode code_curl;
	int running = 0;

	fprintf( stdout, E_MSG_START_DOWNLOAD_THREAD );

	// Gestisto le connessioni
	do {

		if ( curl_multi_socket_all( This->multi_curl, &running ) != CURLM_OK );
		//	cout << "ERROR!!!" << endl;

		// Attiva tutti i download possibili
		if ( running < E_MAX_DOWNLOADS ) This->active_sockets();

		// Da ossigeno alla cpu (frequenza di aggiornamento)
		usleep( E_SOCKET_WAIT_READ );

	} while ( running );

	fprintf( stdout, E_MSG_END_DOWNLOAD_THREAD );

	This->thread_is_running = false;
}

void Socket::active_sockets() {

	// Iteratori
	ptrDownloadTrackList::iterator i = this->sockets_list.begin();
	ptrDownloadTrackList::iterator end = this->sockets_list.end();

	int actives = 0;

	while ( i != end ) {

		if ( (*i)->info.status == WAIT_STATUS ||
			 (*i)->info.status == STARTED_STATUS ) actives++;

		else if ( (*i)->info.status == QUEUED_STATUS )

			if ( actives < E_MAX_DOWNLOADS ) {

				this->resume_download(*i);
				break;
			}

			i++;
		}
	}

unsigned int Socket::get_active_sockets() {

	// Iteratori
	ptrDownloadTrackList::iterator i = this->sockets_list.begin();
	ptrDownloadTrackList::iterator end = this->sockets_list.end();

	int actives = 0;

	while ( i != end ) {

		if ( (*i)->info.status == WAIT_STATUS ||
			 (*i)->info.status == STARTED_STATUS ) actives++;

			i++;
	}
	
	return actives;
}

void Socket::print_warning( CURLcode code ) {

	// Stampa l'errore della connessione
	if ( code != CURLE_OK ) {

		if (code == CURLE_COULDNT_RESOLVE_HOST) fprintf( stderr, E_MSG_UNABLE_TO_CONNECT );
		else fprintf( stderr, E_MSG_UNABLE_TO_CONNECT );
	}

	return;
}

} // namespace Engine

#endif
