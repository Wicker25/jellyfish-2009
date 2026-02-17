/* 
	Title: Jellyfish - main.cpp
	Author: "Giacomo Trudu" aka "Wicker25" - wicker25[at]gmail[dot]com
	Site: https://github.com/Wicker25
	License: (CC) BY-NC-SA - http://creativecommons.org/licenses/by-nc-sa/2.5/it/
	Description: Jellyfish is a free download application

	COMPILE:	g++ -lpcre++ `curl-config --cflags --libs`
							 `pkg-config gtkmm-2.4 --cflags --libs`
							 `pkg-config gthread-2.0 --libs` main.cpp -o jelly

*/

#include <cstdio>
#include <gtkmm/main.h>
#include <glibmm/thread.h>

#include "engine/engine.h"
#include "mainwindow.h"

int main( int argc, char *argv[] ) {

	// Inizializzo i Thread
	g_thread_init(NULL);
	gdk_threads_init();

	// Inizializzo l'applicazione gtk
	Gtk::Main kit(argc, argv);

	// Inizializzo il motore di JellyFish
	JE::Engine *ObjEngine = new JE::Engine( argc, argv );

	// Creo la finestra principale ed i suoi widgets
	MainWindow ObjWindow( ObjEngine );

	// Loop principale
    gdk_threads_enter();
	Gtk::Main::run( ObjWindow );
    gdk_threads_leave();

	return 0;
}

