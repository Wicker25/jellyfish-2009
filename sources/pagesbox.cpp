/* 
	Title: Jellyfish - pagesbox.cpp
	Author: "Giacomo Trudu" aka "Wicker25" - wicker25[at]gmail[dot]com
	Site: https://github.com/Wicker25
	License: (CC) BY-NC-SA - http://creativecommons.org/licenses/by-nc-sa/2.5/it/
	Description: Jellyfish is a free download application
*/

#ifndef PAGESBOX_CPP
#define PAGESBOX_CPP

template < class T_MainWindow >
PagesBox< T_MainWindow >::PagesBox( T_MainWindow *ObjMainWindow ) {

	// Nascondo le "schede" e i bordi del notebook
	this->set_show_tabs( false );
	this->set_show_border( false );

	// Aggiungo la pagina dei server
	this->servers_page = new ServersPage< T_MainWindow >( ObjMainWindow );
	this->append_page( *this->servers_page );

	// Aggiungo la pagina dei server
	this->files_page = new FilesPage< T_MainWindow >( ObjMainWindow );
	this->append_page( *this->files_page );

	// Aggiungo la pagina dei downloads
	this->downloads_page = new DownloadsPage< T_MainWindow >( ObjMainWindow );
	this->append_page( *this->downloads_page );

	// Mostra il contenitore delle pagine
	this->show_all();
}

template < class T_MainWindow > 
PagesBox< T_MainWindow >::~PagesBox() {

}

#endif
