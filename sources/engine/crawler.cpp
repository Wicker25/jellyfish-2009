/* 
	Title: Jellyfish - crawler.cpp
	Author: "Giacomo Trudu" aka "Wicker25" - wicker25[at]gmail[dot]com
	Site: https://github.com/Wicker25
	License: (CC) BY-NC-SA - http://creativecommons.org/licenses/by-nc-sa/2.5/it/
	Description: Jellyfish is a free download application
*/

#ifndef CRAWLER_CPP
#define CRAWLER_CPP

namespace JE {

using namespace std;

template < class T_Engine >
Crawler::Crawler( T_Engine *ObjEngine ) {

	// Inizializzo le strutture
	this->spiders = NULL;

	// Collego i gestori
	this->ObjSocket = ObjEngine->get_ObjSocket();
	this->ObjStreams = ObjEngine->get_ObjStreams();
	this->ObjRegExps = ObjEngine->get_ObjRegExps();

	// Collego le strutture
	this->data_list = ObjEngine->get_data_list();
}

Crawler::~Crawler() {

}

bool compare_( const File &first, const File &second ) {

	if ( first.path < second.path ) return true;
	else return false;
}

void Crawler::start_index_files( const std::string &url,
							FilesList *files_list, unsigned int &hoster_lenght,
							unsigned short int depth = 10 ) {

	// Inizializzo le strutture di memoria
	if ( this->spiders != NULL ) delete[] this->spiders;
	this->spiders = new pthread_t[E_MAX_SPIDERS];

	// Iteratore
	unsigned int i;

	// Preparo la prima traccia
	SpiderTrack *track = new SpiderTrack;

	track->This				= this;
	track->thread_id		= -1;
	track->url				= url;
	track->files_list		= files_list;
	track->hoster_lenght	= hoster_lenght;
	track->depth			= depth;

	// Azzero gli id degli spider
	for ( i = 0; i < E_MAX_SPIDERS; i++ ) this->spiders[i] = 0;

	// Avvio il primo spider
	this->spider( (void *) track );

	// Attendo l'azzeramento gli id degli spider
	bool cond = false;

	while ( !cond ) {

		cond = true;

		for ( i = 0; i < E_MAX_SPIDERS; i++ ) 
			if ( this->spiders[i] > 0 ) { cond = false; break; }

		usleep( 50000 );
	}

	// Riordino i percorsi
	files_list->sort( compare_ );
}

void *Crawler::spider( void *track_ ) {

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
	 * Estrae l'albero delle cartelle e dei file a partire da un indirizzo *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	SpiderTrack *track = (SpiderTrack *) track_;

	// Espressioni regolari
	pthread_mutex_lock( &track->This->files_list_mutex );
	Pcre ReUrls = track->This->ObjRegExps->get_ReUrls();
	Pcre ReNoFile = track->This->ObjRegExps->get_ReNoFile();
	Pcre ReNoSubDirectory = track->This->ObjRegExps->get_ReNoSubDirectory();
	pthread_mutex_unlock( &track->This->files_list_mutex );

	// Strutture dati
	File file; // Creo la base per un nuovo file
	string buffer; // Buffer per la memorizzazione della pagina
	size_t pos = 0; // Posizione dell'ultimo riscontro

	string type, path; // Tipo e percorso estratti

	fprintf( stdout, E_MSG_EXTRACT_FROM, track->url.c_str() );

	// Scarico la pagina corrente
	CURLcode c_code = track->This->ObjSocket->download_page( track->url, &buffer );

	if ( c_code == CURLE_OK && !buffer.empty() ) {

		// Estraggo tutti gli indirizzi dei file contenuti nella pagina
		while ( ReUrls.search( buffer, pos ) ) { 

			pos = ReUrls.get_match_end(0);

			// Estraggo le informazioni sul file
			type = ReUrls.get_match(1); // Tipo del file
			path = ReUrls.get_match(0); // Percorso del file

			// Decodifico le entità HTML
			pthread_mutex_lock( &track->This->files_list_mutex );
			track->This->decode_HTML_special_chars( path );
			pthread_mutex_unlock( &track->This->files_list_mutex );

			// Distinguo i file dalle cartelle
			if ( type == FOLDER ) { 

				// Controllo se si tratta di una sotto-directory
				if ( !ReNoSubDirectory.search( path ) ) {

					// Memorizzo il nome e il tipo del file
					pthread_mutex_lock( &track->This->files_list_mutex );
					file.name = track->This->format_name( path.substr( 0, ( path.length() - 1 ) ) );
					pthread_mutex_unlock( &track->This->files_list_mutex );
					file.type = type;

					// Ricavo e memorizzo il percorso del file
					path.insert( 0, track->url );
					file.path = path.substr( track->hoster_lenght );

					// Aggiungo il file alla lista
					pthread_mutex_lock( &track->This->files_list_mutex );
					track->files_list->push_back( file );
					pthread_mutex_unlock( &track->This->files_list_mutex );

					// Indicizzo la sottocartella
					if ( track->depth ) {

						// Preparo la nuova traccia
						SpiderTrack *new_track = new SpiderTrack;

						new_track->This				= track->This;
						new_track->thread_id		= -1;
						new_track->url				= path;
						new_track->files_list		= track->files_list;
						new_track->hoster_lenght	= track->hoster_lenght;
						new_track->depth			= ( track->depth - 1 );

						unsigned int i = 0;

						// Avvio un nuovo spider o, nel caso c'e ne siano troppi, proseguo con il corrente
						while ( true ) {

							if ( !track->This->spiders[i] ) {

								new_track->thread_id = i;

								pthread_attr_t attr;
								pthread_attr_init( &attr );
								pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_DETACHED );

								int code = pthread_create( &track->This->spiders[i], &attr, &Crawler::spider, (void *) new_track );
								pthread_attr_destroy( &attr );

								if ( code != 0 ) cout << "ERRORE!!!!!!" << endl;
								break;
							}

							if ( ++i == E_MAX_SPIDERS ) {

								track->This->spider( (void *) new_track );
								break;
							}
						}
					}
				}

			} else if ( !ReNoFile.search( path ) ) {

				// Memorizzo il nome e il tipo del file
				pthread_mutex_lock( &track->This->files_list_mutex );
				file.name = track->This->format_name( path.substr( 0, ( path.length() - type.length() ) ) );
				pthread_mutex_unlock( &track->This->files_list_mutex );
				file.type = ReUrls.get_match(2);

				// Memorizzo il percorso del file
				path.insert( 0, track->url ); // Ricostruisco l'indirizzo
				file.path = path.substr( track->hoster_lenght ); // Imposto il percorso del file

				// Aggiungo il file alla lista
				pthread_mutex_lock( &track->This->files_list_mutex );
				track->files_list->push_back( file );
				pthread_mutex_unlock( &track->This->files_list_mutex );
			}
		}
	}

	// Termina il thread
	if ( track->thread_id > -1 ) {

		pthread_mutex_lock( &track->This->files_list_mutex );
		track->This->spiders[track->thread_id] = 0;
		pthread_mutex_unlock( &track->This->files_list_mutex );

		delete track;
	}

	return NULL;
}

inline string Crawler::format_name( string path ) {

	int len = path.length();

	// Decodifico il nome del file
	path = curl_easy_unescape( NULL, path.c_str(), len, &len );

	return path; 
}

void Crawler::decode_HTML_special_chars( string &path ) {

	// Lista dei caratteri speciali
	char special_chars[][10] = { "&amp;", "&quot;", "&lt;", "&gt;", "\t", "\n", "[", "]" };
	char replacements[][3] = { "&", "\"", "<", ">", " ", " ", " ", " " };
	unsigned int lenght[] = { 5, 6, 4, 4, 1, 1, 1, 1 };

	string::size_type found;

	// Decodifico tutte le entità html
	for( unsigned int i = 0; i < 3; i++ ) {

		found = path.find( special_chars[i] );

		while ( found != string::npos ) {

			path.replace( found, lenght[i], replacements[i] );
			found = path.find( special_chars[i], ++found );
		}
	}
}

inline void Crawler::extract_hoster( string &url ) {

	// Ricavo l'indirizzo base
	if ( this->ObjRegExps->get_ReHost()->search( url ) )
		url = this->ObjRegExps->get_ReHost()->get_match(1);
}

double Crawler::ping( const std::string &url ) {

	// Calcola il tempo di risposta di un server
	std::string url_ = url;

	// Preparo l'indirizzo
	this->extract_hoster( url_ );

	fprintf( stdout, E_MSG_PING_SERVER, url_.c_str() );

	// Preparo i parametri
	std::vector< std::string > request;
	request.push_back( "ping" );
	request.push_back( "-c 1" );
	request.push_back( "-n" );
	request.push_back( url_ );

	// Avvio la richiesta
	redi::ipstream in( request[0], request );

	// Leggo l'output
	char str[200];
	in.read( str, 200 );

	// Estraggo il tempo di ping
	if ( this->ObjRegExps->get_RePingMs()->search( str ) )
		return (double) atof( replace_dot( this->ObjRegExps->get_RePingMs()->get_match(0) ).c_str() );

	return 0;
}

int Crawler::find_file( const std::string &key, DataList *data_list,
						FoundFilesList *found_files, FileType type = OTHER_TYPE, 
						unsigned int max = E_FOUNDS_LIMIT ) {

	// Nuovo riscontro
	FoundFile found;

	// Iteratori
	DataList::iterator server_iterator = this->data_list->begin();
	DataList::iterator server_end = this->data_list->end();
	FilesList::iterator file_iterator, file_end;

	// Numero dei riscontri
	unsigned int size = 0;

	// Imposto la chiave di ricerca
	this->ObjRegExps->set_key( key );

	// Controllo in tutti file dei server abilitati
	while ( server_iterator != server_end ) {

		if ( server_iterator->enabled ) {

			file_iterator = server_iterator->files_list.begin();
			file_end = server_iterator->files_list.end();

			while ( file_iterator != file_end ) {

				// Controllo il formato del file
				if ( ( type == OTHER_TYPE )		
				or ( type == FOLDER_TYPE && file_iterator->type == FOLDER )
				or ( type == AUDIO_TYPE && this->ObjRegExps->get_ReAudioTypes()->search( file_iterator->type ) )
				or ( type == VIDEO_TYPE && this->ObjRegExps->get_ReVideoTypes()->search( file_iterator->type ) )
				or ( type == IMAGE_TYPE && this->ObjRegExps->get_ReImageTypes()->search( file_iterator->type ) )
				or ( type == DOCUMENT_TYPE	&& this->ObjRegExps->get_ReDocumentTypes()->search( file_iterator->type ) )
				or ( type == PACKAGE_TYPE && this->ObjRegExps->get_RePackageTypes()->search( file_iterator->type ) )
				or ( type == EXECUTABLE_TYPE && this->ObjRegExps->get_ReExecutableTypes()->search( file_iterator->type ) ) ) {

					// Controllo il nome del file
					if ( this->ObjRegExps->get_ReKey()->search( file_iterator->name ) ) {

						// Aggiungo il riscontro alla lista
						found.server = &(*server_iterator);
						found.file = &(*file_iterator);

						found_files->push_back( found );

						// Limite dei risultati
						if ( ++size == max ) return size;
					}
				}

				file_iterator++;
			}
		} 

		server_iterator++;
	}

	return size;
}

int Crawler::get_sub_files( const std::string &parent, Server *server, FoundFilesList *found_files ) {

	fprintf( stdout, E_MSG_GET_SUB_FILES, ( server->url + parent ).c_str() );

	// Nuovo riscontro
	FoundFile found;

	// Iteratori
	FilesList::iterator i = server->files_list.begin();
	FilesList::iterator end = server->files_list.end();

	// Numero dei riscontri
	unsigned int size = 0;

	// Imposto la chiave di ricerca
	this->ObjRegExps->set_parent_directory( parent );

	while ( i != end ) {

		// Controllo il nome del file
		if ( this->ObjRegExps->get_ReParentDirectory()->search( i->path ) ) {

			// Aggiungo il riscontro alla lista
			found.server = server;
			found.file = &(*i);

			found_files->push_back( found );

			// Incrementa il numero dei file trovati
			size++;
		}
		i++;
	}

	return size;
}

void Crawler::append_download( const FoundFile &found, DownloadTrack *parent_track = NULL ) {

	/* Nel caso si voglia aggiungere un file */
	if ( found.file->type != FOLDER ) {

		// Creo un tracciatore del file (Foglia)
		DownloadTrack *child_ = new DownloadTrack( true );

		// Imposto i dati del tracker
		child_->info.url = found.server->url + found.file->path;
		child_->info.name = found.file->name;
		child_->info.type = found.file->type;
		child_->info.status = WAIT_STATUS;

		if ( parent_track != NULL ) {

			child_->info.path = parent_track->info.path + FOLDER + found.file->name + '.' + found.file->type;
			child_->ref = this->connect_function( child_, parent_track->ref );

			// Collego il tracker al genitore
			parent_track->append_child( child_ );

		} else {

			child_->info.path = found.file->name + '.' + found.file->type;
			child_->ref = this->connect_function( child_, NULL );
		}

		// Aggiungo il file alla coda degli scaricamenti
		this->ObjSocket->add_download( child_ );

	/* Nel caso si voglia aggiungere una cartella */
	} else {

		// Creo un tracciatore della cartella (Nodo)
		DownloadTrack *child_ = new DownloadTrack( false );

		// Imposto i dati del tracker
		child_->info.url = found.server->url + found.file->path;
		child_->info.name = found.file->name;
		child_->info.type = found.file->type;
		child_->info.status = WAIT_STATUS;

		if ( parent_track != NULL ) {

			child_->info.path = parent_track->info.path + FOLDER + found.file->name;
			child_->ref = this->connect_function( child_, parent_track->ref );

			// Collego il tracker al genitore
			parent_track->append_child( child_ );

		} else {

			child_->info.path = found.file->name;
			child_->ref = this->connect_function( child_, NULL );
		}

		// Creo la nuova cartella
		fprintf( stdout, E_MSG_MAKE_DIRECTORY, child_->info.path.c_str() );
		mkdir( ( DOWNLOADS_DIRECTORY + child_->info.path ).c_str(), S_IRWXU );

		// Ricava i file della cartella
		FoundFilesList found_files;
		if ( !this->get_sub_files( found.file->path, found.server, &found_files ) ) return;

		// Iteratori
		FoundFilesList::iterator i = found_files.begin();
		FoundFilesList::iterator end = found_files.end();

		// Aggiungo i sotto file alla coda dei download
		while ( i != end ) {
	
			// Scarico l'intero albero dei file
			this->append_download( *i, child_ );

			i++;
		}
	}
}

inline void Crawler::pause_download( DownloadTrack *track ) {

	if ( track == NULL ) return;

	// Distinguo i "nodi" dalle "foglie"
	if ( track->is_leaf() ) {

		// Fermo momentaneamente la connessione
		this->ObjSocket->pause_download( track );

	} else {

		// Iteratori
		ptrDownloadTrackList::iterator i = track->children()->begin();
		ptrDownloadTrackList::iterator end = track->children()->end();

		while ( i != end ) {

			// Proseguo l'esplorazione dell'albero
			this->pause_download( *i );
			i++;
		}
	}
}

inline void Crawler::remove_download( DownloadTrack *track ) {

	if ( track == NULL ) return;

	// Distinguo i "nodi" dalle "foglie"
	if ( track->is_leaf() ) {

		// Rimuovo il file dalla coda degli scaricamenti
		this->ObjSocket->remove_download( track );
		// Elimino il tracciatore
		track->destroy();

	} else {

		// Iteratori
		ptrDownloadTrackList::iterator i = track->children()->begin();
		ptrDownloadTrackList::iterator end = track->children()->end();

		while ( i != end ) {

			// Proseguo l'esplorazione dell'albero
			this->remove_download( *i );

			// Reimposto gli iteratori
			i = track->children()->begin();
			end = track->children()->end();
		}

		// Segnalo il cambiamento di stato
		this->ObjSocket->status_function( track, REMOVED_STATUS );
		// Elimino il tracciatore
		track->destroy();
	}
}

inline void Crawler::remove_and_clean_download( DownloadTrack *track ) {

	if ( track == NULL ) return;

	// Distinguo i "nodi" dalle "foglie"
	if ( track->is_leaf() ) {

		// Rimuovo il file dalla coda degli scaricamenti
		this->ObjSocket->remove_download( track );
		// Rimuovo il file dall'hard disk
		fprintf( stdout, E_MSG_REMOVE_FILE, track->info.path.c_str() );
		track->delete_stream();
		// Elimino il tracciatore
		track->destroy();

	} else {

		// Iteratori
		ptrDownloadTrackList::iterator i = track->children()->begin();
		ptrDownloadTrackList::iterator end = track->children()->end();

		while ( i != end ) {

			// Proseguo l'esplorazione dell'albero
			this->remove_and_clean_download( *i );

			// Reimposto gli iteratori
			i = track->children()->begin();
			end = track->children()->end();
		}

		// Segnalo il cambiamento di stato
		this->ObjSocket->status_function( track, REMOVED_STATUS );
		// Rimuovo la cartella dall'hard disk
		fprintf( stdout, E_MSG_REMOVE_DIRECTORY, track->info.path.c_str() );
		track->delete_stream();
		// Elimino il tracciatore
		track->destroy();
	}
}

inline void Crawler::resume_download( DownloadTrack *track ) {

	if ( track == NULL ) return;

	// Distinguo i "nodi" dalle "foglie"
	if ( track->is_leaf() ) {

		// Ripristino la connessione interrotta
		this->ObjSocket->resume_download( track );

	} else {

		// Iteratori
		ptrDownloadTrackList::iterator i = track->children()->begin();
		ptrDownloadTrackList::iterator end = track->children()->end();

		while ( i != end ) {

			// Proseguo l'esplorazione dell'albero
			this->resume_download( *i );
			i++;
		}
	}
}

void Crawler::resume_downloads_from_list( DownloadsList &downloads_list ) {

	std::vector< DownloadTrack * > directories; 
	std::string tmp;
	bool exist;

	// Controllo se esiste già in lista
	DownloadsList::iterator i = downloads_list.begin();
	DownloadsList::iterator end = downloads_list.end();

	// Analizzo e scompongo i percorsi
	while ( i != end ) {

		DownloadTrack *parent_track = NULL;
		std::string path = FOLDER + i->path;
		void *ref = NULL;
		size_t t = 0;

		while ( true ) {

			t = path.find_first_of( FOLDER, t + 1 ); 

			if ( t == string::npos ) {

				// Creo un tracciatore del file (Foglia)
				DownloadTrack *child_ = new DownloadTrack( true );

				// Imposto i dati del tracker
				child_->info.url = i->url;
				child_->info.name = i->name;
				child_->info.type = i->type;
				child_->info.path = i->path;

				if ( i->status != QUEUED_STATUS ) child_->info.status = i->status;
				else child_->info.status = WAIT_STATUS;

				if ( parent_track != NULL ) {

					child_->ref = this->connect_function( child_, parent_track->ref );

					// Collego il tracker al genitore
					parent_track->append_child( child_ );

				} else child_->ref = this->connect_function( child_, NULL );

				// Apro lo stream al file di destinazione in modalità "append"
				child_->open_stream( true );

				// Aggiungo il download alla coda
				this->ObjSocket->add_download( child_ );
				break;
			}

			// Ricavo il nuovo percorso
			tmp = path.substr( 0, t );

			// Controllo se la cartella è già stata connessa
			exist = false;

			for( int n = 0; n < directories.size(); n++ )

				if ( directories[n]->info.path == tmp ) {

					// Cambio la cartella madre corrente
					parent_track = directories[n];
					exist = true;
					break;
				}

			if ( !exist ) {

				// Creo una tracciatore della nuova cartella (Nodo)
				DownloadTrack *child_ = new DownloadTrack( false );

				// Imposto i dati del tracker
				child_->info.url = i->url;
				child_->info.type = FOLDER;
				child_->info.path = tmp;
				child_->info.status = WAIT_STATUS;

				if ( parent_track != NULL ) {

					child_->info.name = tmp.substr( parent_track->info.path.size() + 1 );
					child_->ref = this->connect_function( child_, parent_track->ref );

					// Collego il tracker al genitore
					parent_track->append_child( child_ );

				} else {

					child_->info.name = tmp.substr( 1 );
					child_->ref = this->connect_function( child_, NULL );
				}

				// Cambio la cartella madre con la corrente
				parent_track = child_;

				// Memorizzo la cartella già esplorata
				directories.push_back( parent_track );
			}
		}

		i++;
	}
}

DownloadsList Crawler::get_downloads_list() {

	// Lista vuota dei download
	DownloadsList download_list;
	// Struttura del download
	DownloadInfo download;
		
	// Tracciatori delle connessioni
	ptrDownloadTrackList *sockets_list = this->ObjSocket->get_sockets_list();

	// Iteratori
	ptrDownloadTrackList::iterator i = sockets_list->begin();
	ptrDownloadTrackList::iterator end = sockets_list->end();

	// Genero la lista dei download
	while ( i != end ) {

		download = (*i)->info;
		if ( download.status == QUEUED_STATUS ) download.status = WAIT_STATUS;
		download_list.push_back( download );
		i++;
	}

	return download_list;
}

} // namespace Engine

#endif
