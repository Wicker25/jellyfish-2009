/* 
	Title: Jellyfish - statusbar.cpp
	Author: "Giacomo Trudu" aka "Wicker25" - wicker25[at]gmail[dot]com
	Site: https://github.com/Wicker25
	License: (CC) BY-NC-SA - http://creativecommons.org/licenses/by-nc-sa/2.5/it/
	Description: Jellyfish is a free download application
*/

#ifndef STATUSBAR_CPP
#define STATUSBAR_CPP

Statusbar::Statusbar() {

	// Inizializzo le variabili di controllo
	this->thread = NULL;
	this->end_work = false;

	// Creo la barra dei progressi
	this->progress_bar = new Gtk::ProgressBar;
	this->progress_bar->set_size_request( 100, 17 );
	this->pack_start( *this->progress_bar, false, true );

	// Mostro la barra di stato
	this->show_all();
}

Statusbar::~Statusbar() {

}

inline void Statusbar::change_msg( const Glib::ustring& msg ) {

	this->pop(); this->push( msg );
}

template < class T_Widget >
void Statusbar::set_normal_status( T_Widget *widget, bool insensitive = true ) {

	// Nascondo la barra dei progressi
	this->progress_bar->hide();

	// Cambio il messaggio
	this->change_msg( MSG_READY );

	if ( widget != NULL ) {

		// Rendo sensibile il widget scelto
		if ( insensitive ) widget->set_sensitive( true );

		// Cambio il cursore
		Glib::RefPtr< Gdk::Window > ref_window;
		ref_window = widget->get_window();
		ref_window->set_cursor( Gdk::Cursor( Gdk::LEFT_PTR ) );
	}

	// Reinizializzo la variabile di controllo
	this->end_work = false;
}

template < class T_Widget >
void Statusbar::set_work_status( const Glib::ustring& msg, T_Widget *widget, bool insensitive = true ) {

	// Attende il termine degli altri thread
	if ( this->thread != NULL ) return; // this->thread->join();

	// Mostro la barra dei progressi
	this->progress_bar->show();

	// Cambio il messaggio
	this->change_msg( msg );

	if ( widget != NULL ) {

		// Rendo insensibile il widget scelto
		if ( insensitive ) widget->set_sensitive( false );

		// Cambio il cursore
		Glib::RefPtr< Gdk::Window > ref_window;
		ref_window = widget->get_window();
		ref_window->set_cursor( Gdk::Cursor( Gdk::WATCH ) );
	}

	// Avvio il thread per l'animazione della barra del progresso
	this->thread = Glib::Thread::create(
		sigc::bind( sigc::mem_fun( this, &Statusbar::pulse_progressbar< T_Widget > ), widget ), true );
}

template < class T_Widget >
void Statusbar::pulse_progressbar( T_Widget *widget = NULL ) {

	// Ciclo fino al termine del thread
	while ( true ) {

		// Piccola pausa
		Glib::usleep( 50000 );

		// Animo la barra del progresso
		gdk_threads_enter();
		this->progress_bar->pulse();
		gdk_threads_leave();

		// Condizione di uscita
		if ( this->end_work ) break;
	}

	// Riazzera il puntatore al thread
	this->thread = NULL;
}

template < class T_Widget >
inline void Statusbar::stop_work( T_Widget *widget, bool insensitive = true ) {

	// Fermo il thread modificando la variabile di controllo
	this->end_work = true;

	// Attende il termine del thread di animazione
	if ( this->thread != NULL ) this->thread->join();

	// Torno allo stato normale
	gdk_threads_enter();
	this->set_normal_status( widget, insensitive );
	gdk_threads_leave();
}

#endif
