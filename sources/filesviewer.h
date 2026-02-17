/* 
	Title: Jellyfish - filesviewer.h
	Author: "Giacomo Trudu" aka "Wicker25" - wicker25[at]gmail[dot]com
	Site: https://github.com/Wicker25
	License: (CC) BY-NC-SA - http://creativecommons.org/licenses/by-nc-sa/2.5/it/
	Description: Jellyfish is a free download application
*/

#ifndef FILESVIEWER_H
#define FILESVIEWER_H

#include <iostream>

#include "engine/structs.h"

#include <gtkmm/treemodel.h>
#include <gtkmm/treestore.h>
#include <gtkmm/treeview.h>
#include <gtkmm/menu.h>

#include "menubar.h"
#include "toolbar.h"
#include "pagesbox.h"

template < class T_FilesPage > 
class FilesViewer : public Gtk::TreeView {

	/* * * * * * * * * * * * * * * * * * *\
	 *	   Visualizzatore dei file     *
	\* * * * * * * * * * * * * * * * * * */

public:

	FilesViewer( T_FilesPage *ObjFilesPage );
	virtual ~FilesViewer();

	// Aggiunge un file all'inizio del visualizzatore
	void prepend_file( JE::FoundFile &found, bool scroll ); 
	// Aggiunge un file alla fine del visualizzatore
	void append_file( JE::FoundFile &found, bool scroll ); 
	// Aggiunge un file ad una cartella del visualizzatore
	void append_into_directory( JE::FoundFile &found, Gtk::TreeModel::Row &parent, bool first );
	// Modifica i valori di un file nel visualizzatore
	void set_file( Gtk::TreeModel::Row &row, JE::FoundFile &found );
	// Controlla se la directory è già stata espansa
	bool directory_is_expanded( Gtk::TreeModel::Row &row );
	// Ricorda che la cartella è stata espansa
	void set_directory_expanded( Gtk::TreeModel::Row &row, bool value );
	// Ritorna la posizione di un file nella lista
	// a partire da una voce del visualizzatore
	JE::FoundFile get_file_ref( Gtk::TreeModel::Row &row );
	// Pulisce la lista dei file
	inline void clear_list();
	// Mostra un file nel visualizzatore
	void scroll_to_file( Gtk::TreeModel::Row &row );
	void scroll_to_file( const std::string &hoster );
	// Seleziona tutti i file
	inline void select_all();
	// Deseleziona tutti i file
	inline void unselect_all();

	// Metodi di accesso sicuro ai membri
	Gtk::Menu *get_context_menu() const { return this->context_menu; }
	Glib::RefPtr< Gtk::TreeStore > get_list_store() const { return this->list_store; }

protected:

	// Puntatore alla pagina dei file
	T_FilesPage *ObjFilesPage;
	// Lista dei file del visualizzatore
	Glib::RefPtr< Gtk::TreeStore > list_store;

	// Struttura delle colonne
	class ModelColumns : public Gtk::TreeModel::ColumnRecord {

		public:

		ModelColumns() {

			// Aggiungo le colonne al modello
			add( column_icon );
			add( column_name );
			add( column_type );
			add( column_hoster );
			add( column_ping );
			add( column_expanded );
			add( column_ref );
		}

		// Colonne
		Gtk::TreeModelColumn< Glib::RefPtr< Gdk::Pixbuf > >		column_icon; // Colonna dell'icona
		Gtk::TreeModelColumn< Glib::ustring >					column_name; // Colonna del nome dei file
		Gtk::TreeModelColumn< Glib::ustring >					column_type; // Colonna del nome dei file
		Gtk::TreeModelColumn< Glib::ustring >					column_hoster; // Colonna del nome dei file
		Gtk::TreeModelColumn< double >							column_ping; // Colonna del tempo di ping
		Gtk::TreeModelColumn< bool >							column_expanded; // Ricorda se la cartella è stata espansa
		Gtk::TreeModelColumn< JE::FoundFile >					column_ref; // Riferimento al file
	};

	ModelColumns Columns;
	// Menu contestuale
	Gtk::Menu *context_menu;

	// Funzione chiamata alla pressione del mouse
	bool on_button_press_event( GdkEventButton* event );
	// Funzione chiamata all'espansione delle cartelle
	void on_row_expanded( const Gtk::TreeModel::iterator &iter, const Gtk::TreeModel::Path &path );
	// Funzione chiamata all'attivazione delle voci
	void on_row_activated( const Gtk::TreeModel::Path &path, Gtk::TreeViewColumn *column );

	// Modifica i valori di un file nel visualizzatore (inline)
	inline void inline_set_file( Gtk::TreeModel::Row &row, JE::FoundFile &found );
	// Modifica l'icona di un file nel visualizzatore (inline)
	inline void inline_set_file_icon( Gtk::TreeModel::Row &row, JE::FoundFile &found );
};

#include "filesviewer.cpp"

#endif
