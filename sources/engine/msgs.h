/* 
	Title: Jellyfish - msgs.h
	Author: "Giacomo Trudu" aka "Wicker25" - wicker25[at]gmail[dot]com
	Site: https://github.com/Wicker25
	License: (CC) BY-NC-SA - http://creativecommons.org/licenses/by-nc-sa/2.5/it/
	Description: Jellyfish is a free download application
*/

#ifndef MSGS_H
#define MSGS_H

#include "consts.h"

#define E_HEADER_SERVERS_LIST "# ** LISTA DEI SERVER **\n# Indirizzo - Abilitato/Disabilitato - Valore del ping\n# ...\n"
#define E_HEADER_FILES_LIST "# ** LISTA DEGLI INDICI **\n# [:Indirizzo del server:]\n# Nome - Tipo - Percorso\n# ...\n"
#define E_HEADER_DOWNLOADS_LIST "# ** LISTA DEI DOWNLOAD **\n# NON MODIFICARE QUESTO FILE !!!\n# Nome - Tipo - Percorso - Stato - Sorgente\n# ...\n"

#define E_MSG_OPEN_SETTINGS_ERROR  "<Stream>  (!!) Impossibile leggere il file delle impostazioni.\n"
#define E_MSG_OPEN_SERVERS_LIST_ERROR  "<Stream>  (!!) Impossibile leggere la lista dei server.\n"
#define E_MSG_OPEN_FILES_LIST_ERROR "<Stream>  (!!) Impossibile leggere la lista dei file.\n"
#define E_MSG_OPEN_DOWNLOADS_LIST_ERROR "<Stream>  (!!) Impossibile leggere la lista dei download.\n"

#define E_MSG_READ_SETTINGS "<Stream> Leggo le impostazioni... \n"
#define E_MSG_WRITE_SETTINGS "<Stream> Salvo le impostazioni... \n"
#define E_MSG_READ_SERVERS_LIST "<Stream> Leggo la lista dei server...\n"
#define E_MSG_WRITE_SERVERS_LIST "<Stream> Salvo la lista dei server...\n"
#define E_MSG_READ_FILES_LIST "<Stream> Leggo la lista dei file... \n"
#define E_MSG_WRITE_FILES_LIST "<Stream> Salvo la lista dei file... \n"
#define E_MSG_READ_DOWNLOADS_LIST "<Stream> Leggo la lista dei download... \n"
#define E_MSG_WRITE_DOWNLOADS_LIST "<Stream> Salvo la lista dei download... \n"

#define E_MSG_SYNTAX_ERROR "<Stream> (W) \"%s\":%d: Sintassi errata.\n"

#define E_MSG_EMPTY_SERVERS_LIST "<Stream> (W) Nessun server in lista.\n"
#define E_MSG_EMPTY_FILES_LIST "<Stream> (W) Nessun file indicizzato.\n"
#define E_MSG_DUPLICATE_SERVER "<Stream> (W) \"%s\":%d: Server ripetuto: \"%s\"\n"
#define E_MSG_UNKNOWN_SERVER "<Stream> (W) \"%s\":%d: Server non definito: \"%s\"\n"
#define E_MSG_DUPLICATE_DOWNLOAD "<Stream> (W) \"%s\":%d: Download ripetuto: \"%s\"\n"

#define E_MSG_NO_FILES_INDEXED "<Stream>  (W) Nessun file indicizzato.\n"
#define E_MSG_MAKE_DIRECTORY "<Stream>  Creo la directory \"%s\"...\n"

#define E_MSG_UNABLE_TO_RESOLVE_HOST "<Socket> (!!) Impossibile trovare il server.\n\a\a"
#define E_MSG_UNABLE_TO_CONNECT "<Socket> (!!) Impossibile connettersi al server.\n\a\a"
#define E_MSG_NEW_ATTEMPT "<Socket> [%s] (W) Connessione persa. Nuovo tentativo (%d) tra 3 secondi...\n\a"
#define E_MSG_PING_SERVER "<Crawler> Ping su %s...\n"
#define E_MSG_EXTRACT_FROM "<Crawler> Estraggo da %s ...\n"
#define E_MSG_GET_SUB_FILES "<Crawler> Controllo il contenuto di %s ...\n"

#define E_MSG_ADD_DOWNLOAD "<Downloader> Aggiungo \"%s\"...\n"
#define E_MSG_PAUSE_DOWNLOAD "<Downloader> Sospendo \"%s\"...\n"
#define E_MSG_RESUME_DOWNLOAD "<Downloader> Ripristino \"%s\"...\n"
#define E_MSG_REMOVE_DOWNLOAD "<Downloader> Rimuovo \"%s\"...\n"
#define E_MSG_REMOVE_FILE "<Crawler> Rimuovo il file dall'hard disk...\n"
#define E_MSG_REMOVE_DIRECTORY "<Crawler> Rimuovo la cartella dall'hard disk...\n"
#define E_MSG_START_DOWNLOAD_THREAD "<Downloader> Avvio il thread degli scaricamenti...\n"
#define E_MSG_END_DOWNLOAD_THREAD "<Downloader> Fermo il thread degli scaricamenti...\n"
#define E_MSG_DOWNLOAD_THREAD_ERROR "<Downloader> (!!) Impossibile avviare il thread di download.\n"
#define E_MSG_DOWNLOAD_CONTENT_TYPE_ERROR "<Downloader> (!!) Il \"content-type\" non coincide.\n"

#endif
