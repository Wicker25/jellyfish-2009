/* 
	Title: Jellyfish - regexps.h
	Author: "Giacomo Trudu" aka "Wicker25" - wicker25[at]gmail[dot]com
	Site: https://github.com/Wicker25
	License: (CC) BY-NC-SA - http://creativecommons.org/licenses/by-nc-sa/2.5/it/
	Description: Jellyfish is a free download application
*/

#ifndef REGEXPS_H
#define REGEXPS_H

#include "consts.h"

#include <iostream>
#include <pcre++.h>

#include "structs.h"
#include "streams.h"

#define REGEXP_URL "<[^<]*href=\"([^\"]+(\\.([^\"]+)|/))\"[^>]*>([^<]*)</a>"
#define REGEXP_HOST_BASE "^(http://)([^/:]+)(:\\d+|)(/|)(.*)"
#define REGEXP_PARENT_DIRECTORY "^([^/]+)(\\.([^/]+)|/)$"
#define REGEXP_KEY_MODEL ".*().*"
#define REGEXP_SEPARATOR "(.*)"
#define REGEXP_NO_SUB_DIRECTORY "^(/|\\.\\./|(\\w+)://)(.*)"
#define REGEXP_NO_FILE "^(mailto:)"

#define REGEXP_SERVERS_LIST "^(.+)\t([0-1])\t(\\d+(\\.\\d+|))([ ]*)$"
#define REGEXP_FILES_LIST_HEAD "^\\[(.+)\\]([ ]*)$"
#define REGEXP_FILES_LIST_BODY "^([^\t]+)[\t]+([^\t]+)[\t]+([^\t\t\n]+)([ ]*)$"
#define REGEXP_DOWNLOADS_LIST "^([^\t]+)[\t]+([^\t]+)[\t]+([^\t]+)[\t]+([^\t]+)[\t]+([0-4]+)([ ]*)$"
#define REGEXP_PING_MS "time=(.+) ms"

#define REGEXP_AUDIO_FILE_TYPES "^(aac|ac3|aiff|amr|bwf|cda|flac|iff|mid|mka|mp3|m4a|riff|pca|ra|ogg|wav|wma|xm)$"
#define REGEXP_VIDEO_FILE_TYPES "^(rm|gp3|mpg|mpeg|asf|avi|swf|wmv|mov|vob|svi|mkv)$"
#define REGEXP_IMAGE_FILE_TYPES "^(bmp|gif|jpeg|jpg|pict|png|tga|tiff|pcx|cpd|jpd|gpd|ai|cdr|drv|dgn|dwg|dxf|edrw|flt|fla|igs|lfp|par|prt|sat|stl|svg)$"
#define REGEXP_DOCUMENT_FILE_TYPES "^(doc|odt|ott|sxw|stw|rtf|sdw|vor|txt|pdf|html|htm|xml|fodt|uot|ppt|xls)$"
#define REGEXP_PACKAGE_FILE_TYPES "^(bz2|gz|tar|zip|jar|rar|7z|ace|arj|arc|bin|cab|hqx|iso|lha|mdf|sit|zoo)$"
#define REGEXP_EXECUTABLE_FILE_TYPES "^(out|bat|coff|com|cmd|msi|sis|dll|elf|exe|jar|bin|le|mz|ne|pe|app)$"

namespace JE {

using namespace pcrepp;

class RegExps {

	/* * * * * * * * * * * * * * * * * * * *\
	 * Gestore delle espressioni regolari  *
	\* * * * * * * * * * * * * * * * * * * */

protected:

	/** Gli oggetti delle espressioni regolari **/
	
	// Crawler
	Pcre *ReUrls;
	Pcre *ReHost;
	Pcre *ReKey;
	Pcre *ReParentDirectory;
	Pcre *RePingMs;
	Pcre *ReServersList;
	Pcre *ReFilesList[2];
	Pcre *ReDownloadsList;
	Pcre *ReNoSubDirectory;
	Pcre *ReNoFile;
	
	// Tipi di file
	Pcre *ReAudioTypes;
	Pcre *ReVideoTypes;
	Pcre *ReImageTypes;
	Pcre *ReDocumentTypes;
	Pcre *RePackageTypes;
	Pcre *ReExecutableTypes;

public:

	RegExps();
	~RegExps();

	// Imposta una nuova chiave di ricerca
	void set_key( const std::string &key );
	// Imposta la directory madre da cui cercare gli indirizzi
	void set_parent_directory( const std::string &directory );
	// Formatta la chiave di ricerca
	std::string format( const std::string &key );
	// Converte i caratteri speciali in una stringa
	inline void escape_char( std::string &word, const std::string &characters );
	// Sostituisce un carattere in una stringa
	inline void replace_char( std::string &word, const std::string &char_, const std::string &replacement );

	// Restituisce una copia di un oggetto regexp (per i Thread)
	inline Pcre get_ReUrls() { return *this->ReUrls; }
	inline Pcre get_ReNoSubDirectory() { return *this->ReNoSubDirectory; }
	inline Pcre get_ReNoFile() { return *this->ReNoFile; }

	// Accesso sicuro agli elementi
	inline Pcre *get_ReHost() { return this->ReHost; }
	inline Pcre *get_ReKey() { return this->ReKey; }
	inline Pcre *get_ReParentDirectory() { return this->ReParentDirectory; }
	inline Pcre *get_RePingMs() { return this->RePingMs; }
	inline Pcre *get_ReServersList() { return this->ReServersList; }
	inline Pcre **get_ReFilesList() { return this->ReFilesList; }
	inline Pcre *get_ReDownloadsList() { return this->ReDownloadsList; }

	inline Pcre *get_ReAudioTypes() { return this->ReAudioTypes; }
	inline Pcre *get_ReVideoTypes() { return this->ReVideoTypes; }
	inline Pcre *get_ReImageTypes() { return this->ReImageTypes; }
	inline Pcre *get_ReDocumentTypes() { return this->ReDocumentTypes; }
	inline Pcre *get_RePackageTypes() { return this->RePackageTypes; }
	inline Pcre *get_ReExecutableTypes() { return this->ReExecutableTypes; }
};

} // namespace JE

#include "regexps.cpp"

#endif
