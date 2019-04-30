/*
	Title: Jellyfish - engine.cpp
	Author: "Giacomo Trudu" aka "Wicker25" - wicker25[at]gmail[dot]com
	Site: http://www.hackyourmind.org/
	License: (CC) BY-NC-SA - http://creativecommons.org/licenses/by-nc-sa/2.5/it/
	Description: Jellyfish is a free download application
*/

#ifndef ENGINE_CPP
#define ENGINE_CPP

namespace JE {

using namespace std;

Engine::Engine( int argc, char *argv[] ) {

	// Funzioni da riga di comando
	if ( argc > 1 && argc < 3 ) {

		// Controllo gli argomenti passati
		if ( !strcmp( argv[1], CMD_HELP ) || !strcmp( argv[1], CMD_H ) ) {

			fprintf( stdout, E_HELP_SHOW_LOGO, APP_VERSION );
			fprintf( stdout, E_HELP_SHOW_DESCRIPTION, APP_DESCRIPTION );
			fprintf( stdout, E_HELP_SHOW_USAGE, argv[0] );

			fprintf( stdout, E_HELP_SHOW_HEADER_METHODS );

			fprintf( stdout, E_HELP_VERSION, CMD_V, CMD_VERSION );
			fprintf( stdout, E_HELP, CMD_H, CMD_HELP );

			fprintf( stdout, E_HELP_SHOW_AUTHOR, APP_AUTHOR );

		} else if ( !strcmp( argv[1], CMD_VERSION ) || !strcmp( argv[1], CMD_V ) ) {

			fprintf( stdout, E_HELP_SHOW_VERSION, APP_TITLE, APP_VERSION );

		} else {

			fprintf( stdout, E_HELP_SHOW_USAGE, argv[0] );
			fprintf( stdout, E_HELP_SHOW_TRY, CMD_HELP );
		}

		exit(0);

	} else if ( argc > 2 ) {
	
		fprintf( stdout, E_HELP_SHOW_USAGE, argv[0] );
		fprintf( stdout, E_HELP_SHOW_TRY, CMD_HELP );
		exit(0);
	}

	// Inizializzo le strutture
	this->data_list = new DataList;
	this->found_files = new FoundFilesList;

	// Inizializzo i gestori
	this->ObjRegExps = new RegExps;
	this->ObjSocket = new Socket;
	this->ObjStreams = new Streams( this );
	this->ObjCrawler = new Crawler( this );

	// Collego la funzioni di stato per aggiornare il file dei download
	this->ObjSocket->status_function.connect( sigc::mem_fun( *this, &Engine::status_function ) );

	// Controllo l'integrità delle cartelle
	this->ObjStreams->check_directory();
}

Engine::~Engine() {

}

void Engine::progress_function( DownloadTrack *track, double dlnow, double dltotal, double speed, bool complete ) {

	fprintf( stdout, "%p : scaricati %.2f di %.2f Kb, alla velocita' di %.2f Kb/s\n", track, dlnow, dltotal, speed );
}

void Engine::status_function( DownloadTrack *track, DownloadStatus status ) {

	// Salvo i download
	this->write_downloads_list();
}

inline void Engine::write_settings() {

	// Scrive le configurazioni dell'Engine
	fprintf( stdout, E_MSG_WRITE_SETTINGS );
	this->ObjStreams->write_settings();
}

void Engine::read_data_list() {

	// Pulisco i dati relativi ai server e ai file
	this->clear_data_list();

	fprintf( stdout, E_MSG_READ_SETTINGS );
	this->ObjStreams->read_settings();
	// Carico i dati relativi ai server
	fprintf( stdout, E_MSG_READ_SERVERS_LIST );
	this->ObjStreams->read_servers_list();
	// Carico i dati relativi ai file
	fprintf( stdout, E_MSG_READ_FILES_LIST );
	this->ObjStreams->read_files_list();
}

inline void Engine::write_data_list() {

	// Salvo i dati relativi ai server
	this->write_servers_list();
	// Salvo i dati relativi ai file
	this->write_files_list();
}

inline void Engine::clear_data_list() {

	// Pulisco i dati sui server ed i file indicizzati
	this->data_list->clear();
}

void Engine::read_downloads_list( DownloadsList &downloads_list, bool resume = true ) {

	// Carico i dati relativi ai download
	fprintf( stdout, E_MSG_READ_DOWNLOADS_LIST );
	this->ObjStreams->read_downloads_list( downloads_list );

	if ( !resume ) return;

	// Ripristina i download sospesi
	this->ObjCrawler->resume_downloads_from_list( downloads_list );
}

inline void Engine::write_downloads_list() {

	// Salvo i download
	DownloadsList downloads_list = this->ObjCrawler->get_downloads_list();
	this->ObjStreams->write_downloads_list( downloads_list );
}

inline void Engine::set_opt( OPTION option, long int value ) {

	// Modifico il valore specificato
	switch ( option ) {

		case SOCKET_CONNECT_TIMEOUT_OPTION:
			E_SOCKET_CONNECT_TIMEOUT = value; break;

		case SOCKET_TIMEOUT_OPTION:
			E_SOCKET_TIMEOUT = value; break;

		case SOCKET_REPEAT_OPTION:
			E_SOCKET_REPEAT = value; break;

		case SOCKET_WAIT_REPEAT_OPTION:
			E_SOCKET_WAIT_REPEAT = value; break;

		case SOCKET_WAIT_READ_OPTION:
			E_SOCKET_WAIT_READ = value; break;
			
		case SOCKET_PROXY_PORT_OPTION:
			E_SOCKET_PROXY_PORT = value; break;

		case MAX_SPIDERS_OPTION:
			E_MAX_SPIDERS = value; break;

		case MAX_DOWNLOADS_OPTION:
			E_MAX_DOWNLOADS = value; break;

		case FOUNDS_LIMIT_OPTION:
			E_FOUNDS_LIMIT = value; break;
	}
}

inline void Engine::set_opt( OPTION option, std::string value ) {

	// Modifico il valore specificato
	switch ( option ) {

		case SOCKET_PROXY_OPTION:
			E_SOCKET_PROXY = value; break;

		case FILE_MANAGER_OPTION:
			E_FILE_MANAGER = value; break;
	}
}
inline void *Engine::get_opt( OPTION option ) {

	// Ritorno il valore specificato
	switch ( option ) {

		case SOCKET_CONNECT_TIMEOUT_OPTION:
			return &E_SOCKET_CONNECT_TIMEOUT;

		case SOCKET_TIMEOUT_OPTION:
			return &E_SOCKET_TIMEOUT;

		case SOCKET_REPEAT_OPTION:
			return &E_SOCKET_REPEAT;

		case SOCKET_WAIT_REPEAT_OPTION:
			return &E_SOCKET_WAIT_REPEAT;

		case SOCKET_WAIT_READ_OPTION:
			return &E_SOCKET_WAIT_READ;

		case SOCKET_PROXY_OPTION:
			return &E_SOCKET_PROXY;
			
		case SOCKET_PROXY_PORT_OPTION:
			return &E_SOCKET_PROXY_PORT;

		case MAX_SPIDERS_OPTION:
			return &E_MAX_SPIDERS;

		case MAX_DOWNLOADS_OPTION:
			return &E_MAX_DOWNLOADS;

		case FILE_MANAGER_OPTION:
			return &E_FILE_MANAGER;

		case FOUNDS_LIMIT_OPTION:
			return &E_FOUNDS_LIMIT;
	}
}

EngineCode Engine::hoster_exist( const std::string &hoster, Server **server ) {

	// Iteratori
	DataList::iterator i = this->data_list->begin();
	DataList::iterator end = this->data_list->end();

	// Controllo se il server e' gia' in lista
	while ( i != end ) {

		if ( i->url == hoster ) {

			(*server) = &(*i);
			return ENGINE_SERVER_EXIST;
		}
		i++;
	}

	return ENGINE_DONE;
}

EngineCode Engine::add_new_hoster( const std::string &hoster, Server **new_server ) {

	// Controlla se il server esiste gia'
	if ( this->hoster_exist( hoster, new_server ) == ENGINE_SERVER_EXIST )
		return ENGINE_SERVER_EXIST;

	// Creo un nuovo server
	Server empty_server;
	this->data_list->push_back( empty_server );
	(*new_server) = &this->data_list->back();

	// Copio le informazioni sul server
	(*new_server)->url = hoster;
	(*new_server)->enabled = true;

	// Effettuo il ping al server
	(*new_server)->ping = this->ObjCrawler->ping( hoster ); 

	// Indicizzo i file
	unsigned int hoster_lenght = hoster.size();
	this->ObjCrawler->start_index_files( hoster, &(*new_server)->files_list, hoster_lenght );

	return ENGINE_DONE;
}

void Engine::new_ping_hoster( Server *server ) {

	// Aggiorno il valore del ping
	server->ping = this->ObjCrawler->ping( server->url );
}

void Engine::new_index_files( Server *hoster ) {

	// Aggiorno i file indicizzati di un server
	hoster->files_list.clear();
	unsigned int hoster_lenght = hoster->url.size();
	this->ObjCrawler->start_index_files( hoster->url, &hoster->files_list, hoster_lenght );
}

bool Engine::validate_url( const std::string &hoster ) {

	// Aggiorno i file indicizzati di un server
	if ( this->ObjRegExps->get_ReHost()->search( hoster ) ) return true;
	else return false;
}

void Engine::enable_server( Server *server, bool enabled ) {

	// Aggiorno il valore del ping
	server->enabled = enabled;

	// Salvo le nuove informazioni sui server
	this->ObjStreams->write_servers_list();
}

void Engine::remove_hoster( const std::string &hoster ) {

	// Iteratori
	DataList::iterator i = this->data_list->begin();
	DataList::iterator end = this->data_list->end();

	// Rimuovo il server (e i suoi file) dalla lista
	while ( i != end ) {

		if ( i->url == hoster ) {

			this->data_list->erase(i);
			break;
		}
		i++;
	}
}

EngineCode Engine::find_file( const std::string &key, FileType type = OTHER_TYPE, unsigned int max = E_FOUNDS_LIMIT ) {

	// Pulisco i vecchi riscontri
	this->found_files->clear();
	// Cerco un file indicizzato
	this->ObjCrawler->find_file( key, this->data_list, this->found_files, type );

	return ENGINE_DONE;
}

EngineCode Engine::get_sub_files( FoundFile &found ) {

	// Pulisco i vecchi riscontri
	this->found_files->clear();
	// Cerco un file indicizzato
	this->ObjCrawler->get_sub_files( found.file->path, found.server, this->found_files );

	return ENGINE_DONE;
}

FileType Engine::get_file_type( const std::string &type ) {

	// Controllo il tipo del file
	if ( type == FOLDER ) return FOLDER_TYPE;
	if ( this->ObjRegExps->get_ReAudioTypes()->search( type ) )			return AUDIO_TYPE;
	if ( this->ObjRegExps->get_ReVideoTypes()->search( type ) )			return VIDEO_TYPE;
	if ( this->ObjRegExps->get_ReImageTypes()->search( type ) )			return IMAGE_TYPE;
	if ( this->ObjRegExps->get_ReDocumentTypes()->search( type ) )		return DOCUMENT_TYPE;
	if ( this->ObjRegExps->get_RePackageTypes()->search( type ) )		return PACKAGE_TYPE;
	if ( this->ObjRegExps->get_ReExecutableTypes()->search( type ) )	return EXECUTABLE_TYPE;

	return OTHER_TYPE;
}

void Engine::append_download( const FoundFile &found ) {

	// Aggiungo un nuovo file alla coda degli scaricamenti
	this->ObjCrawler->append_download( found );
}

void Engine::pause_download( DownloadTrack *track ) {

	// Fermo momentaneamente la connessione
	this->ObjCrawler->pause_download( track );

	// Salvo i download
	this->write_downloads_list();
}

void Engine::resume_download( DownloadTrack *track ) {

	// Ripristino la connessione interrotta
	this->ObjCrawler->resume_download( track );
}

void Engine::remove_download( DownloadTrack *track ) {

	// Rimuovo il file dalla coda degli scaricamenti
	this->ObjCrawler->remove_download( track );
}

void Engine::remove_and_clean_download( DownloadTrack *track ) {

	// Rimuovo il file dalla coda degli scaricamenti
	this->ObjCrawler->remove_and_clean_download( track );
}

void Engine::open_downloaded_file( DownloadTrack *track ) {

	// Apro il file scaricato
	if ( !E_FILE_MANAGER.empty() )
		system( ( E_FILE_MANAGER + " \"" + DOWNLOADS_DIRECTORY + track->info.path + '"' ).c_str() );
}


} // namespace JE

#endif
