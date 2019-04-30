/* 
	Title: Jellyfish - statusbar.h
	Author: "Giacomo Trudu" aka "Wicker25" - wicker25[at]gmail[dot]com
	Site: http://www.hackyourmind.org/
	License: (CC) BY-NC-SA - http://creativecommons.org/licenses/by-nc-sa/2.5/it/
	Description: Jellyfish is a free download application
*/

#ifndef STATUSBAR_H
#define STATUSBAR_H

#include "engine/consts.h"

#include <gtkmm/box.h>
#include <gtkmm/statusbar.h>
#include <gtkmm/progressbar.h>
#include <glibmm/thread.h>
#include <glibmm/timer.h>
#include <gdkmm/cursor.h>

#include "guimsgs.h"

class Statusbar : public Gtk::Statusbar {

	/* * * * * * * * * * * * * * * * * * *\
	 *          Barra di stato           *
	\* * * * * * * * * * * * * * * * * * */

public:

	Statusbar();
	virtual ~Statusbar();

	// Cambio il messaggio della barra di stato
	inline void change_msg( const Glib::ustring& msg );
	// Imposta lo stato normale
	template < class T_Widget >
	void set_normal_status( T_Widget *widget, bool insensitive );
	// Imposta lo stato di lavoro
	template < class T_Widget >
	void set_work_status( const Glib::ustring& text, T_Widget *widget, bool insensitive );
	// Torno allo stato normale
	template < class T_Widget >
	inline void stop_work( T_Widget *widget, bool insensitive );

protected:

	// Puntatore al thread d'animazione
	Glib::Thread *thread;
	// Mutex per l'accesso sicuro ai membri della classe
	Glib::Mutex work_mutex;
	// Puntatore alla barra del progresso
	Gtk::ProgressBar *progress_bar;

	// Variabili di controllo
	bool end_work;

	// Anima la barra del progresso (Thread)
	template < class T_Widget >
	void pulse_progressbar( T_Widget *widget );
};

#include "statusbar.cpp"

#endif
