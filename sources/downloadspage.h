/* 
	Title: Jellyfish - downloadspage.h
	Author: "Giacomo Trudu" aka "Wicker25" - wicker25[at]gmail[dot]com
	Site: http://www.hackyourmind.org/
	License: (CC) BY-NC-SA - http://creativecommons.org/licenses/by-nc-sa/2.5/it/
	Description: Jellyfish is a free download application
*/

#ifndef DOWNLOADSPAGE_H
#define DOWNLOADSPAGE_H

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
#include "downloadsviewer.h"

template < class T_MainWindow >
class DownloadsPage : public Gtk::VBox {

	/* * * * * * * * * * * * * * * * * * *\
	 *         Pagina dei download         *
	\* * * * * * * * * * * * * * * * * * */

public:

	DownloadsPage( T_MainWindow *ObjMainWindow );
	virtual ~DownloadsPage();

	// Aggiunge un file alla coda degli scaricamenti
	void add_download( JE::FoundFile &found );
	// Riprende i download selezionati
	void continue_selected_downloads();
	// Ferma momentaneamente i download selezionati
	void pause_selected_downloads();
	// Apre i file scaricati
	void open_selected_downloads();
	// Rimuove i file selezionati dalla coda degli scaricamenti
	void remove_selected_downloads();
	// Rimuove i file selezionati dalla coda degli scaricamenti e dall'hard disk
	void remove_and_clean_selected_downloads();

	// Metodo di accesso sicuro ai membri
	T_MainWindow *get_ObjMainWindow() const { return this->ObjMainWindow; }
	DownloadsViewer< DownloadsPage > *get_downloads_viewer() const { return this->downloads_viewer; }

protected:

	// Modalità di lavoro del thread
	typedef enum {

			CONTINUE_MODE,
			PAUSE_MODE,
			OPEN_MODE,
			REMOVE_MODE,
			REMOVE_AND_CLEAN_MODE,

		} PROCESSING_MODE;

	// Puntatore alla finestra principale
	T_MainWindow *ObjMainWindow;
	// Puntatore al visualizzatore dei download
	DownloadsViewer< DownloadsPage > *downloads_viewer;
	// Variabile di controllo del thread di lavoro
	bool thread_is_running;

	// Connette il visualizzatore alle tracce
	void *connect_download( JE::DownloadTrack *track, void *row );
	// Aggiorna i progressi del download
	void refresh_download_progress( JE::DownloadTrack *track, double dlnow, double dltotal, double speed, bool complete );
	// Aggiorna lo stato del download
	void refresh_download_status( JE::DownloadTrack *track, JE::DownloadStatus status );
	// Esegue un'azione sui download selezionati (Thread)
	void process_download_thread( PROCESSING_MODE mode );

	// Funzioni di costruzione
	inline void make_context_menu();
};

#include "downloadspage.cpp"

#endif
