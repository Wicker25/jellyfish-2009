/* 
	Title: Jellyfish - filespage.cpp
	Author: "Giacomo Trudu" aka "Wicker25" - wicker25[at]gmail[dot]com
	Site: https://github.com/Wicker25
	License: (CC) BY-NC-SA - http://creativecommons.org/licenses/by-nc-sa/2.5/it/
	Description: Jellyfish is a free download application
*/

#ifndef FILESPAGE_CPP
#define FILESPAGE_CPP

template < class T_MainWindow > 
FilesPage< T_MainWindow >::FilesPage( T_MainWindow *ObjMainWindow ) {

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

	this->files_viewer = new FilesViewer< FilesPage >( this );

	scrolled_window->add( *this->files_viewer );
	hbox->pack_start( *scrolled_window, true, true, 5 );

	// Configuro il menu contestuale
	this->make_context_menu();

	// Mostra la pagina dei file
	this->show_all();
}

template < class T_MainWindow > 
FilesPage< T_MainWindow >::~FilesPage() {

}

template < class T_MainWindow > 
inline void FilesPage< T_MainWindow >::make_entry_bar( Gtk::VBox *vbox ) {

	// Puntatore ad un oggetto immagine
	Gtk::Image *image;

	// Creo il contenitore della barra d'inserimento
	Gtk::HBox *hbox = new Gtk::HBox;
	vbox->pack_start( *hbox, false, true, 0 );

	// Creo la barra d'inserimento
	this->entry = new Gtk::Entry;
	this->entry->set_max_length( 300 );
	this->entry->signal_activate().connect( sigc::mem_fun( *this, &FilesPage::find_file ) );
	hbox->pack_start( *this->entry, true, true, 3 );

	// Creo il pulsante
	Gtk::Button *add_button = new Gtk::Button( Gtk::Stock::FIND );
	add_button->signal_clicked().connect( sigc::mem_fun( *this, &FilesPage::find_file ) );
	hbox->pack_start( *add_button, false, true, 0 );

	// Creo il pannello della ricerca avvanzata
	hbox = new Gtk::HBox;
	Gtk::Expander *expander = new Gtk::Expander( LABEL_ADVANCED_SEARCH );
	expander->set_spacing( 5 );
	hbox->pack_start( *expander, true, true, 4 );
	vbox->pack_start( *hbox, false, true, 4 );
	hbox = new Gtk::HBox;
	expander->add( *hbox );

	// Scelta del tipo di file
	Gtk::Label *label = new Gtk::Label( LABEL_FORMAT );
	hbox->pack_start( *label, false, true, 4 );
	this->combo_box = new Gtk::ComboBoxText;

	// Aggiungo le voci del tipo di file
	this->combo_box->append_text( LABEL_ALL_TYPE );
	this->combo_box->append_text( LABEL_FOLDER_TYPE );
	this->combo_box->append_text( LABEL_AUDIO_TYPE );
	this->combo_box->append_text( LABEL_VIDEO_TYPE );
	this->combo_box->append_text( LABEL_IMAGE_TYPE );
	this->combo_box->append_text( LABEL_DOCUMENT_TYPE );
	this->combo_box->append_text( LABEL_PACKAGE_TYPE );
	this->combo_box->append_text( LABEL_EXECUTABLE_TYPE );

	// Imposta la voce di default
	this->combo_box->set_active(0);

	hbox->pack_start( *this->combo_box, false, true, 4 );
}

template < class T_MainWindow > 
inline void FilesPage< T_MainWindow >::make_context_menu() {

	// Configuro il menu contestuale
	Gtk::Menu *context_menu = this->files_viewer->get_context_menu();
	context_menu->accelerate( *this->ObjMainWindow );

	// Nuova immagine
	Gtk::Image *image;

	// Aggiungo le voci al menu
	Gtk::Menu::MenuList *menulist = &context_menu->items();

	// Aggiungo la voce "Scarica"
	image = new Gtk::Image( Gtk::Stock::SAVE, Gtk::ICON_SIZE_MENU );
	menulist->push_back( Gtk::Menu_Helpers::ImageMenuElem( ACTION_DOWNLOAD, *image,
		sigc::mem_fun( *this, &FilesPage< T_MainWindow >::download_files ) ) );
	// Aggiungo la voce "Informazioni"
	image = new Gtk::Image( Gtk::Stock::INFO, Gtk::ICON_SIZE_MENU );
	menulist->push_back( Gtk::Menu_Helpers::ImageMenuElem( ACTION_INFO, *image,
		sigc::mem_fun( *this->files_viewer, &FilesViewer< FilesPage >::collapse_all ) ) );
	// Aggiungo un separatore orizzontale
	menulist->push_back( Gtk::Menu_Helpers::SeparatorElem() );
}

template < class T_MainWindow > 
void FilesPage< T_MainWindow >::find_file_thread( const std::string &key, JE::FileType &type, JE::EngineCode &code ) {

	this->thread_is_running = true;

	// Pulisco il visualizzatore
	gdk_threads_enter();
	this->files_viewer->clear_list();
	gdk_threads_leave();

	// Acquisco le informazioni sul nuovo hoster
	code = this->ObjMainWindow->get_ObjEngine()->find_file( key, type );

	if ( code == JE::ENGINE_DONE ) {

		// Riscontri
		JE::FoundFilesList *found_files = this->ObjMainWindow->get_ObjEngine()->get_found_files();

		// Iteratori
		JE::FoundFilesList::iterator i = found_files->begin();
		JE::FoundFilesList::iterator end = found_files->end();

		while ( i != end ) {

			// Aggiungo un riscontro al visualizzatore
			gdk_threads_enter();
			this->files_viewer->append_file( *i );
			gdk_threads_leave();
			i++;
		}
	}

	// Torno allo stato normale
	this->ObjMainWindow->get_status_bar()->stop_work( this->ObjMainWindow );
	this->thread_is_running = false;
}

template < class T_MainWindow > 
void FilesPage< T_MainWindow >::find_file() {

	// Estraggo le chiavi di ricerca
	std::string key = this->entry->get_text();

	// Controllo che non sia vuoto
	if ( !key.empty() ) {

		// Formato del file
		JE::FileType type = (JE::FileType) this->combo_box->get_active_row_number();

		// Codice di controllo
		JE::EngineCode code;

		// Avvio il thread per la ricerca di un file
		this->ObjMainWindow->get_status_bar()->set_work_status( MSG_FIND, this->ObjMainWindow );
		Glib::Thread *thread = Glib::Thread::create( 
					sigc::bind( sigc::mem_fun( *this, &FilesPage::find_file_thread ), key, type, code ), false );
	}
}

template < class T_MainWindow > 
void FilesPage< T_MainWindow >::show_directory_files_thread( Gtk::TreeModel::iterator &iter, JE::EngineCode &code ) {

	this->thread_is_running = true;

	// Estraggo le informazioni sulla directory
	JE::FoundFile found;

	Gtk::TreeModel::Row row = *iter;

	// Estraggo le informazioni sulla directory
	gdk_threads_enter();
	found = this->files_viewer->get_file_ref( row );
	gdk_threads_leave();

	// Cerco i file della directory
	code = this->ObjMainWindow->get_ObjEngine()->get_sub_files( found );
	JE::FoundFilesList *found_files = this->ObjMainWindow->get_ObjEngine()->get_found_files();

	// Iteratori
	JE::FoundFilesList::iterator i = found_files->begin();
	JE::FoundFilesList::iterator end = found_files->end();

	// Aggiungo un file al visualizzatore (eliminando la voce vuota)
	if ( i != end ) {

		gdk_threads_enter();
		this->files_viewer->append_into_directory( *i, row, true );
		gdk_threads_leave();
		i++;

		while ( i != end ) {

			// Aggiungo un file al visualizzatore
			gdk_threads_enter();
			this->files_viewer->append_into_directory( *i, row );
			gdk_threads_leave();
			i++;
		}
	}

	// Ricordo che la cartella è già stata espansa
	gdk_threads_enter();
	this->files_viewer->set_directory_expanded( row, true );
	gdk_threads_leave();

	// Torno allo stato normale
	this->ObjMainWindow->get_status_bar()->stop_work( this->ObjMainWindow );
	this->thread_is_running = false;
}

template < class T_MainWindow >
bool FilesPage< T_MainWindow >::show_directory_files( const Gtk::TreeModel::iterator &iter ) {

	// Codice di controllo
	JE::EngineCode code;

	// Avvio il thread per la ricerca dei file di una directory
	if ( this->thread_is_running ) return false;
	this->ObjMainWindow->get_status_bar()->set_work_status( MSG_FIND, this->ObjMainWindow, false );
	Glib::Thread *thread = Glib::Thread::create( 
				sigc::bind( sigc::mem_fun( *this, &FilesPage::show_directory_files_thread ), iter, code ), false );
	return true;
}

template < class T_MainWindow > 
void FilesPage< T_MainWindow >::clear_founds() {

	// Pulisco la lista dei risultati
	gdk_threads_enter();
	this->files_viewer->clear_list();
	gdk_threads_leave();
}

template < class T_MainWindow > 
void FilesPage< T_MainWindow >::processing_thread( PROCESSING_MODE mode ) {

	this->thread_is_running = true;

	// Riferimenti alle voci da modificare
	std::vector< Gtk::TreeModel::Path > paths;
	std::vector< Gtk::TreeModel::Row > rows;

	// Ricavo le voci selezionate nel visualizzatore
	gdk_threads_enter();
	paths = this->files_viewer->get_selection()->get_selected_rows();
	gdk_threads_leave();

	// Ricavo i riferimenti dei server selezionati
	while ( !paths.empty() ) {

		gdk_threads_enter();
		rows.push_back( *this->files_viewer->get_model()->get_iter( paths.back() ) );
		gdk_threads_leave();
		paths.pop_back();
	}

	// Messaggio nella barra del progresso
	string msg;

	for( int i = rows.size() - 1; i >= 0 ; i-- ) {

		// Ricavo la struttura del server selezionato
		JE::FoundFile found = this->files_viewer->get_file_ref( rows[i] );
		if ( found.file == NULL || found.server == NULL ) continue;

		// Aggiorno il messaggio della barra del progresso
		msg = MSG_ADD_DOWNLOAD;
		msg.insert( 18, found.file->name );

		gdk_threads_enter();
		this->ObjMainWindow->get_status_bar()->change_msg( msg );
		gdk_threads_leave();

		// Aggiungo i file selezionati alla coda degli scaricamenti
		if ( mode == DOWNLOAD_MODE ) {

			//gdk_threads_enter();
			this->ObjMainWindow->get_pages_box()->get_downloads_page()->add_download( found );
			//gdk_threads_leave();
		}
	}

	// Torno allo stato normale
	this->ObjMainWindow->get_status_bar()->stop_work( this->ObjMainWindow );
	this->thread_is_running = false;
}

template < class T_MainWindow > 
void FilesPage< T_MainWindow >::download_files() {

	// Avvio il thread per l'aggiornamento del valore del ping
	if ( this->thread_is_running ) return;
	this->ObjMainWindow->get_status_bar()->set_work_status( MSG_PROCESSING, this->ObjMainWindow, false );
	Glib::Thread *thread = Glib::Thread::create( 
					sigc::bind( sigc::mem_fun( *this, &FilesPage::processing_thread ), DOWNLOAD_MODE ), false );
}

#endif
