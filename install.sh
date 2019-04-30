#!/bin/sh 

echo;

# Controllo se si hanno i privilegi di root
if [ $UID == "0" ]; then echo "Inizio l'installazione del programma...";
else
	echo "Non si hanno i privilegi necessari!";
	exit 1;
fi;

DATA_PATH="binary";

# Verifico i percorsi di destinazione del programma
echo;
echo "Verifico i percorsi d'installazione:";

DIRECTORIES=$(cat "directories.list");

for i in $DIRECTORIES
do
	echo -n " *" "/$i... ";
	if mkdir -p "/$i"; then
		echo "Ok";
	else
		exit 1;
	fi;
done;

# Copio i file del programma
echo;
echo "Copio i file del programma:";

FILES=$(cat "files.list");

for i in $FILES
do
	echo -ne " *" "/$i... ";
	if cp "$DATA_PATH/$i" "/$i"; then
		echo "Ok";
	else
		exit 1;
	fi;
done;

# Aggiorno i menu
echo;
echo -n "Aggiorno i menu... ";

if update-menus --nodpkgchec; then
	echo "Ok";
else
	echo "(W) Impossibile aggiornare i menu!";
fi;

echo;
echo "Il programma è stato installato con successo.";
echo;

exit 0;
