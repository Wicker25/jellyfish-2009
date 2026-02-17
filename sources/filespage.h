/* 
	Title: Jellyfish - filespage.h
	Author: "Giacomo Trudu" aka "Wicker25" - wicker25[at]gmail[dot]com
	Site: https://github.com/Wicker25
	License: (CC) BY-NC-SA - http://creativecommons.org/licenses/by-nc-sa/2.5/it/
	Description: Jellyfish is a free download application
*/

#ifndef FILESPAGE_H
#define FILESPAGE_H

#include <iostream>

#include "engine/consts.h"
#include "engine/structs.h"

#include <gtkmm/box.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/frame.h>
#include <gtkmm/entry.h>
#include <gtkmm/button.h>
#include <gtkmm/messagedialog.h>
#include <gtkmm/expander.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/label.h>

#include "guimsgs.h"
#include "filesviewer.h"

template < class T_MainWindow >
class FilesPage : public Gtk::VBox {

	/* * * * * * * * * * * * * * * * * * *\
	 *          Pagina dei file          *
	\* * * * * * * * * * * * * * * * * * */

public:

	FilesPage( T_MainWindow *ObjMainWindow );
	virtual ~FilesPage();

	// Mostra il contenuto di una directory
	bool show_directory_files( const Gtk::TreeModel::iterator &iter );
	// Pulisce la lista dei riscontri
	void clear_founds();
	// Aggiunge un nuovo file alla coda dei download
	void download_files();

	// Metodo di accesso sicuro ai membri
	T_MainWindow *get_ObjMainWindow() const { return this->ObjMainWindow; }
	FilesViewer< FilesPage > *get_files_viewer() const { return this->files_viewer; }

protected:

	// Modalità di lavoro del thread
	typedef enum {

			DOWNLOAD_MODE,

		} PROCESSING_MODE;

	// Puntatore alla finestra principale
	T_MainWindow *ObjMainWindow;
	// Puntatore alla barra d'ingresso
	Gtk::Entry *entry;
	// Scelta multipla del tipo di file
	Gtk::ComboBoxText *combo_box;
	// Puntatore al visualizzatore dei file
	FilesViewer< FilesPage > *files_viewer;
	// Variabile di controllo del thread di lavoro
	bool thread_is_running;

	// Funzioni di costruzione
	inline void make_entry_bar( Gtk::VBox *vbox );
	inline void make_context_menu();

	// Cerca un file indicizzato
	void find_file();
	// Cerca un file indicizzato (Thread)
	void find_file_thread( const std::string &key, JE::FileType &type, JE::EngineCode &code );
	// Mostra il contenuto di una directory (Thread)
	void show_directory_files_thread( Gtk::TreeModel::iterator &iter, JE::EngineCode &code );

	// Esegue un'operazione sui thread selezionati (Thread)
	void processing_thread( PROCESSING_MODE mode );
};

#include "filespage.cpp"

#endif
