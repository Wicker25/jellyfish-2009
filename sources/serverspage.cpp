/* 
	Title: Jellyfish - serverspage.cpp
	Author: "Giacomo Trudu" aka "Wicker25" - wicker25[at]gmail[dot]com
	Site: https://github.com/Wicker25
	License: (CC) BY-NC-SA - http://creativecommons.org/licenses/by-nc-sa/2.5/it/
	Description: Jellyfish is a free download application
*/

#ifndef SERVERSPAGE_CPP
#define SERVERSPAGE_CPP

template < class T_MainWindow > 
ServersPage< T_MainWindow >::ServersPage( T_MainWindow *ObjMainWindow ) {

	// Puntatore alla finestra principale
	this->ObjMainWindow = ObjMainWindow;

	// Inizializzo le variabili di controllo
	this->thread_is_running = false;

	// Creo il contenitore principale
	Gtk::VBox *vbox = new Gtk::VBox;
	this->pack_start( *vbox, true, true, 2 );

	// Creo la barra d'inserimento con relativo pulsante
	this->make_entry_bar( vbox );

	// Creo il contenitore del visualizzatore
	Gtk::HBox *hbox = new Gtk::HBox;
	vbox->pack_start( *hbox, true, true, 5 );

	// Creo il visualizzatore
	Gtk::ScrolledWindow *scrolled_window = new Gtk::ScrolledWindow;
	scrolled_window->set_policy( Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC );
	scrolled_window->set_shadow_type( Gtk::SHADOW_IN );

	this->servers_viewer = new ServersViewer< ServersPage >( this );

	scrolled_window->add( *this->servers_viewer );
	hbox->pack_start( *scrolled_window, true, true, 5 );

	// Configuro il menu contestuale
	this->make_context_menu();

	// Mostra la pagina dei server
	this->show_all();
}

template < class T_MainWindow > 
ServersPage< T_MainWindow >::~ServersPage() {

}

template < class T_MainWindow > 
inline void ServersPage< T_MainWindow >::make_entry_bar( Gtk::VBox *vbox ) {

	// Puntatore ad un oggetto immagine
	Gtk::Image *image;

	// Creo il contenitore della barra d'inserimento
	Gtk::HBox *hbox = new Gtk::HBox;
	vbox->pack_start( *hbox, false, true, 0 );

	// Creo la barra d'inserimento
	this->entry = new Gtk::Entry;
	this->entry->set_max_length( MAX_LENGTH_SERVER );
	this->entry->signal_activate().connect( sigc::mem_fun( *this, &ServersPage::add_new_server ) );
	hbox->pack_start( *this->entry, true, true, 3 );

	// Creo il pulsante
	Gtk::Button *add_button = new Gtk::Button( Gtk::Stock::ADD );
	add_button->signal_clicked().connect( sigc::mem_fun( *this, &ServersPage::add_new_server ) );
	hbox->pack_start( *add_button, false, true, 3 );
}

template < class T_MainWindow > 
inline void ServersPage< T_MainWindow >::make_context_menu() {

	// Configuro il menu contestuale
	Gtk::Menu *context_menu = this->servers_viewer->get_context_menu();
	context_menu->accelerate( *this->ObjMainWindow );

	// Nuova immagine
	Gtk::Image *image;

	// Aggiungo le voci al menu
	Gtk::Menu::MenuList *menulist = &context_menu->items();

	// Aggiungo la voce "Ping"
	image = new Gtk::Image( Gtk::Stock::CONNECT, Gtk::ICON_SIZE_MENU );
	menulist->push_back( Gtk::Menu_Helpers::ImageMenuElem( ACTION_PING, *image,
		sigc::mem_fun( *this, &ServersPage::ping_servers ) ) );
	// Aggiungo la voce "Indicizza"
	image = new Gtk::Image( Gtk::Stock::FIND, Gtk::ICON_SIZE_MENU );
	menulist->push_back( Gtk::Menu_Helpers::ImageMenuElem( ACTION_INDEX, *image,
		sigc::mem_fun( *this, &ServersPage::index_files ) ) );
	// Aggiungo un separatore orizzontale
	menulist->push_back( Gtk::Menu_Helpers::SeparatorElem() );
	// Aggiungo la voce "Rimuovi"
	menulist->push_back( Gtk::Menu_Helpers::StockMenuElem( Gtk::Stock::REMOVE,
		sigc::mem_fun( *this, &ServersPage::remove_selected_servers ) ) );
}

template < class T_MainWindow > 
void ServersPage< T_MainWindow >::set_servers( JE::DataList *servers_list ) {

	// Pulisco il visualizzatore
	gdk_threads_enter();
	this->servers_viewer->clear_list();
	gdk_threads_leave();

	// Iteratori
	JE::DataList::iterator i = servers_list->begin();
	JE::DataList::iterator end = servers_list->end();

	// Aggiungo i server al visualizzatore
	while ( i != end ) {

		gdk_threads_enter();
		this->servers_viewer->append_server( *i );
		gdk_threads_leave();

		i++;
	}
}

template < class T_MainWindow > 
void ServersPage< T_MainWindow >::add_new_server_thread( const std::string &hoster, JE::EngineCode *code ) {

	this->thread_is_running = true;

	// Puntatore al nuovo server nella lista
	JE::Server *new_server;

	// Acquisco le informazioni sul nuovo hoster
	*code = this->ObjMainWindow->get_ObjEngine()->add_new_hoster( hoster, &new_server );

	// Controllo se il server e' gia' in lista
	if ( *code == JE::ENGINE_DONE ) {

		// Aggiungo il server al visualizzatore
		gdk_threads_enter();
		this->servers_viewer->append_server( *new_server, true );
		gdk_threads_leave();

		// Salvo gli indici
		this->ObjMainWindow->get_ObjEngine()->write_data_list();

	} else if ( *code == JE::ENGINE_SERVER_EXIST ) {

		// Mostro il server nel visualizzatore
		gdk_threads_enter();

		// Mostro un messaggio d'errore
		Gtk::MessageDialog error_dialog( *this->ObjMainWindow, DLG_DUPLICATE_SERVER_ERROR_TITLE,
												false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_CLOSE );
		error_dialog.set_secondary_text( DLG_DUPLICATE_SERVER_ERROR_BODY );
		error_dialog.run();

		this->servers_viewer->scroll_to_server( new_server->url );
		gdk_threads_leave();
	}

	// Torno allo stato normale
	this->ObjMainWindow->get_status_bar()->stop_work( this->ObjMainWindow );
	this->thread_is_running = false;
}

template < class T_MainWindow > 
void ServersPage< T_MainWindow >::add_new_server() {

	// Estraggo l'indirizzo
	std::string new_hoster = this->entry->get_text();

	// Controllo che non sia vuoto
	if ( !new_hoster.empty() ) {

		if ( this->ObjMainWindow->get_ObjEngine()->validate_url( new_hoster ) ) {

			// Svuoto la barra d'inserimento
			this->entry->set_text( "" ); 

			// Codice di controllo
			JE::EngineCode code;

			// Preparo il messaggio della barra di stato
			string msg = MSG_INDEX_FILES + new_hoster + MSG_WAIT;

			// Avvio il thread per l'aggiunta di un nuovo server
			this->ObjMainWindow->get_status_bar()->set_work_status( msg, this->ObjMainWindow );
			Glib::Thread *thread = Glib::Thread::create( 
						sigc::bind( sigc::mem_fun( *this, &ServersPage::add_new_server_thread ), new_hoster, &code ), false );


		} else {

			// Mostro un messaggio d'errore
			Gtk::MessageDialog error_dialog( *this->ObjMainWindow, DLG_URL_ERROR_TITLE,
											false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_CLOSE );
			error_dialog.set_secondary_text( DLG_URL_ERROR_BODY );
			error_dialog.run();
		}
	}
}

template < class T_MainWindow > 
void ServersPage< T_MainWindow >::enable_server_thread( JE::Server *server, bool enable ) {

	this->thread_is_running = true;

	// Abilita/Disabilita un server
	this->ObjMainWindow->get_ObjEngine()->enable_server( server, enable );

	// Torno allo stato normale
	this->ObjMainWindow->get_status_bar()->stop_work( this->ObjMainWindow, false );
	this->thread_is_running = false;
}

template < class T_MainWindow > 
void ServersPage< T_MainWindow >::processing_thread( PROCESSING_MODE mode ) {

	this->thread_is_running = true;

	// Riferimenti alle voci da modificare
	std::vector< Gtk::TreeModel::Path > paths;
	std::vector< Gtk::TreeModel::Row > rows;

	// Ricavo le voci selezionate nel visualizzatore
	gdk_threads_enter();
	paths = this->servers_viewer->get_selection()->get_selected_rows();
	gdk_threads_leave();

	// Ricavo i riferimenti dei server selezionati
	while ( !paths.empty() ) {

		gdk_threads_enter();
		rows.push_back( *this->servers_viewer->get_model()->get_iter( paths.back() ) );
		gdk_threads_leave();
		paths.pop_back();
	}

	// Struttura del server da modificare
	JE::Server *server;
	// Messaggio nella barra del progresso
	string msg;

	for( int i = rows.size() - 1; i >= 0 ; i-- ) {

		// Ricavo la struttura del server selezionato
		server = this->servers_viewer->get_server_ref( rows[i] );

		// Aggiorno il messaggio della barra del progresso
		if ( mode == PING_MODE ) msg = MSG_PING_HOSTER;
		else if ( mode == INDEX_MODE ) msg = MSG_INDEX_FILES;
		else if ( mode == REMOVE_MODE ) msg = MSG_REMOVE_HOSTER;
		msg += server->url + MSG_WAIT;

		gdk_threads_enter();
		this->ObjMainWindow->get_status_bar()->change_msg( msg );
		gdk_threads_leave();

		if ( mode == PING_MODE ) {

			// Aggiorno il valore del ping
			this->ObjMainWindow->get_ObjEngine()->new_ping_hoster( server );

		} else if ( mode == INDEX_MODE ) {

			// Aggiorno gli indici di un server
			this->ObjMainWindow->get_ObjEngine()->new_index_files( server );

		} else if ( mode == REMOVE_MODE ) {

			// Rimuovo il server dalla lista
			this->ObjMainWindow->get_ObjEngine()->remove_hoster( server->url );
			// Rimuovo il server dal visualizzatore
			gdk_threads_enter();
			this->servers_viewer->remove_server( rows[i] );
			gdk_threads_leave();
			break;
		}

		// Aggiorno la voce nel visualizzatore
		gdk_threads_enter();
		this->servers_viewer->set_server( rows[i], *server );
		gdk_threads_leave();
	}

	if ( mode == PING_MODE ) {

		// Salvo le modifiche
		this->ObjMainWindow->get_ObjEngine()->write_servers_list();

	} else if ( mode == INDEX_MODE ) {

		// Salvo le modifiche
		this->ObjMainWindow->get_ObjEngine()->write_files_list();

	} else if ( mode == REMOVE_MODE ) {

		// Salvo le modifiche
		this->ObjMainWindow->get_ObjEngine()->write_data_list();
	}

	this->ObjMainWindow->get_pages_box()->get_files_page()->clear_founds();

	// Torno allo stato normale
	this->ObjMainWindow->get_status_bar()->stop_work( this->ObjMainWindow );
	this->thread_is_running = false;
}

template < class T_MainWindow > 
bool ServersPage< T_MainWindow >::enable_server( JE::Server *server, bool enable ) {

	// Avvio il thread per l'abilitazione/disabilitazione di un server
	if ( this->thread_is_running ) return false;
	this->ObjMainWindow->get_status_bar()->set_work_status( MSG_PROCESSING, this->ObjMainWindow, false );
	Glib::Thread *thread = Glib::Thread::create( 
					sigc::bind( sigc::mem_fun( *this, &ServersPage::enable_server_thread ), server, enable ), false );
	return true;
}

template < class T_MainWindow > 
void ServersPage< T_MainWindow >::ping_servers() {

	// Avvio il thread per l'aggiornamento del valore del ping
	if ( this->thread_is_running ) return;
	this->ObjMainWindow->get_status_bar()->set_work_status( MSG_PROCESSING, this->ObjMainWindow );
	Glib::Thread *thread = Glib::Thread::create( 
					sigc::bind( sigc::mem_fun( *this, &ServersPage::processing_thread ), PING_MODE ), false );
}

template < class T_MainWindow > 
void ServersPage< T_MainWindow >::index_files() {

	// Avvio il thread per l'aggiornamento dei file indicizzati
	if ( this->thread_is_running ) return;
	this->ObjMainWindow->get_status_bar()->set_work_status( MSG_PROCESSING, this->ObjMainWindow );
	Glib::Thread *thread = Glib::Thread::create( 
					sigc::bind( sigc::mem_fun( *this, &ServersPage::processing_thread ), INDEX_MODE ), false );
}

template < class T_MainWindow > 
void ServersPage< T_MainWindow >::remove_selected_servers() {

	// Avvio il thread per la rimozione dei server selezionati
	if ( this->thread_is_running ) return;
	this->ObjMainWindow->get_status_bar()->set_work_status( MSG_PROCESSING, this->ObjMainWindow );
	Glib::Thread *thread = Glib::Thread::create( 
					sigc::bind( sigc::mem_fun( *this, &ServersPage::processing_thread ), REMOVE_MODE ), false );
}

#endif
