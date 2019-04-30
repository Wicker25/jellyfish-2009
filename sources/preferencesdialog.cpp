/* 
	Title: Jellyfish - preferencesdialog.cpp
	Author: "Giacomo Trudu" aka "Wicker25" - wicker25[at]gmail[dot]com
	Site: http://www.hackyourmind.org/
	License: (CC) BY-NC-SA - http://creativecommons.org/licenses/by-nc-sa/2.5/it/
	Description: Jellyfish is a free download application
*/

#ifndef PREFERENCESDIALOG_CPP
#define PREFERENCESDIALOG_CPP

PreferencesDialog::PreferencesDialog( Gtk::Window &parent ) {

	// Imposto le caratteristiche
	this->set_title( PREFERENCE_DIALOG_TITLE );
	this->set_position( Gtk::WIN_POS_CENTER_ON_PARENT );
	this->set_transient_for( parent );

	// Ricavo il contenitore principale
	Gtk::VBox *vbox_ = this->get_vbox();

	// Aggiungo il contenitore delle pagine
	vbox_->pack_start( pages, true, true, 3 );

	// Aggiungo le pagine
	pages.append_page( general_page,	PAGE_GENERAL );
	pages.append_page( connection_page,	PAGE_CONNECTION );

	/** Pagina delle opzioni generali **/
	title_1a.set_markup( TITLE_GENERAL );
	general_page.pack_start( title_1a, false, true, 7 );
	general_page.pack_start( hbox_1a, false, true, 3 );
	hbox_1a.pack_start( vbox_1a, true, true, 10 );
	hbox_1a.pack_start( vbox_1b, true, true, 5 );

	// Numero massimo degli spider
	labels_1[0].set_text( LABEL_MAX_SPIDERS );
	labels_1[0].set_alignment( Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER );
	vbox_1a.pack_start( labels_1[0], true, true, 3 );
	spinbuttons_1[0].set_range( 0, 100 );
	spinbuttons_1[0].set_increments( 1, 10 );
	vbox_1b.pack_start( spinbuttons_1[0] );

	// Numero dei download simultanei
	labels_1[1].set_text( LABEL_MAX_DOWNLOADS );
	labels_1[1].set_alignment( Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER );
	vbox_1a.pack_start( labels_1[1], true, true, 3 );
	spinbuttons_1[1].set_range( 0, 100 );
	spinbuttons_1[1].set_increments( 1, 10 );
	vbox_1b.pack_start( spinbuttons_1[1] );

	// Aggiungo un altro titolo
	title_1b.set_markup( TITLE_CONTENTS );
	general_page.pack_start( title_1b, false, true, 7 );
	general_page.pack_start( hbox_1b, false, true, 3 );
	hbox_1b.pack_start( vbox_1c, true, true, 10 );
	hbox_1b.pack_start( vbox_1d, true, true, 5 );

	// File manager
	labels_1[2].set_text( LABEL_FILE_MANAGER );
	labels_1[2].set_alignment( Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER );
	vbox_1c.pack_start( labels_1[2], true, true, 3 );
	vbox_1d.pack_start( entry_1, true, true, 3 );

	// Limite dei riscontri
	labels_1[3].set_text( LABEL_FOUNDS_LIMIT );
	labels_1[3].set_alignment( Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER );
	vbox_1c.pack_start( labels_1[3], true, true, 3 );
	spinbuttons_1[2].set_range( 1, 1000 );
	spinbuttons_1[2].set_increments( 1, 10 );
	vbox_1d.pack_start( spinbuttons_1[2] );

	/** Pagina delle opzioni sulla connessione **/
	title_2a.set_markup( TITLE_CONNECTION );
	connection_page.pack_start( title_2a, false, true, 7 );
	connection_page.pack_start( hbox_2a, false, true, 3 );
	hbox_2a.pack_start( vbox_2a, true, true, 10 );
	hbox_2a.pack_start( vbox_2b, true, true, 5 );

	// Testo delle etichette
	const char *msg[] = {

		LABEL_SOCKET_CONNECT_TIMEOUT,
		LABEL_SOCKET_TIMEOUT,
		LABEL_SOCKET_REPEAT,
		LABEL_SOCKET_WAIT_REPEAT,
		LABEL_SOCKET_WAIT_READ,
	};

	// Iteratore
	unsigned int i;

	// Aggiungo le voci
	for( i = 0; i < 5; i++ ) {

		// Creo l'etichetta
		labels_2[i].set_text( msg[i] );
		labels_2[i].set_alignment( Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER );
		vbox_2a.pack_start( labels_2[i] );

		// Widget per la modifica
		spinbuttons_2[i].set_range( 0, 120 );
		spinbuttons_2[i].set_increments( 1, 10 );
		vbox_2b.pack_start( spinbuttons_2[i] );
	}

	// Aggiungo un altro titolo
	title_2b.set_markup( TITLE_PRIVACY );
	connection_page.pack_start( title_2b, false, true, 7 );
	connection_page.pack_start( hbox_2b, false, true, 3 );
	hbox_2b.pack_start( vbox_2c, true, true, 10 );
	hbox_2b.pack_start( vbox_2d, true, true, 5 );

	// Indirizzo del proxy
	labels_2[5].set_text( LABEL_SOCKET_PROXY );
	labels_2[5].set_alignment( Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER );
	vbox_2c.pack_start( labels_2[5], true, true, 3 );
	vbox_2d.pack_start( entry_2, true, true, 3 );

	// Porta del proxy
	labels_2[6].set_text( LABEL_SOCKET_PROXY_PORT );
	labels_2[6].set_alignment( Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER );
	vbox_2c.pack_start( labels_2[6], true, true, 3 );
	spinbuttons_2[5].set_range( 0, 65535 );
	spinbuttons_2[5].set_increments( 1, 10 );
	vbox_2d.pack_start( spinbuttons_2[5], true, true, 3 );

	// Aggiungo i pulsanti
	this->add_button( Gtk::Stock::APPLY, 0 );
	this->add_button( Gtk::Stock::CLOSE, 1 );

	// Imposto la risposta predefinita
	this->set_default_response( 1 );

	// Mostro i widgets figli
	this->show_all_children( );
}

PreferencesDialog::~PreferencesDialog() {

}

#endif

