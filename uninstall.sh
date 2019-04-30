#!/bin/sh 

echo;

# Controllo se si hanno i privilegi di root
if [ $UID -eq "0" ]; then echo "Inizio la rimozione del programma...";
else
	echo "Non si hanno i privilegi necessari!";
	exit 1;
fi;

DATA_PATH="binary";

# Rimuovo i file del programma
echo;
echo "Rimuovo i file:";

FILES=$(cat "files.list");

for i in $FILES
do
	echo -n " *" "/$i... ";
	if rm "/$i"; then
		echo "Ok";
	else
		echo "(W) Impossibile cancellare i file!";
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
echo "Il programma è stato rimosso con successo.";
echo;

exit 0;
