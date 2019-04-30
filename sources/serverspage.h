/* 
	Title: Jellyfish - serverspage.h
	Author: "Giacomo Trudu" aka "Wicker25" - wicker25[at]gmail[dot]com
	Site: http://www.hackyourmind.org/
	License: (CC) BY-NC-SA - http://creativecommons.org/licenses/by-nc-sa/2.5/it/
	Description: Jellyfish is a free download application
*/

#ifndef SERVERSPAGE_H
#define SERVERSPAGE_H

#include <iostream>

#include "engine/consts.h"
#include "engine/structs.h"

#include <gtkmm/box.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/frame.h>
#include <gtkmm/entry.h>
#include <gtkmm/button.h>
#include <gtkmm/messagedialog.h>

#include "guimsgs.h"
#include "serversviewer.h"

template < class T_MainWindow >
class ServersPage : public Gtk::VBox {

	/* * * * * * * * * * * * * * * * * * *\
	 *         Pagina dei server         *
	\* * * * * * * * * * * * * * * * * * */

public:

	ServersPage( T_MainWindow *ObjMainWindow );
	virtual ~ServersPage();

	// Imposta i server nel visualizzatore
	void set_servers( JE::DataList *servers_list );

	// Aggiunge un nuovo server alla lista
	void add_new_server();
	// Abilita/Disabilita un server
	bool enable_server( JE::Server *server, bool enable );
	// Aggiorna il valore del ping dei server selezionati
	void ping_servers();
	// Aggiorna l'indice dei file dei server selezionati
	void index_files();
	// Rimuove i server selezionati
	void remove_selected_servers();

	// Metodo di accesso sicuro ai membri
	ServersViewer< ServersPage > *get_servers_viewer() const { return this->servers_viewer; }

protected:

	// Modalità di lavoro del thread
	typedef enum {

			PING_MODE,
			INDEX_MODE,
			REMOVE_MODE,

		} PROCESSING_MODE;

	// Puntatore alla finestra principale
	T_MainWindow *ObjMainWindow;
	// Puntatore alla barra d'ingresso
	Gtk::Entry *entry;
	// Puntatore al visualizzatore dei server
	ServersViewer< ServersPage > *servers_viewer;
	// Variabile di controllo del thread di lavoro
	bool thread_is_running;

	// Funzioni di costruzione
	inline void make_entry_bar( Gtk::VBox *vbox );
	inline void make_context_menu();

	// Aggiunge un nuovo server alla lista (Thread)
	void add_new_server_thread( const std::string &hoster, JE::EngineCode *code );
	// Abilita/Disabilita un server (Thread)
	void enable_server_thread( JE::Server *server, bool enable );
	// Aggiorna un valore dei thread selezionati (Thread)
	void processing_thread( PROCESSING_MODE mode );
};

#include "serverspage.cpp"

#endif
