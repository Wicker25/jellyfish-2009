/* 
	Title: Jellyfish - mainwindow.cpp
	Author: "Giacomo Trudu" aka "Wicker25" - wicker25[at]gmail[dot]com
	Site: https://github.com/Wicker25
	License: (CC) BY-NC-SA - http://creativecommons.org/licenses/by-nc-sa/2.5/it/
	Description: Jellyfish is a free download application
*/

#ifndef MAINWINDOW_CPP
#define MAINWINDOW_CPP

MainWindow::MainWindow( JE::Engine *ObjEngine ) {

	/** Impongo l'accesso esclusivo alle GTK **/
	gdk_threads_enter();

	// Collego il motore di JellyFish
	this->ObjEngine = ObjEngine;

	// Imposto le caratteristiche della finestra
	this->set_title( WINDOW_TITLE );
	this->set_default_size( 800, 480 );

	// Imposto l'icona della finestra
	this->set_icon( Gdk::Pixbuf::create_from_file( APP_ICON ) );

	// Creo le icone
	this->icon_folder		= Gdk::Pixbuf::create_from_file( ICON_FOLDER );
	this->icon_audio		= Gdk::Pixbuf::create_from_file( ICON_AUDIO );
	this->icon_video		= Gdk::Pixbuf::create_from_file( ICON_VIDEO );
	this->icon_image 		= Gdk::Pixbuf::create_from_file( ICON_IMAGE );
	this->icon_document		= Gdk::Pixbuf::create_from_file( ICON_DOCUMENT );
	this->icon_package		= Gdk::Pixbuf::create_from_file( ICON_PACKAGE );
	this->icon_executable	= Gdk::Pixbuf::create_from_file( ICON_EXECUTABLE );
	this->icon_other		= Gdk::Pixbuf::create_from_file( ICON_OTHER );

	this->icon_wait			= Gdk::Pixbuf::create_from_file( ICON_WAIT );
	this->icon_start		= Gdk::Pixbuf::create_from_file( ICON_START );
	this->icon_pause		= Gdk::Pixbuf::create_from_file( ICON_PAUSE );
	this->icon_stop			= Gdk::Pixbuf::create_from_file( ICON_STOP );
	this->icon_interrupt	= Gdk::Pixbuf::create_from_file( ICON_INTERRUPT );
	this->icon_queue		= Gdk::Pixbuf::create_from_file( ICON_QUEUE );

	// Creo il contenitore principale
	Gtk::VBox *vbox = new Gtk::VBox;
	this->add( *vbox );

	// Creo la barra dei menu
	MenuBar *menu_bar = new MenuBar;
	vbox->pack_start( *menu_bar, false, 0 );

	// Collego le funzioni del menu 
	menu_bar->get_preferences_button()->signal_activate().connect(	sigc::mem_fun( *this, &MainWindow::show_preferences_dialog ) );
	menu_bar->get_exit_button()->signal_activate().connect(	sigc::ptr_fun( &Gtk::Main::quit ) );

	menu_bar->get_selectall_button()->signal_activate().connect(
					sigc::bind( sigc::mem_fun( *this, &MainWindow::generic_functions ), SELECTALL_TYPE ) );
	menu_bar->get_unselectall_button()->signal_activate().connect(
					sigc::bind( sigc::mem_fun( *this, &MainWindow::generic_functions ), UNSELECTALL_TYPE ) );

	menu_bar->get_about_button()->signal_activate().connect( sigc::mem_fun( *this, &MainWindow::show_about_dialog ) );

	// Creo la barra degli strumenti
	Toolbar *toolbar = new Toolbar;
	vbox->pack_start( *toolbar, false, 0 );

	// Collego i pulsanti della barra degli strumenti
	toolbar->get_servers_tool()->signal_clicked().connect(
					sigc::bind( sigc::mem_fun( *this, &MainWindow::set_page ), SERVERS_PAGE ) );
	toolbar->get_search_tool()->signal_clicked().connect(
					sigc::bind( sigc::mem_fun( *this, &MainWindow::set_page ), SEARCH_PAGE ) );
	toolbar->get_downloads_tool()->signal_clicked().connect(
					sigc::bind( sigc::mem_fun( *this, &MainWindow::set_page ), DOWNLOADS_PAGE ) );

	// Creo il contenitore delle pagine
	this->pages_box = new PagesBox< MainWindow >( this );
	vbox->pack_start( *this->pages_box, true, true, 0 );

	// Creo la barra di stato
	this->status_bar = new Statusbar;
	vbox->pack_start( *this->status_bar, false, true, 0 );

	// Mostro la finestra ed i suoi widgets
	this->show_all();

	/** Rimuovo l'accesso esclusivo alle GTK **/
	gdk_threads_leave();

	// Carico i dati relativi ai server e ai file
	this->load_data_list();
}

MainWindow::~MainWindow() {

}

void MainWindow::set_page( PAGES page ) {

	// Cambia la pagina del programma
	this->pages_box->set_current_page( page );
}

void MainWindow::show_preferences_dialog() {

	// Mostro la finestra delle preferenze
	PreferencesDialog dialog( *this );

	// Imposto i valori attuali
	long int *value = (long int *) this->ObjEngine->get_opt( JE::SOCKET_CONNECT_TIMEOUT_OPTION ); // da millisecondi a secondi
	dialog.set_socket_connect_timeout( value );
	value = (long int *) this->ObjEngine->get_opt( JE::SOCKET_TIMEOUT_OPTION ); // da millisecondi a minuti
	dialog.set_socket_timeout( value );
	value = (long int *) this->ObjEngine->get_opt( JE::SOCKET_REPEAT_OPTION );
	dialog.set_socket_repeat( value );
	value = (long int *) this->ObjEngine->get_opt( JE::SOCKET_WAIT_REPEAT_OPTION ); // da microsecondi a secondi
	dialog.set_socket_wait_repeat( value );
	value = (long int *) this->ObjEngine->get_opt( JE::SOCKET_WAIT_READ_OPTION ); // da microsecondi a millisecondi
	dialog.set_socket_wait_read( value );
	std::string *text = (std::string *) this->ObjEngine->get_opt( JE::SOCKET_PROXY_OPTION );
	dialog.set_socket_proxy( text );
	value = (long int *) this->ObjEngine->get_opt( JE::SOCKET_PROXY_PORT_OPTION );
	dialog.set_socket_proxy_port( value );
	value = (long int *) this->ObjEngine->get_opt( JE::MAX_SPIDERS_OPTION );
	dialog.set_max_spiders( value );
	value = (long int *) this->ObjEngine->get_opt( JE::MAX_DOWNLOADS_OPTION );
	dialog.set_max_downloads( value );
	text = (std::string *) this->ObjEngine->get_opt( JE::FILE_MANAGER_OPTION );
	dialog.set_file_manager( text );
	value = (long int *) this->ObjEngine->get_opt( JE::FOUNDS_LIMIT_OPTION );
	dialog.set_founds_limit( value );

	if ( !dialog.run() ) {

		// Applico i cambiamenti all'engine
		this->ObjEngine->set_opt( JE::SOCKET_CONNECT_TIMEOUT_OPTION, dialog.get_socket_connect_timeout() );
		this->ObjEngine->set_opt( JE::SOCKET_TIMEOUT_OPTION, dialog.get_socket_timeout() );
		this->ObjEngine->set_opt( JE::SOCKET_REPEAT_OPTION, dialog.get_socket_repeat() );
		this->ObjEngine->set_opt( JE::SOCKET_WAIT_REPEAT_OPTION, dialog.get_socket_wait_repeat() );
		this->ObjEngine->set_opt( JE::SOCKET_WAIT_READ_OPTION, dialog.get_socket_wait_read() );
		this->ObjEngine->set_opt( JE::SOCKET_PROXY_OPTION, dialog.get_socket_proxy() );
		this->ObjEngine->set_opt( JE::SOCKET_PROXY_PORT_OPTION, dialog.get_socket_proxy_port() );
		this->ObjEngine->set_opt( JE::MAX_SPIDERS_OPTION, dialog.get_max_spiders() );
		this->ObjEngine->set_opt( JE::MAX_DOWNLOADS_OPTION, dialog.get_max_downloads() );
		this->ObjEngine->set_opt( JE::FILE_MANAGER_OPTION, dialog.get_file_manager() );
		this->ObjEngine->set_opt( JE::FOUNDS_LIMIT_OPTION, dialog.get_founds_limit() );

		this->ObjEngine->write_settings();
	}
}

void MainWindow::show_about_dialog() {

	// Mostro la finestra delle informazioni
	AboutDialog dialog( *this );
	dialog.run();
}

void MainWindow::generic_functions( FUNCTION_TYPE mode ) {

	// Eseguo una funzione generica
	switch ( this->pages_box->get_current_page() ) {

		// Pagina dei server
		case SERVERS_PAGE: {

			switch ( mode ) {

				// Seleziono tutto
				case SELECTALL_TYPE: {
					this->pages_box->get_servers_page()->get_servers_viewer()->select_all(); break; }

				// Deseleziono tutto
				case UNSELECTALL_TYPE: {
					this->pages_box->get_servers_page()->get_servers_viewer()->unselect_all(); break; }
			}
			break;
		}

		// Pagina dei server
		case SEARCH_PAGE: {

			switch ( mode ) {

				// Seleziono tutto
				case SELECTALL_TYPE: {
					this->pages_box->get_files_page()->get_files_viewer()->select_all(); break; }

				// Deseleziono tutto
				case UNSELECTALL_TYPE: {
					this->pages_box->get_files_page()->get_files_viewer()->unselect_all(); break; }
			}
			break;
		}

		// Pagina dei download
		case DOWNLOADS_PAGE: {

			switch ( mode ) {

				// Seleziono tutto
				case SELECTALL_TYPE: {
					this->pages_box->get_downloads_page()->get_downloads_viewer()->select_all(); break; }

				// Deseleziono tutto
				case UNSELECTALL_TYPE: {
					this->pages_box->get_downloads_page()->get_downloads_viewer()->unselect_all(); break; }
			}
			break;
		}
	}
}

void MainWindow::processing_data_list( PROCESSING_MODE mode ) {

	if ( mode == LOAD_MODE ) {

		// Carico/Salvo i dati relativi ai server e ai file
		this->ObjEngine->read_data_list();

		// Aggiunge i server al visualizzatore
		this->pages_box->get_servers_page()->set_servers( this->ObjEngine->get_data_list() );

		// Ripristino i download sospesi
		JE::DownloadsList downloads_list;
		this->ObjEngine->read_downloads_list( downloads_list );

	} else if ( mode == SAVE_MODE ) this->ObjEngine->write_data_list();

	// Torno allo stato normale
	this->status_bar->stop_work( this );
}

inline void MainWindow::load_data_list() {

	// Avvio il thread per caricare i dati relativi ai server e ai file
	this->status_bar->set_work_status( MSG_READ_INDICES, this );
	this->thread = Glib::Thread::create( 
					sigc::bind( sigc::mem_fun( *this, &MainWindow::processing_data_list ), LOAD_MODE ), true );
}

inline void MainWindow::save_data_list() {

	// Avvio il thread per salvare i dati relativi ai server e ai file
	this->status_bar->set_work_status( MSG_WRITE_INDICES, this );
	this->thread = Glib::Thread::create( 
					sigc::bind( sigc::mem_fun( *this, &MainWindow::processing_data_list ), SAVE_MODE ), true );
}

#endif
