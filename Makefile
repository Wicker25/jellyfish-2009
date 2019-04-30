# JellyFish's Makefile

VERSION = 0.5.1

PCREPP_FLAGS	=	-lpcre++
CURL_FLAGS		=	`curl-config --cflags --libs`
GTKMM_FLAGS		=	`pkg-config gtkmm-2.4 --cflags --libs`
GTHREAD_FLAGS	=	`pkg-config gthread-2.0 --libs`

INSTALL_SH		= ./install.sh
UNINSTALL_SH	= ./uninstall.sh

CXX				=	g++
CXXFLAGS		=	$(PCREPP_FLAGS) $(CURL_FLAGS) $(GTKMM_FLAGS) $(GTHREAD_FLAGS) -fpermissive -lz
SOURCES			=	sources/main.cpp
EXECUTABLE		=	binary/usr/bin/jelly

$(EXECUTABLE) : $(SOURCES)
	$(CXX) $(CXXFLAGS) $(LIBS) $(SOURCES) -o $(EXECUTABLE)

install:
	$(INSTALL_SH)

uninstall:
	$(UNINSTALL_SH)

clean:
	rm -f $(EXECUTABLE)
