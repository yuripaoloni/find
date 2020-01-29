# find

The find application allows you to identify the number of occurrences of a set of strings within a group of files. To determine the number of occurrences (and their position) the Knuth-Morris and Pratt algorithm is used. At each execution, the application will output the list of files analyzed with the occurrences of the string in the text together with information regarding its position. The same information produced in output can be saved on an external file.
The find application can be used both to generate analysis reports and to extract information from reports generated in previous executions.

## Build

Run the `make` command in the main folder where the `makefile` is provided.

## Program report

For each word the report will contain the following information:

```
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
```

## Use and features

To generate a new report, indicate the words to be searched and the directories or files to be searched. The files and directories to be analyzed will be listed in a file, to be passed as input to the program, with the following structure:

```
<path1> [r]\r\n
<path2> [r]\r\n
...
<pathk> [r]\r\n
\r\n
```
Each ```<pathi>``` can be an absolute path or relative to a file or directory. In the second case, the optional parameter *r* will indicate if it is necessary to recursively analyze the subdirectories. If not present only the regular files in the directory will be analyzed.

Instead, the list of words to search for will be passed to the program through a form file:

```
word1\r\n
word2\r\n
...
```

The program will then be executed with the following parameters:

```
find (--words|-w) <wordfile> (--input|-i) <inputfile> 
```

In this case the report will be printed at the end of the execution.

To save the report on a particular file, add the parameter:

```
--output|-o <outputfile>
```

During the analysis phase it will be possible to ignore the files with specific extensions, adding the parameter:

```
--exclude|-e <ext>
```

Finally, to view the analysis process you can add the parameter:

```
--verbose|-v
```

In this case, for each file (or directory) analyzed, it will be necessary to print messages indicating the beginning of the processing, the term and the time required for the analysis.

### Report analysis

Once the report file is generated, the *find* program can be used to retrieve the saved information. We could:


Print the list of files where at least ```<n>``` times the word ```<word>``` occurs:

```
find --report|-r <reportfile> --show <word> <n>
```

If ```<n>``` is omitted, the value *1* is used.

Print all the positions where the word ```<word>``` occurs in the file ```<file>```:

```
find --report|-r <reportfile> --show <word> --file <file>

```

If ```<word>``` is not needed in ```<file>```, a suitable message is printed on the screen.

Anyway, the list of command is viewed with:

```
find --help
```
or simply:

```
find
```

## Test

All the tests are located into the ```test.c``` and ```test.h``` files. The tests used the text file provided by the ```test``` folder.
To launch the test :

```
find --test
```

## Author

* **Yuri Paoloni** - [yuripaoloni](https://github.com/yuripaoloni)

## Version 

1.0

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.

