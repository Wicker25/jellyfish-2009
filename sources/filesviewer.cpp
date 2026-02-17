/* 
	Title: Jellyfish - filesviewer.cpp
	Author: "Giacomo Trudu" aka "Wicker25" - wicker25[at]gmail[dot]com
	Site: https://github.com/Wicker25
	License: (CC) BY-NC-SA - http://creativecommons.org/licenses/by-nc-sa/2.5/it/
	Description: Jellyfish is a free download application
*/

#ifndef FILESVIEWER_CPP
#define FILESVIEWER_CPP

template < class T_FilesPage > 
FilesViewer< T_FilesPage >::FilesViewer( T_FilesPage *ObjFilesPage ) {

	// Puntatore alla pagina dei file
	this->ObjFilesPage = ObjFilesPage;

	// Creo il menu contestuale
	this->context_menu = new Gtk::Menu;

	// Imposto le caratteristiche del visualizzatore
	this->set_rules_hint( true );
	this->get_selection()->set_mode( Gtk::SELECTION_MULTIPLE );

	// Creo una struttura per contenere i dati
	this->list_store = Gtk::TreeStore::create( Columns );
	this->set_model( this->list_store );
	this->set_enable_search( false );

	// Creo le colonne del visualizzatore
	Gtk::TreeView::Column *Column = Gtk::manage( new Gtk::TreeView::Column( LABEL_NAME ) );
	Column->pack_start( Columns.column_icon, false );
	Column->pack_end( Columns.column_name, false );
	this->append_column( *Column );
	this->append_column( LABEL_TYPE, Columns.column_type );
	this->append_column( LABEL_HOSTER, Columns.column_hoster );
	this->append_column_numeric( LABEL_PING, Columns.column_ping, "%.2f" );

	// Imposto le colonne ridimensionabili
	this->get_column(0)->set_resizable( true );
	this->get_column(1)->set_resizable( true );
	this->get_column(2)->set_resizable( true );
	this->get_column(3)->set_resizable( true );
}

template < class T_FilesPage > 
FilesViewer< T_FilesPage >::~FilesViewer() {

}

template < class T_FilesPage > 
void FilesViewer< T_FilesPage >::on_row_expanded( const Gtk::TreeModel::iterator &iter, const Gtk::TreeModel::Path &path ) {

	// Mostro il contenuto di una directory
	Gtk::TreeModel::Row row = *iter;
	if ( !this->directory_is_expanded( row ) )
		if ( this->ObjFilesPage->show_directory_files( iter ) )
			Gtk::TreeView::on_row_expanded( iter, path );
		else
			this->collapse_row( path );
}

template < class T_FilesPage > 
void FilesViewer< T_FilesPage >::on_row_activated( const Gtk::TreeModel::Path &path, Gtk::TreeViewColumn *column ) {

	// Scarico le voci selezionate
	this->ObjFilesPage->download_files();

	return Gtk::TreeView::on_row_activated( path, column );
}


template < class T_FilesPage > 
bool FilesViewer< T_FilesPage >::on_button_press_event( GdkEventButton* event ) {

	// La voce in cui l'utente ha cliccato
	Gtk::TreeModel::Path new_path;


	// Apro il menu contestuale alla pressione del tasto destro del mouse
	if ( ( event->type == GDK_BUTTON_PRESS ) && ( event->button == 3 ) ) {

		// Memorizzo la selezione esistente
		std::vector< Gtk::TreeModel::Path > paths = this->get_selection()->get_selected_rows();

		// Controllo se si e' cliccato sulla selezione esistente
		Gtk::TreeModel::Path new_path;
		Gtk::TreeViewColumn *p_column;
		int x, y;

		if ( !this->get_path_at_pos( event->x, event->y, new_path, p_column, x, y ) ) {

			this->unselect_all();
			return Gtk::TreeView::on_button_press_event( event );
		}

		bool current_ = false;

		for ( unsigned int i = 0; i < paths.size(); i++ ) 

			if ( new_path == paths[i] ) { 

				current_ = true;
				break;
			}

		// Apro il menu contestuale
		this->context_menu->popup( event->button, event->time );
		// Se si e' cliccato sulla selezione esistente la lascio intatta
		if ( current_ ) return false;
	}

	return Gtk::TreeView::on_button_press_event( event );
}

template < class T_FilesPage > 
inline void FilesViewer< T_FilesPage >::inline_set_file_icon( Gtk::TreeModel::Row &row, JE::FoundFile &found ) {

	// Modifico l'icona di un file nel visualizzatore
	JE::FileType type = this->ObjFilesPage->get_ObjMainWindow()->get_ObjEngine()->get_file_type( found.file->type );

	if ( type == JE::FOLDER_TYPE ) row[ Columns.column_icon ] = this->ObjFilesPage->get_ObjMainWindow()->icon_folder;
	else if ( type == JE::AUDIO_TYPE ) row[ Columns.column_icon ] = this->ObjFilesPage->get_ObjMainWindow()->icon_audio;
	else if ( type == JE::VIDEO_TYPE ) row[ Columns.column_icon ] = this->ObjFilesPage->get_ObjMainWindow()->icon_video;
	else if ( type == JE::IMAGE_TYPE ) row[ Columns.column_icon ] = this->ObjFilesPage->get_ObjMainWindow()->icon_image;
	else if ( type == JE::DOCUMENT_TYPE ) row[ Columns.column_icon ] = this->ObjFilesPage->get_ObjMainWindow()->icon_document;
	else if ( type == JE::PACKAGE_TYPE ) row[ Columns.column_icon ] = this->ObjFilesPage->get_ObjMainWindow()->icon_package;
	else if ( type == JE::EXECUTABLE_TYPE ) row[ Columns.column_icon ] = this->ObjFilesPage->get_ObjMainWindow()->icon_executable;
	else if ( type == JE::OTHER_TYPE ) row[ Columns.column_icon ] = this->ObjFilesPage->get_ObjMainWindow()->icon_other;
}

template < class T_FilesPage > 
inline void FilesViewer< T_FilesPage >::inline_set_file( Gtk::TreeModel::Row &row, JE::FoundFile &found ) {

	// Modifico i valori di un file nel visualizzatore
	row[ Columns.column_name ] = Glib::locale_to_utf8( found.file->name );
	row[ Columns.column_type ] = found.file->type;
	row[ Columns.column_hoster ] = found.server->url;
	row[ Columns.column_ping ] = found.server->ping;
	row[ Columns.column_expanded ] = false;
	row[ Columns.column_ref ] = found;

	// Imposto l'icona corretta
	this->inline_set_file_icon( row, found );
}

template < class T_FilesPage > 
void FilesViewer< T_FilesPage >::prepend_file( JE::FoundFile &found, bool scroll = false ) {

	// Aggiungo un file al visualizzatore (alla fine della lista)
	Gtk::TreeModel::Row row = *this->list_store->prepend();
	this->inline_set_file( row, found );

	// Mostro la voce nel visualizzatore
	if ( scroll ) this->scroll_to_file( row );
}

template < class T_FilesPage > 
void FilesViewer< T_FilesPage >::append_file( JE::FoundFile &found, bool scroll = false ) {

	// Aggiungo un file al visualizzatore (alla fine della lista)
	Gtk::TreeModel::Row row = *this->list_store->append();
	this->inline_set_file( row, found );
	// Aggiungo una voce vuota per distinguere le cartelle
	if ( found.file->type == FOLDER ) { 

		Gtk::TreeModel::Row sub_row = *this->list_store->append( row.children() );

		JE::FoundFile found;
		found.file = NULL;
		found.server = NULL;

		sub_row[ Columns.column_ref ] = found;
	}

	// Mostro la voce nel visualizzatore
	if ( scroll ) this->scroll_to_file( row );
}

template < class T_FilesPage > 
void FilesViewer< T_FilesPage >::set_file( Gtk::TreeModel::Row &row, JE::FoundFile &found ) {

	// Modifico i valori di un file nel visualizzatore
	this->inline_set_file( row, found );
}

template < class T_FilesPage > 
void FilesViewer< T_FilesPage >::append_into_directory( JE::FoundFile &found, Gtk::TreeModel::Row &parent, bool first = false ) {

	// Aggiungo un file alla cartella del visualizzatore
	Gtk::TreeModel::Row row = *this->list_store->append( parent.children() );
	this->inline_set_file( row, found );
	// Aggiungo una voce vuota per distinguere le cartelle
	if ( found.file->type == FOLDER ) this->list_store->append( row.children() );
	// Rimuovo la voce vuota all'interno della cartella
	if ( first ) this->list_store->erase( --row );
}

template < class T_FilesPage > 
JE::FoundFile FilesViewer< T_FilesPage >::get_file_ref( Gtk::TreeModel::Row &row ) {

	// Ritorno l'indice di un file
	return row[ Columns.column_ref ];
}

template < class T_FilesPage > 
bool FilesViewer< T_FilesPage >::directory_is_expanded( Gtk::TreeModel::Row &row ) {

	// Controllo se una cartella è stata espansa
	return row[ Columns.column_expanded ];
}

template < class T_FilesPage > 
void FilesViewer< T_FilesPage >::set_directory_expanded( Gtk::TreeModel::Row &row, bool value ) {

	// Ricordo che la cartella è stata espansa
	row[ Columns.column_expanded ] = value;
}

template < class T_FilesPage > 
void FilesViewer< T_FilesPage >::clear_list() {

	// Pulisco la lista dei file
	this->list_store->clear();
}

template < class T_FilesPage > 
void FilesViewer< T_FilesPage >::scroll_to_file( const std::string &hoster ) {

	// Cerco il file nel visualizzatore
	typedef Gtk::TreeModel::Children type_children;
	Gtk::TreeModel::Row row;
	type_children children = this->list_store->children();
	type_children::iterator iter;

	for ( iter = children.begin(); iter != children.end(); ++iter ) {

		row = *iter;

		if ( hoster == row[ Columns.column_host ] ) {

			// Lo mostro nel visualizzatore
			this->scroll_to_file( row );
			break;
		}
	}
}

template < class T_FilesPage > 
void FilesViewer< T_FilesPage >::scroll_to_file( Gtk::TreeModel::Row &row ) {

	// Ricavo l'oggetto selettore
	Glib::RefPtr<Gtk::TreeSelection> selection = this->get_selection();
	Gtk::TreeModel::Path path;

	// Seleziono solo la voce scelta
	if ( selection->count_selected_rows() > 0 ) 
		selection->unselect_all();
	selection->select( row );

	// Lo mostro nel visualizzatore
	path = this->list_store->get_path( row );
	this->scroll_to_row( path );
}

template < class T_FilesPage > 
inline void FilesViewer< T_FilesPage >::select_all() {

	// Seleziono tutti i file
	this->get_selection()->select_all();
}

template < class T_FilesPage > 
inline void FilesViewer< T_FilesPage >::unselect_all() {

	// Deseleziono tutti i file
	this->get_selection()->unselect_all();
}

#endif
