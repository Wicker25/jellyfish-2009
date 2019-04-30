/* 
	Title: Jellyfish - consts.cpp
	Author: "Giacomo Trudu" aka "Wicker25" - wicker25[at]gmail[dot]com
	Site: http://www.hackyourmind.org/
	License: (CC) BY-NC-SA - http://creativecommons.org/licenses/by-nc-sa/2.5/it/
	Description: Jellyfish is a free download application
*/

#ifndef CONSTS_H
#define CONSTS_H

/* Costanti dell'Engine */
#define APP_TITLE "Jellyfish"
#define APP_VERSION "v0.5.1"
#define APP_AUTHOR "Trudu Giacomo aka \"Wicker25\""
#define APP_SITE "http://www.hackyourmind.org/"
#define APP_EMAIL "< wicker25@gmail.com >"
#define APP_DESCRIPTION "Un'applicazione libera per l'indicizzazione ed il download di file"
#define APP_LICENSE "(CC) BY-NC-SA - http://creativecommons.org/licenses/by-nc-sa/2.5/it/\nPer maggiori informazioni consultare il file LICENSE."

#define USER_AGENT "Jellyfish-Agent/5.0"

#define JELLYFISH_DIRECTORY			( std::string( getenv("HOME") ) + "/.jellyfish/" ).c_str()
#define SETTINGS_PATH 				( std::string( getenv("HOME") ) + "/.jellyfish/engine.cfg" ).c_str()
#define SERVERS_LIST_PATH			( std::string( getenv("HOME") )  + "/.jellyfish/servers.list" ).c_str()
#define FILES_LIST_PATH				( std::string( getenv("HOME") )  + "/.jellyfish/files.list.gz" ).c_str() // Compresso
#define DOWNLOADS_LIST_PATH			( std::string( getenv("HOME") )  + "/.jellyfish/downloads.list" ).c_str()
#define DOWNLOADS_DIRECTORY			( std::string( getenv("HOME") )  + "/.jellyfish/downloads/" ).c_str()
#define SERVER_HEADER "\n[%s]\n"
#define FILE_DATA "%s\t%s\t%s\n"
#define FILE_DATA_BUFFER 2048
#define MAX_LENGTH_SERVER 128
#define COMMENT_CHAR '#'
#define SERVER_SEPARATOR '\t'
#define DOWNLOADS_SEPARATOR "\t"
#define FOLDER "/"

#define CMD_HELP "--help"
#define CMD_H "-h"
#define CMD_VERSION "--version"
#define CMD_V "-v"

#endif
