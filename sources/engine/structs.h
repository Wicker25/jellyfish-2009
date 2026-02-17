/* 
	Title: Jellyfish - Structs.cpp
	Author: "Giacomo Trudu" aka "Wicker25" - wicker25[at]gmail[dot]com
	Site: https://github.com/Wicker25
	License: (CC) BY-NC-SA - http://creativecommons.org/licenses/by-nc-sa/2.5/it/
	Description: Jellyfish is a free download application
*/

#ifndef STRUCTS_H
#define STRUCTS_H

#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <curl/curl.h>

namespace JE {

/********** INDICI **********/

/* Tipo del file */
typedef enum {

		OTHER_TYPE, // ALL_TYPE
		FOLDER_TYPE,
		AUDIO_TYPE,
		VIDEO_TYPE,
		IMAGE_TYPE,
		DOCUMENT_TYPE,
		PACKAGE_TYPE,
		EXECUTABLE_TYPE,

	} FileType;

/* Struttura di un file */
typedef struct {

			std::string name; // Nome completo
			std::string type; // Estensione ( '/' in caso di directory)
			std::string path; // Posizione all'interno del server

		} File;

/* Lista di file */
typedef std::list< File > FilesList;


/* Struttura di un server */
typedef struct {

			std::string url; // Indirizzo
			bool enabled; // Abilitato/disabilitato
			double ping; // Tempo di ping
			FilesList files_list; // Lista dei file indicizzati

		} Server;

/* Lista di server */
typedef std::list< Server > DataList;


/********** RISCONTRI **********/

/* Struttura contenente le posizioni dei riscontri trovati */
typedef struct {

			Server *server;
			File *file;

		} FoundFile;

/* Confronto tra due strutture di riscontro */
bool operator== ( const FoundFile &a, const FoundFile &b ) { 

	return ( a.server == b.server && a.file == b.file );
};

/* Lista di server */
typedef std::list< FoundFile > FoundFilesList;

/********** DOWNLOADS **********/

// Stato del download
typedef enum {

		WAIT_STATUS,
		STARTED_STATUS,
		PAUSED_STATUS,
		STOPED_STATUS,
		INTERRUPTED_STATUS,
		REMOVED_STATUS,
		QUEUED_STATUS,

	} DownloadStatus;
	
/* Informazioni sul download */
typedef struct {

			std::string name; // Nome del file
			std::string type; // Estensione ( '/' in caso di directory)
			std::string url; // Indirizzo del file sorgente
			std::string path; // Percorso del file di destinazione
			DownloadStatus status; // Stato del download

		} DownloadInfo;

/* Lista degli scaricamenti */
typedef std::list< DownloadInfo > DownloadsList; // DownloadsList


/* Stream del file da scaricare */
class dfstream : public std::ofstream {

public:

	// Nuova funzione d'apertura
	void open(	const DownloadInfo &download,
				ios_base::openmode mode = ios_base::out ) {

		// Apro lo stream
		std::ofstream::open( ( DOWNLOADS_DIRECTORY + download.path ).c_str(), mode );
	}

	// Nuova funzione d'apertura
	void open(	const std::string &url,	const std::string &name,
				const std::string &type, const std::string &path,
				ios_base::openmode mode = ios_base::out ) {

		std::string path_;

		// Apro lo stream
		if ( type == FOLDER ) path_ = path;
		else path_ = path + '.' + type;
		std::ofstream::open( ( DOWNLOADS_DIRECTORY + path_ ).c_str(), mode );
	}
};


/* Struttura per il tracciamento della connessione */
typedef struct {

	void *downloader; // Puntatore al gestore delle connessioni
	CURL *curl; // Struttura cUrl della connessione
	double downloaded; // Byte totali scaricati
	double lastd; // Byte scaricati nell'ultima ricezione
	double lastp; // Byte scaricati dell'ultimo progresso

} SocketTrack;

/* Contronto tra due tracciatori della connessione */
bool operator== ( const SocketTrack &a, const SocketTrack &b ) { return ( &a == &b ); } ;

/* Lista dei tracciatori delle connessioni */
typedef std::list< SocketTrack > SocketTrackList;
typedef std::vector< SocketTrack * > ptrSocketTrackList;


/* Vettore dei tracciatori dei download */
class DownloadTrack;
typedef std::list< DownloadTrack * > ptrDownloadTrackList;

/* Struttura per il tracciamento dei download (Struttura ad albero) */
class DownloadTrack {

	private:

	bool leaf_; // Indica se si tratta di un "Nodo" o di una "foglia"

	SocketTrack socket_; // Puntatore al tracciatore della connessione
	DownloadTrack *parent_; // Tracciatori del download padre
	ptrDownloadTrackList children_; // Tracciatori dei download figli
	dfstream *stream_; // Stream del file di destinazione

	public:

	DownloadInfo info; // Informazioni sul download

	void *ref; // Riferimento ad una struttura arbitraria

	// Metodo costruttore (NULL in caso di directory)
	DownloadTrack( bool leaf = true, DownloadTrack *parent_ = NULL ) {

		// Inizializza il tracciatore
		this->leaf_ = leaf;
		this->parent_ = parent_;
		this->stream_ = NULL;

		// Inizializza il tracciatore della connessione
		this->socket_.downloaded = 0;
		this->socket_.lastd = 0;
		this->socket_.lastp = 0;
	}

	// Metodo distruttore
	~DownloadTrack() {

	}

	// Aggiunge un tracciatore figlio
	void append_child( DownloadTrack *track ) {

		// Collego il padre al figlio
		track->set_parent( this );
		this->children_.push_back( track );
	}

	// Rimuove un tracciatore figlio
	void remove_child( DownloadTrack *track ) {

		// Rendo il figlio orfano
		track->set_parent( NULL );
		// Rimuovo un tracciatore figlio
		this->children_.remove( track );
	}

	// Separa il tracker dal padre e dai figli
	void disconnect() {

		// Segnalo al padre che ha appena perso un figlio
		if ( parent_ != NULL ) this->parent_->remove_child( this );

		// Segnalo ai figli che sono divenuti orfani
		ptrDownloadTrackList::iterator i = this->children_.begin();
		ptrDownloadTrackList::iterator end = this->children_.end();

		while ( i != end ) {

			(*i)->set_parent( NULL );
			i++;
		}
	}

	// Rimuove un tracciatore figlio
	dfstream *open_stream( bool append = false ) {

		if ( this->leaf_ == false ) return NULL;

		if ( !append ) {

			// Creo un nuovo stream
			this->stream_ = new dfstream;
			this->stream_->open( this->info, std::ios::binary );

		} else {

			// Leggo la dimensione attuale del file
			std::ifstream in( (DOWNLOADS_DIRECTORY + this->info.path).c_str(), 
								std::ios::in | std::ios::binary | std::ios::ate );
			double total = in.tellg();
			in.close();

			// Aggiorno i dati
			this->socket_.lastd			= total;
			this->socket_.lastp			= total;
			this->socket_.downloaded	= total;

			// Creo un nuovo stream in modalità "append"
			this->stream_ = new dfstream;
			this->stream_->open( this->info, std::ios::app | std::ios::binary );
		}

		return this->stream_;
	}

	// Rimuove un tracciatore figlio
	void delete_stream() {

		if ( this->leaf_ ) {

			// Reinizializza la connessione
			this->socket_.lastd = 0;
			this->socket_.lastp = 0;
			this->socket_.downloaded = 0;
			// Reinizializza lo stream
			if ( this->stream_ != NULL ) {

				this->stream_->close();
				this->stream_ = NULL;
			}

			// Rimuovo il file dall'hard disk
			remove( ( DOWNLOADS_DIRECTORY + this->info.path ).c_str() );

		} else {

			// Rimuovo la cartella dall'hard disk
			rmdir( ( DOWNLOADS_DIRECTORY + this->info.path ).c_str() );
		}
	}

	// Elimina il tracciatore
	void destroy( bool recursive = false ) {

		// Segnalo al padre che ha appena perso un figlio
		if ( parent_ != NULL ) this->parent_->remove_child( this );

		// Elimino l'albero dei figli
		ptrDownloadTrackList::iterator i = this->children_.begin();
		ptrDownloadTrackList::iterator end = this->children_.end();

		while ( i != end ) {

			(*i)->destroy();
			i++;
		}

		// Chiudo lo stream al file di destinazione
		if ( this->stream_ != NULL ) this->stream_->close();

		// Elimino l'istanza
		delete this;
	}

	// Metodi di accesso sicuro agli elementi
	bool is_leaf() { return this->leaf_; }
	SocketTrack *socket() { return &this->socket_; }
	DownloadTrack *parent() { return this->parent_; }
	void set_parent( DownloadTrack *_parent_ ) { this->parent_ = _parent_; }
	ptrDownloadTrackList *children() { return &this->children_; }
	dfstream *stream() { return this->stream_ != NULL ? this->stream_ : this->open_stream(); }
};

bool operator== ( const DownloadTrack &a, const DownloadTrack &b ) { return ( &a == &b ); } ;

/* Lista dei tracciatori dei download */
typedef std::list< DownloadTrack > DownloadTrackList;

} // namespace JE

#endif
