# find
Descrizione
L'obiettivo del progetto è quello di sviluppare l'applicazione find che consente di individuare il numero di occorrenze di un insieme di stringhe all'interno di un gruppo di file. Per determinare il numero di occorrenze (e la loro posizione) si consiglia di utilizzare l'Algoritmo di Knuth-Morris e Pratt. Ad ogni esecuzione l'applicazione produrrà in output la lista dei file analizzati con le occorrenze della stringa nel testo insieme alle informazioni riguardante la posizioni della stesse. Le stesse informazioni prodotte in output potranno essere salvate su di un file esterno.

I sorgenti devono essere consegnati in un archivio .zip o .tgz. Gli script Makefile o CMake devono essere acclusi per supportare la compilazione.

Report
Per ogni parola ricercata, il report conterrà le seguenti informazioni:

WORD <word1>\r\n
TOTAL <total_occurrences_word1>\r\n
FILE <absoluthepath_file1>\r\n
OCCURRENCES <total_occurrences_file1>\r\n
<line1> <char1>\r\n
...
<linen> <charn>\r\n
...
FILE <absoluthepath_filek>\r\n
OCCURRENCES <total_occurrences_filek>\r\n
<line1> <char1>\r\n
...
<linem> <charm>\r\n 
Il file terminerà con una riga vuota (\r\n).

Le parole vengono elencate nel report in ordine alfabetico mentre per ogni parola i file vengono presentati in ordine decrescente per occorrenza. Nel caso di più file con il medesimo numero di occorrenze i file verranno presentati in ordine alfabetico (usando il path assoluto).

Se ad esempio consideriamo i file messi a disposizione al seguente link, collocati nella directory /home/loreti/Documents, il risultato della ricerca delle parole aria, una e genio, il risultato sarà:

WORD aria\r\n
TOTAL 2\r\n
FILE /home/loreti/Documents/marzo1821.txt\r\n
OCCURRENCES 2\r\n
69 2\r\n
71 2\r\n
FILE /home/loreti/Documents/5maggio.txt\r\n
OCCURRENCES 0\r\n
WORD genio\r\n
TOTAL 2\r\n
FILE /home/loreti/Documents/5maggio.txt\r\n
OCCURRENCES 1\r\n
16 13\r\n
FILE /home/loreti/Documents/marzo1821.txt\r\n
OCCURRENCES 0\r\n
WORD una\r\n
TOTAL 2\r\n
FILE /home/loreti/Documents/marzo1821.txt\r\n
OCCURRENCES 4\r\n
28 15\r\n
32 1\r\n
34 1\r\n
49 4\r\n
FILE /home/loreti/Documents/5maggio.txt\r\n
OCCURRENCES 1\r\n
10 14\r\n
\r\n
Funzionalità e Uso
L'applicazione find può essere usata sia per generare dei report delle analisi che per estrarre informazioni da report generati in esecuzioni precedenti.

Generazione di report
Per generare un nuovo report occorre indicare le parole da ricercare e le directory o file dove eseguire la ricerca. I file e le directory da analizzare verranno elencate in un file, da passare come input al programma, con la seguente struttura:

<path1> [r]\r\n
<path2> [r]\r\n
...
<pathk> [r]\r\n
\r\n
Ogni <pathi> potrà essere un path assoluto o relativo ad un file o directory. Nel secondo caso, il parametro r, opzionale, indicherà se occorra analizzare ricorsivamente le sottodirectory. Se non presente solo i file regolari nella directory verranno analizzati.

L'esecuzione del programma avverrà quindi con i seguenti parametri:

find --words|-w <inputfile> 
In questo caso il report verrà stampato a termine dell'esecuzione.

Per salvare il report su un file particolare occorrerà aggiungere il parametro

--output|-o <outputfile>
Durante la fase di analisi sarà possibile ignorare i file con specifiche estensioni, aggiungendo il parametro:

--exclude|-e <ext>
Infine, per visionare il processo di analisi si potrà aggiungere il parametro

--verbose|-v
In questo caso per ogni file (o directory) analizzata occorrerà stampare messaggi che indicano l'inizio dell'elaborazione, il termine ed il tempo necessario all'analisi. Un possibile schema potrebbe essere (i tempi riportati non sono indicativi):

Inizio elaborazione parola: aria
Inizio elaborazione directory: /home/loreti/Documents
Inizio elaborazione file: /home/loreti/Documents/marzo1821.txt
Fine elaborazione file: /home/loreti/Documents/marzo1821.txt (0.2 sec)
Inizio elaborazione file: /home/loreti/Documents/5maggio.txt
Fine elaborazione file: /home/loreti/Documents/5maggio.txt (0.1 sec)
Fine elaborazione parola: aria
Analisi dei report
Una volta generato il file di report, il programma find potrà essere usato per recuperare le informazioni salvate. Potremmo:

* Stampare la lista dei file dove occorre almeno <n> volte la parola <word>:

fine --report|-r <reportfile> --show <word> <n>
Se <n> viene omesso, si utilizza il valore 1.

* Stampare tutte le posizioni dove la parola <word> occorre nel file <file>:

fine --report|-r <reportfile> --show <word> --file <file>
Se <word> non occorre in <file>, viene stampato a video un messaggio opportuno.
