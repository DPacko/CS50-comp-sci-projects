# Questions

## What is pneumonoultramicroscopicsilicovolcanoconiosis?
It's a lung disease caused by inhaling very fine ash and sand dust.

## According to its man page, what does `getrusage` do?
It returns resource usage measures for an argument (ex: int who).
We will use RUSAGE_SELF in speller.c. This returns resource usage statistics for the calling process,
which is the sum of resources used by all threads in the process.

## Per that same man page, how many members are in a variable of type `struct rusage`?
16

## Why do you think we pass `before` and `after` by reference (instead of by value) to `calculate`, even though we're not changing their contents?
Saves code size, memory usage and cpu usage. It saves time whereas, passing in values would mean time spent copying.

## Explain as precisely as possible, in a paragraph or more, how `main` goes about reading words from a file.
## In other words, convince us that you indeed understand how that function's `for` loop works.
In general:
'main' reads words from a file as follows: You need a pre-existing file to work with. So, with the file function - fopen,
you open that pre-existing file in 'r' or 'w' mode (checking for NULL) with a file pointer. Once it's opened, you open a
blank file, this also has a file pointer. This blank file will store the pre-existing files data.
This is all implemented through the two files' file pointers. Now, with the two files, we can start with a file function
(fgetc,fread, fwrite). In its paramaters are the pre-existing file and the blank file. Along with
these two files, we put in the file's unit's size and quanity. Here, main reads the words from the file.

Speller.c specific:
As stated above, we open the text file (with misspellings). When starting a for loop, we create an fgetc
function with a integer variable 'c' and have the pre-existing file pointer as the parameter. 'c' returns
the text file data into its own. Reading each one, it continues this task until the end of the file. Inside
the for loop, there is an if/else statement. Depending on if the characters in each word in the text file
are alphabetical, apostrophe, numeric, or too many characters in a word it will run a specific program. To
keep track, we declare, before the for loop, an integer counter of each word, character index of the word,
words and misspellings. Keeping those in mind during the reading of the words.

##        The key aspect of the for loop is the c = fgetc(fp) function call. fgetc
##        gets the next character (an unsigned char) from fp and advances the
##        position indicator in fp until the end of the file (EOF) is reached. As
##        each character is received, it is put through an if(if)-elseif-elseif
##        logical sequence. Summarizing the sequence: 1) If the character is an
##        alphabetical character or an apostrophe that is not at index 0 of the
##        word, the character is appended to the word array. 2) Else if the
##        character is numerical, the word is ignored and we skip ahead to the
##        next word (which is found after a space). 3) Else if we hit a space or
##        punctuation, we must have found a whole word so we terminate the word by
##        adding \0 to the word array.


## Why do you think we used `fgetc` to read each word's characters one at a time rather than use `fscanf` with a
##format string like `"%s"` to read whole words at a time? Put another way, what problems might arise by relying on `fscanf` alone?
fscanf and fgetc both read a file. The difference is that fscanf does not store the data and only reads into whitespace (making
it part of the word.) fgetc stores the data and reads to a newline.
Also, if a longer string than expected was read using fscanf (some random word like jkafkdfadfadfjfe), we could  overwrite important
data in memory or cause a segmentation fault.

## Why do you think we declared the parameters for `check` and `load` as `const` (which means "constant")?
The paramaters for these two are declared as const pointers because we want to prevent changes to a read
word and the dictionary we are using. Constant pointers prevent changing the data pointed to.


