/* 
	Title: Jellyfish - downloadsviewer.cpp
	Author: "Giacomo Trudu" aka "Wicker25" - wicker25[at]gmail[dot]com
	Site: https://github.com/Wicker25
	License: (CC) BY-NC-SA - http://creativecommons.org/licenses/by-nc-sa/2.5/it/
	Description: Jellyfish is a free download application
*/

#ifndef DOWNLOADSVIEWER_CPP
#define DOWNLOADSVIEWER_CPP

template < class T_DownloadsPage > 
DownloadsViewer< T_DownloadsPage >::DownloadsViewer( T_DownloadsPage *ObjDownloadsPage ) {

	// Puntatore alla pagina dei download
	this->ObjDownloadsPage = ObjDownloadsPage;

	// Creo il menu contestuale
	this->context_menu = new Gtk::Menu;

	// Imposto le caratteristiche del visualizzatore
	this->set_rules_hint( true );
	this->get_selection()->set_mode( Gtk::SELECTION_MULTIPLE );

	// Creo una struttura per contenere i dati
	this->tree_store = Gtk::TreeStore::create( Columns );
	this->set_model( this->tree_store );
	this->set_search_column( Columns.column_name );

	// Creo le colonne del visualizzatore
	Gtk::TreeView::Column *Column = Gtk::manage( new Gtk::TreeView::Column( LABEL_NAME ) );
	Column->pack_start( Columns.column_icon, false );
	Column->pack_end( Columns.column_name, false );
	this->append_column( *Column );
	this->append_column( LABEL_STATUS, Columns.column_status_icon );
	this->append_column_numeric( LABEL_NOW_SIZE, Columns.column_now_size, "%.2f Kb" );
	this->append_column_numeric( LABEL_SIZE, Columns.column_size, "%.2f Kb" );
	this->append_column_numeric( LABEL_SPEED, Columns.column_speed, "%d Kb/s" );
	Gtk::CellRendererProgress* cell = Gtk::manage( new Gtk::CellRendererProgress );
	this->append_column( LABEL_PROGRESS, *cell );
	this->get_column(5)->add_attribute( cell->property_value(), Columns.column_progress );

	// Imposto le colonne riordinabili
	this->get_column(0)->set_sort_column_id( Columns.column_name );
	this->get_column(2)->set_sort_column_id( Columns.column_now_size );
	this->get_column(3)->set_sort_column_id( Columns.column_size );
	this->get_column(4)->set_sort_column_id( Columns.column_speed );
	this->get_column(5)->set_sort_column_id( Columns.column_progress );

	// Imposto le colonne ridimensionabili
	this->get_column(0)->set_resizable( true );
	this->get_column(2)->set_resizable( true );
	this->get_column(3)->set_resizable( true );
	this->get_column(4)->set_resizable( true );
	this->get_column(5)->set_resizable( true );
}

template < class T_DownloadsPage > 
DownloadsViewer< T_DownloadsPage >::~DownloadsViewer() {

}

template < class T_DownloadsPage > 
bool DownloadsViewer< T_DownloadsPage >::on_key_press_event( GdkEventKey* event ) {

	// Cancello le voci selezionate alla pressione del tasto "CANC"
	if ( event->hardware_keycode == 119 ) this->ObjDownloadsPage->remove_selected_downloads();

	return Gtk::TreeView::on_key_press_event( event );
}

template < class T_DownloadsPage > 
bool DownloadsViewer< T_DownloadsPage >::on_button_press_event( GdkEventButton* event ) {

	// La voce in cui l'utente ha cliccato
	Gtk::TreeModel::Path new_path;
	Gtk::TreeViewColumn *p_column;
	int x, y;

	if ( !this->get_path_at_pos( event->x, event->y, new_path, p_column, x, y ) ) {

		this->unselect_all();
		return Gtk::TreeView::on_button_press_event( event );
	}

	// Controllo se ha premuto un tasto del mouse
	if ( ( event->type == GDK_BUTTON_PRESS ) )

		/** MENU CONTESTUALE **/
		if ( event->button == 3 ) {

			// Memorizzo la selezione esistente
			std::vector< Gtk::TreeModel::Path > paths = this->get_selection()->get_selected_rows();

			// Controllo se si e' cliccato sulla selezione esistente
			bool current = false;

			for ( unsigned int i = 0; i < paths.size(); i++ ) 

				if ( new_path == paths[i] ) {

					current = true;
					break;
				}

			// Apro il menu contestuale
			this->context_menu->popup( event->button, event->time );

			// Se si e' cliccato sulla selezione esistente la lascio intatta
			if ( current ) return false;
		}

	return Gtk::TreeView::on_button_press_event( event );
}

template < class T_DownloadsPage > 
void DownloadsViewer< T_DownloadsPage >::on_row_activated( const Gtk::TreeModel::Path &path, Gtk::TreeViewColumn *column ) {

	Gtk::TreeView::on_row_activated( path, column );

	this->ObjDownloadsPage->open_selected_downloads();
}

template < class T_DownloadsPage > 
void DownloadsViewer< T_DownloadsPage >::set_status( Gtk::TreeModel::Row &row, JE::DownloadStatus status ) {

	// Modifico lo stato del download
	row[ Columns.column_status ] = status;

	// Modifico l'icona dello stato del download
	switch( status ) {

		case JE::WAIT_STATUS: {

				row[ Columns.column_status_icon ] = this->ObjDownloadsPage->get_ObjMainWindow()->icon_wait;
				break;
			}

		case JE::STARTED_STATUS: {

				row[ Columns.column_status_icon ] = this->ObjDownloadsPage->get_ObjMainWindow()->icon_start;
				break;
			}

		case JE::PAUSED_STATUS: {

				row[ Columns.column_status_icon ] = this->ObjDownloadsPage->get_ObjMainWindow()->icon_pause;
				row[ Columns.column_speed ] = 0;
				break;
			}

		case JE::STOPED_STATUS: {

				row[ Columns.column_status_icon ] = this->ObjDownloadsPage->get_ObjMainWindow()->icon_stop;
				row[ Columns.column_speed ] = 0;
				break;
			}

		case JE::INTERRUPTED_STATUS: {

				row[ Columns.column_status_icon ] = this->ObjDownloadsPage->get_ObjMainWindow()->icon_interrupt;
				row[ Columns.column_speed ] = 0;
				break;
			}

		case JE::QUEUED_STATUS: {

				row[ Columns.column_status_icon ] = this->ObjDownloadsPage->get_ObjMainWindow()->icon_queue;
				row[ Columns.column_speed ] = 0;
				break;
			}
	}
}

template < class T_DownloadsPage > 
JE::DownloadStatus DownloadsViewer< T_DownloadsPage >::get_status( Gtk::TreeModel::Row &row ) {

	return row[ Columns.column_status ];
}

template < class T_DownloadsPage > 
inline void DownloadsViewer< T_DownloadsPage >::inline_set_file_icon( Gtk::TreeModel::Row &row, JE::DownloadInfo &download ) {

	// Modifico l'icona di un file nel visualizzatore
	JE::FileType type = this->ObjDownloadsPage->get_ObjMainWindow()->get_ObjEngine()->get_file_type( download.type );

	if ( type == JE::FOLDER_TYPE ) row[ Columns.column_icon ] = this->ObjDownloadsPage->get_ObjMainWindow()->icon_folder;
	else if ( type == JE::AUDIO_TYPE ) row[ Columns.column_icon ] = this->ObjDownloadsPage->get_ObjMainWindow()->icon_audio;
	else if ( type == JE::VIDEO_TYPE ) row[ Columns.column_icon ] = this->ObjDownloadsPage->get_ObjMainWindow()->icon_video;
	else if ( type == JE::IMAGE_TYPE ) row[ Columns.column_icon ] = this->ObjDownloadsPage->get_ObjMainWindow()->icon_image;
	else if ( type == JE::DOCUMENT_TYPE ) row[ Columns.column_icon ] = this->ObjDownloadsPage->get_ObjMainWindow()->icon_document;
	else if ( type == JE::PACKAGE_TYPE ) row[ Columns.column_icon ] = this->ObjDownloadsPage->get_ObjMainWindow()->icon_package;
	else if ( type == JE::EXECUTABLE_TYPE ) row[ Columns.column_icon ] = this->ObjDownloadsPage->get_ObjMainWindow()->icon_executable;
	else if ( type == JE::OTHER_TYPE ) row[ Columns.column_icon ] = this->ObjDownloadsPage->get_ObjMainWindow()->icon_other;
}

template < class T_DownloadsPage > 
inline void DownloadsViewer< T_DownloadsPage >::inline_set_download( Gtk::TreeModel::Row &row, JE::DownloadInfo &download ) {

	// Modifico i valori di un download nel visualizzatore
	if ( download.type == FOLDER )
		row[ Columns.column_name ] = Glib::locale_to_utf8( download.name );
	else
		row[ Columns.column_name ] = Glib::locale_to_utf8( download.name + "." + download.type );

	row[ Columns.column_speed ] = 0;
	row[ Columns.column_now_size ] = 0;
	row[ Columns.column_size ] = 0;
	row[ Columns.column_progress ] = 0;

	// Imposto l'icona corretta
	this->inline_set_file_icon( row, download );

	// Imposta lo stato di default
	this->set_status( row, download.status );
}

template < class T_DownloadsPage > 
Gtk::TreeModel::Row *DownloadsViewer< T_DownloadsPage >::append_download( JE::DownloadInfo &download, bool scroll = false ) {

	// Aggiungo un download al visualizzatore (alla fine della lista)
	Gtk::TreeModel::Row *row = new Gtk::TreeModel::Row( *this->tree_store->prepend() );
	this->inline_set_download( *row, download );

	// Memorizza il puntatore ad un download dell'engine
	(*row)[ Columns.column_ref ] = NULL;

	// Mostro la voce nel visualizzatore
	if ( scroll ) this->scroll_to_download( *row );

	return row;
}

template < class T_DownloadsPage > 
Gtk::TreeModel::Row *DownloadsViewer< T_DownloadsPage >::append_download_with_ref( JE::DownloadTrack *track, void *row ) {

	// Aggiungo un download al visualizzatore (alla fine della lista)
	Gtk::TreeModel::Row *row_;

	if ( row != NULL )
		row_ = new Gtk::TreeModel::Row( *this->tree_store->append( ((Gtk::TreeModel::Row *) row)->children() ) );
	 else
	 	row_ = new Gtk::TreeModel::Row( *this->tree_store->append() );

	// Imposto le caratteristiche
	this->inline_set_download( *row_, track->info );

	// Memorizza il puntatore ad un download dell'engine
	(*row_)[ Columns.column_ref ] = track;

	return row_;
}

template < class T_DownloadsPage > 
void DownloadsViewer< T_DownloadsPage >::set_download( Gtk::TreeModel::Row &row, JE::DownloadInfo &download ) {

	// Modifico i valori di un download nel visualizzatore
	this->inline_set_download( row, download );
}

template < class T_DownloadsPage > 
inline void DownloadsViewer< T_DownloadsPage >::set_download_progress( Gtk::TreeModel::Row &row, double &dlnow, double &dltotal,
																double &speed, bool &complete ) {

	// Modifica le informazioni sul download in corso
	row[ Columns.column_now_size ] = dlnow;
	row[ Columns.column_size ] = dltotal;
	row[ Columns.column_speed ] = (unsigned int) speed;
	row[ Columns.column_progress ] = (dlnow / dltotal) * 100;
}

template < class T_DownloadsPage > 
inline void DownloadsViewer< T_DownloadsPage >::set_download_status( Gtk::TreeModel::Row &row, JE::DownloadStatus &status ) {

	// Aggiorna lo stato del download
	this->set_status( row, status );
}

template < class T_DownloadsPage > 
void DownloadsViewer< T_DownloadsPage >::set_download_ref( Gtk::TreeModel::Row &row, JE::DownloadTrack *ref ) {

	// Memorizza il puntatore ad un download dell'engine
	row[ Columns.column_ref ] = ref;
}

template < class T_DownloadsPage > 
JE::DownloadTrack *DownloadsViewer< T_DownloadsPage >::get_download_ref( Gtk::TreeModel::Row &row ) {

	// Ritorna l'indice di un download
	return row[ Columns.column_ref ];
}

template < class T_DownloadsPage > 
void DownloadsViewer< T_DownloadsPage >::clear_list() {

	// Pulisco la lista dei download
	this->tree_store->clear();
}

template < class T_DownloadsPage > 
void DownloadsViewer< T_DownloadsPage >::scroll_to_download( const std::string &hoster ) {

	// Cerco il download nel visualizzatore
	typedef Gtk::TreeModel::Children type_children;
	Gtk::TreeModel::Row row;
	type_children children = this->tree_store->children();
	type_children::iterator iter;

	for ( iter = children.begin(); iter != children.end(); ++iter ) {

		row = *iter;

		if ( hoster == row[ Columns.column_host ] ) {

			// Lo mostro nel visualizzatore
			this->scroll_to_download( row );
			break;
		}
	}
}

template < class T_DownloadsPage > 
void DownloadsViewer< T_DownloadsPage >::scroll_to_download( Gtk::TreeModel::Row &row ) {

	// Ricavo l'oggetto selettore
	Glib::RefPtr<Gtk::TreeSelection> selection = this->get_selection();
	Gtk::TreeModel::Path path;

	// Seleziono solo la voce scelta
	if ( selection->count_selected_rows() > 0 ) 
		selection->unselect_all();
	selection->select( row );

	// Lo mostro nel visualizzatore
	path = this->tree_store->get_path( row );
	this->scroll_to_row( path );
}

template < class T_DownloadsPage > 
inline void DownloadsViewer< T_DownloadsPage >::select_all() {

	// Seleziono tutti i download
	this->get_selection()->select_all();
}

template < class T_DownloadsPage > 
inline void DownloadsViewer< T_DownloadsPage >::unselect_all() {

	// Deseleziono tutti i download
	this->get_selection()->unselect_all();
}

#endif
