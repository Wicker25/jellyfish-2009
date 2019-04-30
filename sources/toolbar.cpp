/* 
	Title: Jellyfish - toolbar.cpp
	Author: "Giacomo Trudu" aka "Wicker25" - wicker25[at]gmail[dot]com
	Site: http://www.hackyourmind.org/
	License: (CC) BY-NC-SA - http://creativecommons.org/licenses/by-nc-sa/2.5/it/
	Description: Jellyfish is a free download application
*/

#ifndef TOOLBAR_CPP
#define TOOLBAR_CPP

Toolbar::Toolbar() {

	// Puntatore ad un oggetto immagine
	Gtk::Image *image;

	// Aggiungo il pulsante per la pagina dei server
	image = new Gtk::Image( Gtk::Stock::NETWORK, Gtk::ICON_SIZE_LARGE_TOOLBAR );

	this->servers_tool = new Gtk::ToolButton( *image, LABEL_NETS );
	this->append( *this->servers_tool );
	this->servers_tool->show();

	// Aggiungo il pulsante per la pagina di ricerca dei file
	image = new Gtk::Image( Gtk::Stock::FIND, Gtk::ICON_SIZE_LARGE_TOOLBAR );

	this->search_tool = new Gtk::ToolButton( *image, LABEL_FIND );
	this->append( *this->search_tool );
	this->search_tool->show();

	// Aggiungo il pulsante per la pagina dei downloads
	image = new Gtk::Image( Gtk::Stock::SAVE, Gtk::ICON_SIZE_LARGE_TOOLBAR );

	this->downloads_tool = new Gtk::ToolButton( *image, LABEL_DOWNLOADS  );
	this->append( *this->downloads_tool );

	// Mostro sla barra degli strumenti
	this->show_all();
}

Toolbar::~Toolbar() {

}


#endif
