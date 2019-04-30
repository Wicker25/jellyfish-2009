/* 
	Title: Jellyfish - guimsgs.h
	Author: "Giacomo Trudu" aka "Wicker25" - wicker25[at]gmail[dot]com
	Site: http://www.hackyourmind.org/
	License: (CC) BY-NC-SA - http://creativecommons.org/licenses/by-nc-sa/2.5/it/
	Description: Jellyfish is a free download application
*/

#ifndef GUIMSGS_H
#define GUIMSGS_H

/** FINESTRA PRINCIPALE **/
#define WINDOW_TITLE		"JellyFish v0.5"

/** ICONE **/
#define APP_ICON			"/usr/share/pixmaps/jellyfish/app.png"
#define APP_LOGO			"/usr/share/pixmaps/jellyfish/logo.png"

#define ICON_FOLDER			"/usr/share/pixmaps/jellyfish/folder.png"
#define ICON_AUDIO			"/usr/share/pixmaps/jellyfish/audio.png"
#define ICON_VIDEO			"/usr/share/pixmaps/jellyfish/video.png"
#define ICON_IMAGE			"/usr/share/pixmaps/jellyfish/image.png"
#define ICON_DOCUMENT		"/usr/share/pixmaps/jellyfish/document.png"
#define ICON_PACKAGE		"/usr/share/pixmaps/jellyfish/package.png"
#define ICON_EXECUTABLE		"/usr/share/pixmaps/jellyfish/executable.png"
#define ICON_OTHER			"/usr/share/pixmaps/jellyfish/other.png"

#define ICON_WAIT			"/usr/share/pixmaps/jellyfish/wait.png"
#define ICON_START			"/usr/share/pixmaps/jellyfish/start.png"
#define ICON_PAUSE			"/usr/share/pixmaps/jellyfish/pause.png"
#define ICON_STOP			"/usr/share/pixmaps/jellyfish/stop.png"
#define ICON_INTERRUPT		"/usr/share/pixmaps/jellyfish/interrupt.png"
#define ICON_QUEUE			"/usr/share/pixmaps/jellyfish/queue.png"

/** VOCI DEI MENU **/
#define MENU_LABEL_FILE	"File"
#define MENU_LABEL_EDIT	"Modifica"

#define MENU_LABEL_RELOAD	"Ricarica le impostazioni"
#define MENU_LABEL_SAVE		"Salva le impostazioni"
#define MENU_LABEL_HELP		"Aiuto"

//#define MENU_LABEL_SELECTALL	"Seleziona tutto"
#define MENU_LABEL_UNSELECTALL	"Deseleziona tutto"

/** ETICHETTE **/
#define NO_LABEL ""

#define LABEL_NETS		"Reti"
#define LABEL_FIND		"Cerca"
#define LABEL_DOWNLOADS	"Scaricamenti"

#define LABEL_ADDRESS				"Indirizzo"
#define LABEL_NAME					"Nome"
#define LABEL_TYPE					"Tipo"
#define LABEL_STATUS				"Stato"
#define LABEL_SIZE					"Dimensione"
#define LABEL_NOW_SIZE				"Scaricati"
#define LABEL_SPEED					"Velocità"
#define LABEL_PROGRESS				"Progresso"
#define LABEL_HOSTER				"Host"
#define LABEL_PING					"Ping"
#define LABEL_FILES_AND_DIRECTORIES "File e Cartelle"
#define LABEL_FORMAT				"Formato"
#define LABEL_ADVANCED_SEARCH		"Ricerca avanzata"

/** TIPI DI FILE **/
#define LABEL_ALL_TYPE			"Qualsiasi"
#define LABEL_FOLDER_TYPE		"Cartella"
#define LABEL_AUDIO_TYPE		"Audio"
#define LABEL_VIDEO_TYPE		"Video"
#define LABEL_IMAGE_TYPE		"Immagine"
#define LABEL_DOCUMENT_TYPE		"Documento"
#define LABEL_PACKAGE_TYPE		"Archivio"
#define LABEL_EXECUTABLE_TYPE	"Eseguibile"

/** AZIONI GENERICHE **/
#define ACTION_PING		"_Ping"
#define ACTION_INDEX	"_Indicizza"

#define ACTION_DOWNLOAD		"_Scarica"
#define ACTION_INFO			"_Informazioni"

/** MESSAGGI **/
#define MSG_READY "Pronto"
#define MSG_READ_INDICES "Leggo gli indici..."
#define MSG_WRITE_INDICES "Salvo gli indici..."
#define MSG_PING_HOSTER "Ping su "
#define MSG_INDEX_FILES "Indicizzo "
#define MSG_REMOVE_HOSTER "Rimuovo "
#define MSG_FIND "Cerco tra gli indici..."
#define MSG_ADD_DOWNLOAD "Aggiungo il file \"\" alla coda degli scaricamenti..."
#define MSG_RESUME_DOWNLOAD "Riprendo gli scaricamenti..."
#define MSG_OPEN_DOWNLOAD "Apro i file scaricati..."
#define MSG_PAUSE_DOWNLOAD "Fermo momentaneamente gli scaricamenti..."
#define MSG_REMOVE_DOWNLOAD "Rimuovo i file dalla coda degli scaricamenti..."

#define MSG_PROCESSING "Elaborazione..."
#define MSG_WAIT "..."

/** DIALOGHI **/
#define DLG_URL_ERROR_TITLE					"Attenzione: l'indirizzo non e valido!"
#define DLG_URL_ERROR_BODY					"L'indirizzo deve seguire la forma \"http://www.pippo.it/sub/\" ."

#define DLG_DUPLICATE_SERVER_ERROR_TITLE	"Attenzione: il server è già stato indicizzato!"
#define DLG_DUPLICATE_SERVER_ERROR_BODY		"Il server è già stato indicizzato."

/** IMPOSTAZIONI **/
#define PREFERENCE_DIALOG_TITLE			"Preferenze di JellyFish"

#define PAGE_GENERAL					"Generale"
#define PAGE_CONNECTION					"Connessione"

#define TITLE_GENERAL					"<b>Generale</b>"
#define TITLE_CONTENTS					"<b>Contenuti</b>"
#define TITLE_CONNECTION				"<b>Connessione</b>"
#define TITLE_PRIVACY					"<b>Privacy</b>"

#define LABEL_SOCKET_CONNECT_TIMEOUT	"Tempo massimo per la connessione (secondi):"
#define LABEL_SOCKET_TIMEOUT			"Tempo massimo del download (minuti):"
#define LABEL_SOCKET_REPEAT				"Numero di tentativi (solo spiders):"
#define LABEL_SOCKET_WAIT_REPEAT		"Tempo di attesa per il nuovo tentativo (secondi):"
#define LABEL_SOCKET_WAIT_READ			"Frequenza di lettura della connessione (millisecondi):"
#define LABEL_SOCKET_PROXY				"Http proxy:"
#define LABEL_SOCKET_PROXY_PORT			"Porta:"
#define LABEL_MAX_SPIDERS				"Numero massimo di spider per l'indicizzazione:"
#define LABEL_MAX_DOWNLOADS				"Numero massimo di download simultanei:"
#define LABEL_FILE_MANAGER				"File manager (per l'apertura dei file scaricati):"
#define LABEL_FOUNDS_LIMIT				"Limite dei riscontri:"

#endif


