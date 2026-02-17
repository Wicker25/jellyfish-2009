/* 
	Title: Jellyfish - downloadsviewer.h
	Author: "Giacomo Trudu" aka "Wicker25" - wicker25[at]gmail[dot]com
	Site: https://github.com/Wicker25
	License: (CC) BY-NC-SA - http://creativecommons.org/licenses/by-nc-sa/2.5/it/
	Description: Jellyfish is a free download application
*/

#ifndef DOWNLOADSVIEWER_H
#define DOWNLOADSVIEWER_H

#include <iostream>

#include "engine/structs.h"

#include <gtkmm/treemodel.h>
#include <gtkmm/treestore.h>
#include <gtkmm/treeview.h>
#include <gtkmm/cellrendererprogress.h>
#include <gtkmm/menu.h>

#include "menubar.h"
#include "toolbar.h"
#include "pagesbox.h"

template < class T_DownloadsPage > 
class DownloadsViewer : public Gtk::TreeView {

	/* * * * * * * * * * * * * * * * * * *\
	 *	 Visualizzatore dei download     *
	\* * * * * * * * * * * * * * * * * * */

public:

	DownloadsViewer( T_DownloadsPage *ObjDownloadsPage );
	virtual ~DownloadsViewer();

	// Aggiunge un download all'inizio del visualizzatore
	Gtk::TreeModel::Row *append_download( JE::DownloadInfo &download, bool scroll ); 
	// Aggiunge un download alla fine del visualizzatore
	Gtk::TreeModel::Row *append_download_with_ref( JE::DownloadTrack *track, void *row ); 
	// Modifica i valori di un download nel visualizzatore
	void set_download( Gtk::TreeModel::Row &row, JE::DownloadInfo &download );
	// Imposta lo stato del download
	void set_status( Gtk::TreeModel::Row &row, JE::DownloadStatus status );
	// Ritorna lo stato del download
	JE::DownloadStatus get_status( Gtk::TreeModel::Row &row );
	// Aggiorna le informazioni dei download
	inline void set_download_progress( Gtk::TreeModel::Row &row, double &dlnow, double &dltotal, 
										double &speed, bool &complete );
	// Aggiorna lo stato dei download
	inline void set_download_status( Gtk::TreeModel::Row &row, JE::DownloadStatus &status );
	// Imposta il riferimento ad un download dell'engine
	void set_download_ref( Gtk::TreeModel::Row &row, JE::DownloadTrack *ref );
	// Ritorna il puntatore ad un download
	// a partire da una voce del visualizzatore
	JE::DownloadTrack *get_download_ref( Gtk::TreeModel::Row &row );
	// Rimuove un download dal visualizzatore
	inline void remove_download( Gtk::TreeModel::Row &row )	{ this->tree_store->erase( *row ); }
	// Pulisce la lista dei download
	inline void clear_list();
	// Mostra un download nel visualizzatore
	void scroll_to_download( Gtk::TreeModel::Row &row );
	void scroll_to_download( const std::string &hoster );
	// Seleziona tutti i download
	inline void select_all();
	// Deseleziona tutti i download
	inline void unselect_all();

	// Metodi di accesso sicuro ai membri
	Gtk::Menu *get_context_menu() const { return this->context_menu; }
	Glib::RefPtr< Gtk::TreeStore > get_tree_store() const { return this->tree_store; }

protected:

	// Puntatore alla pagina dei download
	T_DownloadsPage *ObjDownloadsPage;
	// Lista dei download del visualizzatore
	Glib::RefPtr< Gtk::TreeStore > tree_store;

	// Struttura delle colonne
	class ModelColumns : public Gtk::TreeModel::ColumnRecord {

		public:

		ModelColumns() {

			// Aggiungo le colonne al modello
			add( column_icon );
			add( column_name );
			add( column_status );
			add( column_status_icon );
			add( column_now_size );
			add( column_size );
			add( column_speed );
			add( column_progress );
			add( column_ref );
		}

		// Colonne
		Gtk::TreeModelColumn< Glib::RefPtr< Gdk::Pixbuf > >		column_icon; // Colonna dell'icona
		Gtk::TreeModelColumn< Glib::ustring >					column_name; // Colonna del nome dei file
		Gtk::TreeModelColumn< JE::DownloadStatus >					column_status; // Colonna dello stato
		Gtk::TreeModelColumn< Glib::RefPtr< Gdk::Pixbuf > >		column_status_icon; // Colonna dell'icona dello stato
		Gtk::TreeModelColumn< double >							column_now_size; // Colonna della dimensione attuale
		Gtk::TreeModelColumn< double >							column_size; // Colonna della dimensione del file
		Gtk::TreeModelColumn< unsigned int >					column_speed; // Colonna della velocità di scaricamento
		Gtk::TreeModelColumn< unsigned int >					column_progress; // Colonna del progresso
		Gtk::TreeModelColumn< JE::DownloadTrack * >				column_ref; // Riferimento al download
	};

	ModelColumns Columns;
	// Menu contestuale
	Gtk::Menu *context_menu;

	// Funzione chiamata alla pressione del mouse
	bool on_button_press_event( GdkEventButton* event );
	// Funzione chiamata alla pressione di un tasto
	bool on_key_press_event( GdkEventKey* event );
	// Funzione chiamata all'abilitazione/disabilitazione di un download
	void on_row_activated( const Gtk::TreeModel::Path &path, Gtk::TreeViewColumn *column );

	// Modifica i valori di un download nel visualizzatore (inline)
	inline void inline_set_download( Gtk::TreeModel::Row &row, JE::DownloadInfo &download );
	// Modifica l'icona di un file nel visualizzatore (inline)
	inline void inline_set_file_icon( Gtk::TreeModel::Row &row, JE::DownloadInfo &download );
};

#include "downloadsviewer.cpp"

#endif
