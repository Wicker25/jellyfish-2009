/* 
	Title: Jellyfish - regexps.cpp
	Author: "Giacomo Trudu" aka "Wicker25" - wicker25[at]gmail[dot]com
	Site: https://github.com/Wicker25
	License: (CC) BY-NC-SA - http://creativecommons.org/licenses/by-nc-sa/2.5/it/
	Description: Jellyfish is a free download application
*/

#ifndef REGEXPS_CPP
#define REGEXPS_CPP

namespace JE {

using namespace pcrepp;

RegExps::RegExps() {

	/** Inizializzo gli oggetti delle espressioni regolari **/

	// Crawler
	this->ReUrls			= new Pcre( REGEXP_URL, PCRE_CASELESS );
	this->ReNoFile			= new Pcre( REGEXP_NO_FILE, PCRE_CASELESS );
	this->ReNoSubDirectory	= new Pcre( REGEXP_NO_SUB_DIRECTORY, PCRE_CASELESS );

	this->ReHost			= new Pcre( REGEXP_HOST_BASE, PCRE_CASELESS );
	this->ReKey				= new Pcre( "", PCRE_CASELESS );
	this->ReParentDirectory	= new Pcre( "", PCRE_CASELESS );
	this->RePingMs			= new Pcre( REGEXP_PING_MS );
	this->ReServersList		= new Pcre( REGEXP_SERVERS_LIST, PCRE_CASELESS );
	this->ReFilesList[0]	= new Pcre( REGEXP_FILES_LIST_HEAD, PCRE_CASELESS );
	this->ReFilesList[1]	= new Pcre( REGEXP_FILES_LIST_BODY, PCRE_CASELESS );
	this->ReDownloadsList	= new Pcre( REGEXP_DOWNLOADS_LIST, PCRE_CASELESS );

	// Tipi di file
	this->ReAudioTypes		= new Pcre( REGEXP_AUDIO_FILE_TYPES, PCRE_CASELESS );
	this->ReVideoTypes		= new Pcre( REGEXP_VIDEO_FILE_TYPES, PCRE_CASELESS );
	this->ReImageTypes		= new Pcre( REGEXP_IMAGE_FILE_TYPES, PCRE_CASELESS );
	this->ReDocumentTypes	= new Pcre( REGEXP_DOCUMENT_FILE_TYPES, PCRE_CASELESS );
	this->RePackageTypes	= new Pcre( REGEXP_PACKAGE_FILE_TYPES, PCRE_CASELESS );
	this->ReExecutableTypes	= new Pcre( REGEXP_EXECUTABLE_FILE_TYPES, PCRE_CASELESS );
}

RegExps::~RegExps() {

}

void RegExps::set_key( const std::string &key ) {  

	// Imposto una nuova chiave di ricerca
	std::string word = REGEXP_KEY_MODEL;
	word.insert( 3, this->format( key ) );

	*this->ReKey = Pcre( word, PCRE_CASELESS );
}

void RegExps::set_parent_directory( const std::string &directory ) {  

	// Imposto una nuova chiave di ricerca
	std::string word = REGEXP_PARENT_DIRECTORY;
	word.insert( 1, this->format( directory ) );

	*this->ReParentDirectory = Pcre( word, PCRE_CASELESS );
}

std::string RegExps::format( const std::string &key ) {

	// Formatto la chiave di ricerca
	std::string word = key;

	// Converto i caratteri speciali
	this->escape_char( word, "\\.$^()[]+|?-" );

	// Sostituisco i caratteri jolly
	this->replace_char( word, "*", " " );
	this->replace_char( word, " ", REGEXP_SEPARATOR );

	return word;
}

inline void RegExps::escape_char( std::string &word, const std::string &characters ) {

	// Converto i caratteri speciali
	std::string::size_type found;

	found = word.find_first_of( characters );

	while ( found != std::string::npos ) {

		word.insert( found++, "\\" );
		found = word.find_first_of( characters, ++found );
	}
}

inline void RegExps::replace_char( std::string &word, const std::string &char_, const std::string &replacement ) {

	// Converto i caratteri speciali
	std::string::size_type found;

	found = word.find_first_of( char_ );

	while ( found != std::string::npos ) {

		word.replace( found, 1, replacement );
		found = word.find_first_of( char_, ++found );
	}
}

} // namespace JE

#endif
