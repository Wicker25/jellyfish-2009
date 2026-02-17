/* 
	Title: Jellyfish - toolbar.h
	Author: "Giacomo Trudu" aka "Wicker25" - wicker25[at]gmail[dot]com
	Site: https://github.com/Wicker25
	License: (CC) BY-NC-SA - http://creativecommons.org/licenses/by-nc-sa/2.5/it/
	Description: Jellyfish is a free download application
*/

#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <gtkmm/toolbar.h>
#include <gtkmm/toolbutton.h>
#include <gtkmm/stock.h>

#include "guimsgs.h"

class Toolbar : public Gtk::Toolbar {

	/* * * * * * * * * * * * * * * * * * *\
	 *       Barra degli strumenti       *
	\* * * * * * * * * * * * * * * * * * */

protected:

	// Puntatori ai pulsanti
	Gtk::ToolButton *servers_tool;
	Gtk::ToolButton *search_tool;
	Gtk::ToolButton *downloads_tool;

public:

	Toolbar();
	virtual ~Toolbar();

	// Metodo di accesso sicuro ai membri
	Gtk::ToolButton *get_servers_tool() const { return this->servers_tool; }
	Gtk::ToolButton *get_search_tool() const { return this->search_tool; }
	Gtk::ToolButton *get_downloads_tool() const { return this->downloads_tool; }
};

#include "toolbar.cpp"

#endif
