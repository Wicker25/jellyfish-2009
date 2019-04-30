/* 
	Title: Jellyfish - aboutdialog.cpp
	Author: "Giacomo Trudu" aka "Wicker25" - wicker25[at]gmail[dot]com
	Site: http://www.hackyourmind.org/
	License: (CC) BY-NC-SA - http://creativecommons.org/licenses/by-nc-sa/2.5/it/
	Description: Jellyfish is a free download application
*/

#ifndef ABOUTDIALOG_CPP
#define ABOUTDIALOG_CPP

AboutDialog::AboutDialog( Gtk::Window &parent ) {

	// Imposto le caratteristiche
	this->set_title( PREFERENCE_DIALOG_TITLE );
	this->set_position( Gtk::WIN_POS_CENTER_ON_PARENT );
	this->set_transient_for( parent );

	this->set_name( APP_TITLE );
	this->set_logo( Gdk::Pixbuf::create_from_file( APP_LOGO ) );
	this->set_website( APP_SITE );
	this->set_website_label( APP_SITE );
	this->set_version( APP_VERSION );
	this->set_copyright( APP_AUTHOR );
	this->set_comments( APP_DESCRIPTION );
	this->set_license( APP_LICENSE );

	// Mostro i widgets figli
	this->show_all_children( );
}

AboutDialog::~AboutDialog() {

}

#endif

