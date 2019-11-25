# find

The find application allows you to identify the number of occurrences of a set of strings within a group of files. To determine the number of occurrences (and their position) the Knuth-Morris and Pratt algorithm is used. At each execution, the application will output the list of files analyzed with the occurrences of the string in the text together with information regarding its position. The same information produced in output can be saved on an external file.
The find application can be used both to generate analysis reports and to extract information from reports generated in previous executions.

To generate a new report, indicate the words to be searched and the directories or files to be searched. The files and directories to be analyzed will be listed in a file, to be passed as input to the program, with the following structure:

<code> path1</code>
<code> path2</code>
      ....
      pathk

Ogni ''<pathi>'' potrà essere un path assoluto o relativo ad un file o directory. Nel secondo caso, il parametro ''r'', opzionale, indicherà se occorra analizzare ricorsivamente le sottodirectory. Se non presente solo i file regolari nella directory verranno analizzati. 

L'elenco delle parole da ricercare, invece, verrà passato al programma attraverso un file della forma:

<code>
word1\r\n
word2\r\n
...
</code>

L'esecuzione del programma avverrà quindi con i seguenti parametri:

<code>
find (--words|-w) <wordfile> (--input|-i) <inputfile> 
</code>

In questo caso il report verrà stampato a termine dell'esecuzione.

Per salvare il report su un file particolare occorrerà aggiungere il parametro

<code>
--output|-o <outputfile>
</code>

Durante la fase di analisi sarà possibile ignorare i file con specifiche estensioni, aggiungendo il parametro:

<code>
--exclude|-e <ext>
</code>

Infine, per visionare il processo di analisi si potrà aggiungere il parametro

<code>
--verbose|-v
</code>

In questo caso per ogni file (o directory) analizzata occorrerà stampare messaggi che indicano l'inizio dell'elaborazione, il termine ed il tempo necessario all'analisi. Un possibile schema potrebbe essere (i tempi riportati non sono indicativi):

<code>
Inizio elaborazione parola: aria
Inizio elaborazione directory: /home/loreti/Documents
Inizio elaborazione file: /home/loreti/Documents/marzo1821.txt
Fine elaborazione file: /home/loreti/Documents/marzo1821.txt (0.2 sec)
Inizio elaborazione file: /home/loreti/Documents/5maggio.txt
Fine elaborazione file: /home/loreti/Documents/5maggio.txt (0.1 sec)
Fine elaborazione parola: aria
</code>  

== Analisi dei report ==

Una volta generato il file di report, il programma ''find'' potrà essere usato per recuperare le informazioni salvate. Potremmo:


*  Stampare la lista dei file dove occorre almeno ''<n>'' volte la parola ''<word>'':

<code>
fine --report|-r <reportfile> --show <word> <n>
</code>

Se ''<n>'' viene omesso, si utilizza il valore ''1''.


*  Stampare tutte le posizioni dove la parola ''<word>'' occorre nel file ''<file>'':

<code>
fine --report|-r <reportfile> --show <word> --file <file>
</code>

Se ''<word>'' non occorre in ''<file>'', viene stampato a video un messaggio opportuno. 
