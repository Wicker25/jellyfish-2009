/* 
	Title: Jellyfish - menubar.cpp
	Author: "Giacomo Trudu" aka "Wicker25" - wicker25[at]gmail[dot]com
	Site: http://www.hackyourmind.org/
	License: (CC) BY-NC-SA - http://creativecommons.org/licenses/by-nc-sa/2.5/it/
	Description: Jellyfish is a free download application
*/

#ifndef MENUBAR_CPP
#define MENUBAR_CPP

MenuBar::MenuBar() {

	/** Creo la voce "File" **/
	Gtk::MenuItem *menu_label = new Gtk::MenuItem( MENU_LABEL_FILE );
	//menu_label->set_use_underline( true );

	// Creo il menu "File"
	Gtk::Menu *menu = new Gtk::Menu;
	menu_label->set_submenu( *menu );

	// Aggiungo la sotto-voce "Preferenze"
	this->preferences_button = new Gtk::ImageMenuItem( Gtk::Stock::PREFERENCES );
	menu->add( *this->preferences_button );

	// Aggiungo un separatore
	Gtk::SeparatorMenuItem *separator = new Gtk::SeparatorMenuItem;
	menu->add( *separator );

	// Aggiungo la sotto-voce "Esci"
	this->exit_button = new Gtk::ImageMenuItem( Gtk::Stock::QUIT );
	menu->add( *this->exit_button );

	this->add( *menu_label );

	/** Creo la voce "Modifica" **/
	menu_label = new Gtk::MenuItem( MENU_LABEL_EDIT );
	//menu_label->property_use_underline() = true;

	// Creo il menu "Modifica"
	menu = new Gtk::Menu;
	menu_label->set_submenu( *menu );

	// Aggiungo la sotto-voce "Seleziona tutto"
	this->selectall_button = new Gtk::ImageMenuItem( Gtk::Stock::SELECT_ALL );
	menu->add( *this->selectall_button );

	// Aggiungo la sotto-voce "Deseleziona tutto"
	this->unselectall_button = new Gtk::MenuItem( MENU_LABEL_UNSELECTALL );
	menu->add( *this->unselectall_button );

	this->add( *menu_label );

	/** Creo la voce "Aiuto" **/
	menu_label = new Gtk::MenuItem( MENU_LABEL_HELP );
	//menu_label->property_use_underline() = true;

	// Creo il menu "Aiuto"
	menu = new Gtk::Menu;
	menu_label->set_submenu( *menu );

	// Aggiungo la sotto-voce "Informazioni"
	this->about_button = new Gtk::ImageMenuItem( Gtk::Stock::ABOUT );
	menu->add( *this->about_button );
	
	this->add( *menu_label );

	// Mostra la barra dei menu
	this->show_all();
}

MenuBar::~MenuBar() {

}

#endif
