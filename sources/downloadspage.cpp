/* 
	Title: Jellyfish - downloadspage.cpp
	Author: "Giacomo Trudu" aka "Wicker25" - wicker25[at]gmail[dot]com
	Site: http://www.hackyourmind.org/
	License: (CC) BY-NC-SA - http://creativecommons.org/licenses/by-nc-sa/2.5/it/
	Description: Jellyfish is a free download application
*/

#ifndef DOWNLOADSPAGE_CPP
#define DOWNLOADSPAGE_CPP

template < class T_MainWindow >
DownloadsPage< T_MainWindow >::DownloadsPage( T_MainWindow *ObjMainWindow ) {

	// Puntatore alla finestra principale
	this->ObjMainWindow = ObjMainWindow;

	// Inizializzo le variabili di controllo
	this->thread_is_running = false;

	// Creo il contenitore principale
	Gtk::VBox *vbox = new Gtk::VBox;
	this->pack_start( *vbox, true, true, 2 );

	// Creo il contenitore del visualizzatore
	Gtk::HBox *hbox = new Gtk::HBox;
	vbox->pack_start( *hbox, true, true, 5 );

	// Creo il visualizzatore
	Gtk::ScrolledWindow *scrolled_window = new Gtk::ScrolledWindow;
	scrolled_window->set_policy( Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC );
	scrolled_window->set_shadow_type( Gtk::SHADOW_IN );

	this->downloads_viewer = new DownloadsViewer< DownloadsPage >( this );

	scrolled_window->add( *this->downloads_viewer );
	hbox->pack_start( *scrolled_window, true, true, 5 );

	// Configuro il menu contestuale
	this->make_context_menu();

	// Collego le funzioni d'aggiornamento dei download
	this->ObjMainWindow->get_ObjEngine()->signal_connect()->connect(
									sigc::mem_fun( *this, &DownloadsPage::connect_download ) );
	this->ObjMainWindow->get_ObjEngine()->signal_progress()->connect(
									sigc::mem_fun( *this, &DownloadsPage::refresh_download_progress ) );
	this->ObjMainWindow->get_ObjEngine()->signal_status()->connect(
									sigc::mem_fun( *this, &DownloadsPage::refresh_download_status ) );

	// Mostra la pagina dei download
	this->show_all();
}

template < class T_MainWindow >
DownloadsPage< T_MainWindow >::~DownloadsPage() {

}

template < class T_MainWindow >
inline void DownloadsPage< T_MainWindow >::make_context_menu() {

	// Configuro il menu contestuale
	Gtk::Menu *context_menu = this->downloads_viewer->get_context_menu();
	context_menu->accelerate( *this->ObjMainWindow );

	// Nuova immagine
	Gtk::Image *image;

	// Aggiungo le voci al menu
	Gtk::Menu::MenuList *menulist = &context_menu->items();

	// Aggiungo la voce "Riprendi"
	image = new Gtk::Image( Gtk::Stock::MEDIA_PLAY, Gtk::ICON_SIZE_MENU );
	menulist->push_back( Gtk::Menu_Helpers::ImageMenuElem( "Riprendi", *image,
		sigc::mem_fun( *this, &DownloadsPage::continue_selected_downloads ) ) );
	// Aggiungo la voce "Pausa"
	menulist->push_back( Gtk::Menu_Helpers::StockMenuElem( Gtk::Stock::MEDIA_PAUSE,
		sigc::mem_fun( *this, &DownloadsPage::pause_selected_downloads ) ) );
	// Aggiungo un separatore orizzontale
	menulist->push_back( Gtk::Menu_Helpers::SeparatorElem() );
	// Aggiungo la voce "Rimuovi"
	menulist->push_back( Gtk::Menu_Helpers::StockMenuElem( Gtk::Stock::REMOVE,
		sigc::mem_fun( *this, &DownloadsPage::remove_selected_downloads ) ) );
	// Aggiungo la voce "Rimuovi completamente"
	menulist->push_back( Gtk::Menu_Helpers::StockMenuElem( Gtk::Stock::DELETE,
		sigc::mem_fun( *this, &DownloadsPage::remove_and_clean_selected_downloads ) ) );
	// Aggiungo un separatore orizzontale
	menulist->push_back( Gtk::Menu_Helpers::SeparatorElem() );
	// Aggiungo la voce "Apri"
	menulist->push_back( Gtk::Menu_Helpers::StockMenuElem( Gtk::Stock::OPEN,
		sigc::mem_fun( *this, &DownloadsPage::open_selected_downloads ) ) );
}

template < class T_MainWindow >
void DownloadsPage< T_MainWindow >::add_download( JE::FoundFile &found ) {

	// Aggiungo un file alla coda degli scaricamenti
	this->ObjMainWindow->get_ObjEngine()->append_download( found );
}

template < class T_MainWindow >
void *DownloadsPage< T_MainWindow >::connect_download( JE::DownloadTrack *track, void *row ) {

	// Connetto il visualizzatore alla connessione
	//gdk_threads_enter();
	void *ref = (void *)this->downloads_viewer->append_download_with_ref( track, row );
	//gdk_threads_leave();

	return ref;
}

template < class T_MainWindow >
void DownloadsPage< T_MainWindow >::refresh_download_progress( JE::DownloadTrack *track, double dlnow, double dltotal, double speed, bool complete ) {

	// Aggiorno i progressi di un download
	Gtk::TreeModel::Row *row = (Gtk::TreeModel::Row *) track->ref;
	gdk_threads_enter();
	this->downloads_viewer->set_download_progress( *row, dlnow, dltotal, speed, complete );
	gdk_threads_leave();
}

template < class T_MainWindow >
void DownloadsPage< T_MainWindow >::refresh_download_status( JE::DownloadTrack *track, JE::DownloadStatus status ) {

	// Ricavo la riga nel visualizzatore
	Gtk::TreeModel::Row *row = (Gtk::TreeModel::Row *) track->ref;

	if ( status != JE::REMOVED_STATUS ) {

		// Aggiorno lo stato di un download 
		gdk_threads_enter();
		this->downloads_viewer->set_download_status( *row, status );
		gdk_threads_leave();

	} else {

		// Rimuovo il file dal visualizzatore
		gdk_threads_enter();
		this->downloads_viewer->remove_download( *row );
		gdk_threads_leave();
	}
}

template < class T_MainWindow >
void DownloadsPage< T_MainWindow >::process_download_thread( PROCESSING_MODE mode ) {

	this->thread_is_running = true;

	// Ricavo le voci selezionate nel visualizzatore
	std::vector< Gtk::TreeModel::Path > paths;
	gdk_threads_enter();
	paths = this->downloads_viewer->get_selection()->get_selected_rows();
	gdk_threads_leave();

	// Rimuovo i file selezionati dalla coda degli scaricamenti
	for( int i = paths.size() - 1; i >= 0; i-- ) {

		// Ricavo i riferimenti alle strutture
		gdk_threads_enter();
		Gtk::TreeModel::Row row = *this->downloads_viewer->get_tree_store()->get_iter( paths[i] );
		gdk_threads_leave();

		// Distinguo file da cartelle
		gdk_threads_enter();
		JE::DownloadTrack *track = this->downloads_viewer->get_download_ref( row );
		gdk_threads_leave();

		if ( mode == OPEN_MODE ) {

			// Apro il file
			this->ObjMainWindow->get_ObjEngine()->open_downloaded_file( track );

		} else if ( mode == REMOVE_MODE ) {

			// Rimuovo il file dalla coda degli scaricamenti
			this->ObjMainWindow->get_ObjEngine()->remove_download( track );

		} else if ( mode == REMOVE_AND_CLEAN_MODE ) {

			// Rimuovo il file dalla coda degli scaricamenti e dall'hard disk
			this->ObjMainWindow->get_ObjEngine()->remove_and_clean_download( track );

		} else if ( mode == PAUSE_MODE ) {

			// Fermo momentaneamente il download
			this->ObjMainWindow->get_ObjEngine()->pause_download( track );

		} else if ( mode == CONTINUE_MODE ) {

			// Riprendo il download
			this->ObjMainWindow->get_ObjEngine()->resume_download( track );
		}
	}

	// Torno allo stato normale
	this->ObjMainWindow->get_status_bar()->stop_work( this->ObjMainWindow );
	this->thread_is_running = false;
}

template < class T_MainWindow >
void DownloadsPage< T_MainWindow >::continue_selected_downloads() {

	// Riprende i download selezionati
	if ( this->thread_is_running ) return;
	this->ObjMainWindow->get_status_bar()->set_work_status( MSG_RESUME_DOWNLOAD, this->ObjMainWindow, false );
	// Avvio il thread di lavoro
	Glib::Thread *thread = Glib::Thread::create( 
				sigc::bind( sigc::mem_fun( *this, &DownloadsPage::process_download_thread ), CONTINUE_MODE ), false );
}

template < class T_MainWindow >
void DownloadsPage< T_MainWindow >::pause_selected_downloads() {

	// Ferma i download selezionati
	if ( this->thread_is_running ) return;
	this->ObjMainWindow->get_status_bar()->set_work_status( MSG_PAUSE_DOWNLOAD, this->ObjMainWindow, false );
	// Avvio il thread di lavoro
	Glib::Thread *thread = Glib::Thread::create( 
				sigc::bind( sigc::mem_fun( *this, &DownloadsPage::process_download_thread ), PAUSE_MODE ), false );
}

template < class T_MainWindow >
void DownloadsPage< T_MainWindow >::open_selected_downloads() {

	// Apro i download selezionati
	if ( this->thread_is_running ) return;
	this->ObjMainWindow->get_status_bar()->set_work_status( MSG_OPEN_DOWNLOAD, this->ObjMainWindow, false );
	// Avvio il thread di lavoro
	Glib::Thread *thread = Glib::Thread::create( 
				sigc::bind( sigc::mem_fun( *this, &DownloadsPage::process_download_thread ),OPEN_MODE ), false );
}

template < class T_MainWindow >
void DownloadsPage< T_MainWindow >::remove_selected_downloads() {

	// Rimuove i download selezionati
	if ( this->thread_is_running ) return;
	this->ObjMainWindow->get_status_bar()->set_work_status( MSG_REMOVE_DOWNLOAD, this->ObjMainWindow, false );
	// Avvio il thread di lavoro
	Glib::Thread *thread = Glib::Thread::create( 
				sigc::bind( sigc::mem_fun( *this, &DownloadsPage::process_download_thread ), REMOVE_MODE ), false );
}

template < class T_MainWindow >
void DownloadsPage< T_MainWindow >::remove_and_clean_selected_downloads() {

	// Rimuove i download selezionati
	if ( this->thread_is_running ) return;
	this->ObjMainWindow->get_status_bar()->set_work_status( MSG_REMOVE_DOWNLOAD, this->ObjMainWindow, false );
	// Avvio il thread di lavoro
	Glib::Thread *thread = Glib::Thread::create( 
				sigc::bind( sigc::mem_fun( *this, &DownloadsPage::process_download_thread ), REMOVE_AND_CLEAN_MODE ), false );
}


#endif
