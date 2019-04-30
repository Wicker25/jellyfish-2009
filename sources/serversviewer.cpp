/* 
	Title: Jellyfish - serversviewer.cpp
	Author: "Giacomo Trudu" aka "Wicker25" - wicker25[at]gmail[dot]com
	Site: http://www.hackyourmind.org/
	License: (CC) BY-NC-SA - http://creativecommons.org/licenses/by-nc-sa/2.5/it/
	Description: Jellyfish is a free download application
*/

#ifndef SERVERSVIEWER_CPP
#define SERVERSVIEWER_CPP

template < class T_ServersPage > 
ServersViewer< T_ServersPage >::ServersViewer( T_ServersPage *ObjServersPage ) {

	// Puntatore alla pagina dei server
	this->ObjServersPage = ObjServersPage;

	// Creo il menu contestuale
	this->context_menu = new Gtk::Menu;

	// Imposto le caratteristiche del visualizzatore
	this->set_rules_hint( true );
	this->get_selection()->set_mode( Gtk::SELECTION_MULTIPLE );

	// Creo una struttura per contenere i dati
	this->list_store = Gtk::ListStore::create( Columns );
	this->set_model( this->list_store );
	this->set_search_column( Columns.column_host );

	// Creo le colonne del visualizzatore
	this->append_column_editable( NO_LABEL, Columns.column_enabled );
	this->append_column( LABEL_ADDRESS, Columns.column_host );
	this->append_column_numeric( LABEL_PING, Columns.column_ping, "%.2f" );
	this->append_column_numeric( LABEL_FILES_AND_DIRECTORIES, Columns.column_files, "%d" );

	// Imposto le colonne riordinabili
	this->get_column(1)->set_sort_column_id( Columns.column_host );
	this->get_column(2)->set_sort_column_id( Columns.column_ping );
	this->get_column(3)->set_sort_column_id( Columns.column_files );

	// Imposto le colonne ridimensionabili
	this->get_column(1)->set_resizable( true );
	this->get_column(2)->set_resizable( true );
	this->get_column(3)->set_resizable( true );
}

template < class T_ServersPage > 
ServersViewer< T_ServersPage >::~ServersViewer() {

}

template < class T_ServersPage > 
bool ServersViewer< T_ServersPage >::on_key_press_event( GdkEventKey* event ) {

	// Cancello le voci selezionate alla pressione del tasto "CANC"
	if ( event->hardware_keycode == 119 ) this->ObjServersPage->remove_selected_servers();

	return Gtk::TreeView::on_key_press_event( event );
}

template < class T_ServersPage > 
bool ServersViewer< T_ServersPage >::on_button_press_event( GdkEventButton* event ) {

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

		/** ABILITA/DISABILITA' SERVER **/

		else if ( event->button == 1 ) {

			// Memorizzo il valore "enabled" della voce
			Gtk::TreeModel::iterator iter = this->list_store->get_iter( new_path );
			Gtk::TreeModel::Row row = *iter;
			bool enabled = row[ Columns.column_enabled ];

			// Eseguo la funzione standart
			bool result = Gtk::TreeView::on_button_press_event( event );

			// Controllo se il valore "enabled" è cambiato
			iter = this->list_store->get_iter( new_path );
			row = *iter;

			// In caso di una modifica aggiorno la lista dati dell'engine
			if ( enabled != row[ Columns.column_enabled ] ) {

				JE::Server *server = this->get_server_ref( row );
				if ( !this->ObjServersPage->enable_server( server, row[ Columns.column_enabled ] ) )
					row[ Columns.column_enabled ] = enabled;
			}

			return result;
		}

	return Gtk::TreeView::on_button_press_event( event );
}

template < class T_ServersPage > 
void ServersViewer< T_ServersPage >::on_row_activated( const Gtk::TreeModel::Path &path, Gtk::TreeViewColumn *column ) {

	Gtk::TreeView::on_row_activated( path, column );
}

template < class T_ServersPage > 
inline void ServersViewer< T_ServersPage >::inline_set_server( Gtk::TreeModel::Row &row, JE::Server &server ) {

	// Modifico i valori di un server nel visualizzatore
	row[ Columns.column_enabled ] = server.enabled;
	row[ Columns.column_host ] = Glib::locale_to_utf8( server.url );
	row[ Columns.column_ping ] = server.ping;
	row[ Columns.column_files ] = server.files_list.size();
	row[ Columns.column_ref ] = &server;
}

template < class T_ServersPage > 
void ServersViewer< T_ServersPage >::prepend_server( JE::Server &server, bool scroll = false ) {

	// Aggiungo un server al visualizzatore (alla fine della lista)
	Gtk::TreeModel::Row row = *this->list_store->prepend();
	this->inline_set_server( row, server );

	// Mostro la voce nel visualizzatore
	if ( scroll ) this->scroll_to_server( row );
}

template < class T_ServersPage > 
void ServersViewer< T_ServersPage >::append_server( JE::Server &server, bool scroll = false ) {

	// Aggiungo un server al visualizzatore (alla fine della lista)
	Gtk::TreeModel::Row row = *this->list_store->append();
	this->inline_set_server( row, server );

	// Mostro la voce nel visualizzatore
	if ( scroll ) this->scroll_to_server( row );
}

template < class T_ServersPage > 
void ServersViewer< T_ServersPage >::set_server( Gtk::TreeModel::Row &row, JE::Server &server ) {

	// Modifico i valori di un server nel visualizzatore
	this->inline_set_server( row, server );
}


template < class T_ServersPage > 
JE::Server *ServersViewer< T_ServersPage >::get_server_ref( Gtk::TreeModel::Row &row ) {

	// Ritorna l'indice di un server
	return row[ Columns.column_ref ];
}

template < class T_ServersPage > 
void ServersViewer< T_ServersPage >::clear_list() {

	// Pulisco la lista dei server
	this->list_store->clear();
}

template < class T_ServersPage > 
void ServersViewer< T_ServersPage >::scroll_to_server( const std::string &hoster ) {

	// Cerco il server nel visualizzatore
	typedef Gtk::TreeModel::Children type_children;
	Gtk::TreeModel::Row row;
	type_children children = this->list_store->children();
	type_children::iterator iter;

	for ( iter = children.begin(); iter != children.end(); ++iter ) {

		row = *iter;

		if ( hoster == row[ Columns.column_host ] ) {

			// Lo mostro nel visualizzatore
			this->scroll_to_server( row );
			break;
		}
	}
}

template < class T_ServersPage > 
void ServersViewer< T_ServersPage >::scroll_to_server( Gtk::TreeModel::Row &row ) {

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

template < class T_ServersPage > 
inline void ServersViewer< T_ServersPage >::select_all() {

	// Seleziono tutti i server
	this->get_selection()->select_all();
}

template < class T_ServersPage > 
inline void ServersViewer< T_ServersPage >::unselect_all() {

	// Deseleziono tutti i server
	this->get_selection()->unselect_all();
}

#endif
