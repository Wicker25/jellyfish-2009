/* 
	Title: Jellyfish - pagesbox.h
	Author: "Giacomo Trudu" aka "Wicker25" - wicker25[at]gmail[dot]com
	Site: http://www.hackyourmind.org/
	License: (CC) BY-NC-SA - http://creativecommons.org/licenses/by-nc-sa/2.5/it/
	Description: Jellyfish is a free download application
*/

#ifndef PAGESBOX_H
#define PAGESBOX_H

#include <gtkmm/notebook.h>

#include "serverspage.h"
#include "filespage.h"
#include "downloadspage.h"

template < class T_MainWindow >
class PagesBox : public Gtk::Notebook {

	/* * * * * * * * * * * * * * * * * * *\
	 *     Contenitore delle pagine      *
	\* * * * * * * * * * * * * * * * * * */

public:

	PagesBox( T_MainWindow *ObjMainWindow );
	virtual ~PagesBox();

	// Metodi di accesso sicuro ai membri
	ServersPage< T_MainWindow > *get_servers_page() const { return this->servers_page; }
	FilesPage< T_MainWindow > *get_files_page() const { return this->files_page; }
	DownloadsPage< T_MainWindow > *get_downloads_page() const { return this->downloads_page; }

protected:

	ServersPage< T_MainWindow > *servers_page;
	FilesPage< T_MainWindow > *files_page;
	DownloadsPage< T_MainWindow > *downloads_page;
};

#include "pagesbox.cpp"

#endif
