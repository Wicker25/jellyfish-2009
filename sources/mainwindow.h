/* 
	Title: Jellyfish - mainwindow.h
	Author: "Giacomo Trudu" aka "Wicker25" - wicker25[at]gmail[dot]com
	Site: http://www.hackyourmind.org/
	License: (CC) BY-NC-SA - http://creativecommons.org/licenses/by-nc-sa/2.5/it/
	Description: Jellyfish is a free download application
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "engine/consts.h"
#include "engine/engine.h"

#include <gtkmm/box.h>
#include <gtkmm/window.h>
#include <gtkmm/statusbar.h>
#include <gtkmm/image.h>
#include <gtkmm/stock.h>

#include "guimsgs.h"
#include "menubar.h"
#include "toolbar.h"
#include "pagesbox.h"
#include "statusbar.h"
#include "preferencesdialog.h"
#include "aboutdialog.h"

class MainWindow : public Gtk::Window {

	/* * * * * * * * * * * * * * * * * * *\
	 *       Finestra principale         *
	\* * * * * * * * * * * * * * * * * * */

public:

	MainWindow( JE::Engine *ObjEngine );
	virtual ~MainWindow();

	// Icone del tipo di file
	Glib::RefPtr< Gdk::Pixbuf > icon_folder;
	Glib::RefPtr< Gdk::Pixbuf > icon_audio;
	Glib::RefPtr< Gdk::Pixbuf > icon_video;
	Glib::RefPtr< Gdk::Pixbuf > icon_image;
	Glib::RefPtr< Gdk::Pixbuf > icon_document;
	Glib::RefPtr< Gdk::Pixbuf > icon_package;
	Glib::RefPtr< Gdk::Pixbuf > icon_executable;
	Glib::RefPtr< Gdk::Pixbuf > icon_other;

	// Icone di stato
	Glib::RefPtr< Gdk::Pixbuf > icon_wait;
	Glib::RefPtr< Gdk::Pixbuf > icon_start;
	Glib::RefPtr< Gdk::Pixbuf > icon_pause;
	Glib::RefPtr< Gdk::Pixbuf > icon_stop;
	Glib::RefPtr< Gdk::Pixbuf > icon_interrupt;
	Glib::RefPtr< Gdk::Pixbuf > icon_queue;

	// Carica i dati relativi ai server e ai file
	inline void load_data_list();
	// Salva i dati relativi ai server e ai file
	inline void save_data_list();

	// Metodi di accesso sicuro ai membri
	JE::Engine *get_ObjEngine() const { return this->ObjEngine; }
	PagesBox< MainWindow > *get_pages_box() const { return this->pages_box; }
	Statusbar *get_status_bar() const { return this->status_bar; }
	Glib::Thread *get_thread() const { return this->thread; }

protected:

	// Pagine del programma
	typedef enum {

			SERVERS_PAGE,
			SEARCH_PAGE,
			DOWNLOADS_PAGE,

		} PAGES;

	// Tipo delle funzioni generiche
	typedef enum {

			SELECTALL_TYPE,
			UNSELECTALL_TYPE,

		} FUNCTION_TYPE;

	// Modalità di lavoro del thread
	typedef enum {

			SAVE_MODE,
			LOAD_MODE,

		} PROCESSING_MODE;

	// Puntatore al motore di JellyFish
	JE::Engine *ObjEngine;
	// Puntatore al contenitore delle pagine
	PagesBox< MainWindow > *pages_box;
	// Puntatore alla barra di stato
	Statusbar *status_bar;
	// Puntatore al thread di lavoro
	Glib::Thread *thread;

	// Cambia pagina del programma
	void set_page( PAGES page );
	// Mostra la finestra delle preferenze
	void show_preferences_dialog();
	// Mostra la finestra delle informazioni
	void show_about_dialog();
	// Carica i dati relativi ai server e ai file (Thread)
	void processing_data_list( PROCESSING_MODE mode );
	// Esegue le funzioni generiche
	void generic_functions( FUNCTION_TYPE mode );
};

#include "mainwindow.cpp"

#endif
