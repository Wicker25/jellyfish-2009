/* 
	Title: Jellyfish - menubar.h
	Author: "Giacomo Trudu" aka "Wicker25" - wicker25[at]gmail[dot]com
	Site: http://www.hackyourmind.org/
	License: (CC) BY-NC-SA - http://creativecommons.org/licenses/by-nc-sa/2.5/it/
	Description: Jellyfish is a free download application
*/

#ifndef MENUBAR_H
#define MENUBAR_H

#include <gtkmm/menubar.h>
#include <gtkmm/menu.h>
#include <gtkmm/menuitem.h>
#include <gtkmm/stock.h>

#include "guimsgs.h"

class MenuBar : public Gtk::MenuBar {

	/* * * * * * * * * * * * * * * * * * *\
	 *           Barra dei menu'         *
	\* * * * * * * * * * * * * * * * * * */

public:

	MenuBar();
	virtual ~MenuBar();

	// Metodi di accesso sicuro ai membri
	Gtk::ImageMenuItem *get_preferences_button() const { return this->preferences_button; }
	Gtk::ImageMenuItem *get_exit_button() const { return this->exit_button; }

	Gtk::ImageMenuItem *get_selectall_button() const { return this->selectall_button; }
	Gtk::MenuItem *get_unselectall_button() const { return this->unselectall_button; }

	Gtk::MenuItem *get_about_button() const { return this->about_button; }

protected:

	// Voci del menu "File"
	Gtk::ImageMenuItem *preferences_button;
	Gtk::ImageMenuItem *exit_button;

	// Voci del menu "Modifica"
	Gtk::ImageMenuItem *selectall_button;
	Gtk::MenuItem *unselectall_button;

	// Voci del menu "Aiuto"
	Gtk::ImageMenuItem *about_button;
};

#include "menubar.cpp"

#endif
