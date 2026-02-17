/* 
	Title: Jellyfish - aboutdialog.h
	Author: "Giacomo Trudu" aka "Wicker25" - wicker25[at]gmail[dot]com
	Site: https://github.com/Wicker25
	License: (CC) BY-NC-SA - http://creativecommons.org/licenses/by-nc-sa/2.5/it/
	Description: Jellyfish is a free download application
*/

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <iostream>
#include <gtkmm/aboutdialog.h>

#include "guimsgs.h"

class AboutDialog : public Gtk::AboutDialog {

	/* * * * * * * * * * * * * * * * * * *\
	 *    Finestra delle informazioni    *
	\* * * * * * * * * * * * * * * * * * */

public:

	AboutDialog( Gtk::Window &parent );
	virtual ~AboutDialog();
};


#include "aboutdialog.cpp"

#endif
