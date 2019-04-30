/* 
	Title: Jellyfish - preferencesdialog.h
	Author: "Giacomo Trudu" aka "Wicker25" - wicker25[at]gmail[dot]com
	Site: http://www.hackyourmind.org/
	License: (CC) BY-NC-SA - http://creativecommons.org/licenses/by-nc-sa/2.5/it/
	Description: Jellyfish is a free download application
*/

#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <iostream>
#include <gtkmm/dialog.h>
#include <gtkmm/box.h>
#include <gtkmm/notebook.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/stock.h>

#include "guimsgs.h"

class PreferencesDialog : public Gtk::Dialog {

	/* * * * * * * * * * * * * * * * * * *\
	 *    Finestra delle preferenze      *
	\* * * * * * * * * * * * * * * * * * */

protected:

	// Contenitore delle pagine
	Gtk::Notebook pages;

	// Pagina delle opzioni generali
	Gtk::VBox general_page;
	Gtk::Label title_1a, title_1b;
	Gtk::HBox hbox_1a, hbox_1b;
	Gtk::VBox vbox_1a, vbox_1b;
	Gtk::VBox vbox_1c, vbox_1d;
	Gtk::Label labels_1[4];
	Gtk::Entry entry_1;
	Gtk::SpinButton spinbuttons_1[3];

	// Pagina delle opzioni sulla connessione
	Gtk::VBox connection_page;
	Gtk::Label title_2a, title_2b;
	Gtk::HBox hbox_2a, hbox_2b;
	Gtk::VBox vbox_2a, vbox_2b;
	Gtk::VBox vbox_2c, vbox_2d;
	Gtk::Label labels_2[7];
	Gtk::Entry entry_2;
	Gtk::SpinButton spinbuttons_2[6];

public:

	PreferencesDialog( Gtk::Window &parent );
	virtual ~PreferencesDialog();

	// Metodi di accesso sicuro ai valori
	void set_max_spiders( long int *value ) { spinbuttons_1[0].set_value( *value ); }
	long int get_max_spiders() { return (long int) spinbuttons_1[0].get_value_as_int(); }

	void set_max_downloads( long int *value ) { spinbuttons_1[1].set_value( *value ); }
	long int get_max_downloads() { return (long int) spinbuttons_1[1].get_value_as_int(); }

	void set_file_manager( std::string *value ) { entry_1.set_text( *value ); }
	std::string get_file_manager() { return std::string( entry_1.get_text().c_str() ); }

	void set_founds_limit( long int *value ) { spinbuttons_1[2].set_value( *value ); }
	long int get_founds_limit() { return (long int) spinbuttons_1[2].get_value_as_int(); }

	void set_socket_connect_timeout( long int *value ) { spinbuttons_2[0].set_value( *value / 1000 ); }
	long int get_socket_connect_timeout() { return (long int) spinbuttons_2[0].get_value_as_int() * 1000; }

	void set_socket_timeout( long int *value ) { spinbuttons_2[1].set_value( *value / 60000 ); }
	long int get_socket_timeout() { return (long int) spinbuttons_2[1].get_value_as_int() * 60000; }

	void set_socket_repeat( long int *value ) { spinbuttons_2[2].set_value( *value ); }
	long int get_socket_repeat() { return (long int) spinbuttons_2[2].get_value_as_int(); }

	void set_socket_wait_repeat( long int *value ) { spinbuttons_2[3].set_value( *value / 1000000 ); }
	long int get_socket_wait_repeat() {	return (long int) spinbuttons_2[3].get_value_as_int() * 1000000; }

	void set_socket_wait_read( long int *value ) { spinbuttons_2[4].set_value( *value / 1000 );	}
	long int get_socket_wait_read() { return (long int) spinbuttons_2[4].get_value_as_int() * 1000; }

	void set_socket_proxy( std::string *value ) { entry_2.set_text( *value ); }
	std::string get_socket_proxy() { return std::string( entry_2.get_text().c_str() ); }

	void set_socket_proxy_port( long int *value ) { spinbuttons_2[5].set_value( *value ); }
	long int get_socket_proxy_port() { return (long int) spinbuttons_2[5].get_value_as_int(); }
};


#include "preferencesdialog.cpp"

#endif
