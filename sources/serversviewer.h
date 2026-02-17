/* 
	Title: Jellyfish - serversviewer.h
	Author: "Giacomo Trudu" aka "Wicker25" - wicker25[at]gmail[dot]com
	Site: https://github.com/Wicker25
	License: (CC) BY-NC-SA - http://creativecommons.org/licenses/by-nc-sa/2.5/it/
	Description: Jellyfish is a free download application
*/

#ifndef SERVERSVIEWER_H
#define SERVERSVIEWER_H

#include <iostream>

#include "engine/structs.h"

#include <gtkmm/treemodel.h>
#include <gtkmm/liststore.h>
#include <gtkmm/treeview.h>
#include <gtkmm/menu.h>

#include "menubar.h"
#include "toolbar.h"
#include "pagesbox.h"

template < class T_ServersPage > 
class ServersViewer : public Gtk::TreeView {

	/* * * * * * * * * * * * * * * * * * *\
	 *	   Visualizzatore dei server     *
	\* * * * * * * * * * * * * * * * * * */

public:

	ServersViewer( T_ServersPage *ObjServersPage );
	virtual ~ServersViewer();

	// Aggiunge un server all'inizio del visualizzatore
	void prepend_server( JE::Server &server, bool scroll ); 
	// Aggiunge un server alla fine del visualizzatore
	void append_server( JE::Server &server, bool scroll ); 
	// Modifica i valori di un server nel visualizzatore
	void set_server( Gtk::TreeModel::Row &row, JE::Server &server );
	// Rimuove un server nel visualizzatore
	void remove_server( Gtk::TreeModel::Row &row ) { this->list_store->erase( row ); };
	// Ritorna la posizione di un server nella lista
	// a partire da una voce del visualizzatore
	JE::Server *get_server_ref( Gtk::TreeModel::Row &row );
	// Pulisce la lista dei server
	inline void clear_list();
	// Mostra un server nel visualizzatore
	void scroll_to_server( Gtk::TreeModel::Row &row );
	void scroll_to_server( const std::string &hoster );
	// Seleziona tutti i server
	inline void select_all();
	// Deseleziona tutti i server
	inline void unselect_all();

	// Metodi di accesso sicuro ai membri
	Gtk::Menu *get_context_menu() const { return this->context_menu; }
	Glib::RefPtr< Gtk::ListStore > get_list_store() const { return this->list_store; }

protected:

	// Puntatore alla pagina dei server
	T_ServersPage *ObjServersPage;
	// Lista dei server del visualizzatore
	Glib::RefPtr< Gtk::ListStore > list_store;

	// Struttura delle colonne
	class ModelColumns : public Gtk::TreeModel::ColumnRecord {

		public:

		ModelColumns() {

			// Aggiungo le colonne al modello
			add( column_enabled );
			add( column_host );
			add( column_ping );
			add( column_files );
			add( column_ref );
		}

		// Colonne
		Gtk::TreeModelColumn< bool >			column_enabled; // Colonna di abilitazione
		Gtk::TreeModelColumn< Glib::ustring >	column_host; // Colonna del nome dei server
		Gtk::TreeModelColumn< double >			column_ping; // Colonna del tempo di ping
		Gtk::TreeModelColumn< unsigned int >	column_files; // Colonna del numero dei file
		Gtk::TreeModelColumn< JE::Server * >	column_ref; // Riferimento al server
	};

	ModelColumns Columns;
	// Menu contestuale
	Gtk::Menu *context_menu;

	// Funzione chiamata alla pressione del mouse
	bool on_button_press_event( GdkEventButton* event );
	// Funzione chiamata alla pressione di un tasto
	bool on_key_press_event( GdkEventKey* event );
	// Funzione chiamata all'abilitazione/disabilitazione di un server
	void on_row_activated( const Gtk::TreeModel::Path &path, Gtk::TreeViewColumn *column );

	// Modifica i valori di un server nel visualizzatore (inline)
	inline void inline_set_server( Gtk::TreeModel::Row &row, JE::Server &server );
};

#include "serversviewer.cpp"

#endif
