
# UW CLMS Application Work Sample
## Matt Fergoda
### Coding project submitted as a work sample for admission to UW CLMS in March 2020.


**Applicant**: Matt Fergoda  
**Course**: Harvard’s CS50: Introduction to Computer Science via EdX  
**Dates taken**: May - Sept. 2019  
[**Link to full assignment description**](https://docs.cs50.net/2019/x/psets/4/speller/hashtable/speller.html)  

### Usage
1. Make sure you open the program in an environment with a bash shell. Otherwise, the program will have trouble compiling during step 3. If you would like to execute the program and you have a PowerShell environment, the easiest workaround is to access the project in CS50’s proprietary IDE. To do so, please email me your GitHub username requesting access. Once I've granted you access, you can go to [this link](https://ide.cs50.io/mattfergoda/ide).

2. Navigate into the directory `UW_CLMS_Work_Sample`.

3. To have `clang` compile the program according to the `Makefile`’s specifications, run `make speller`.

4. The program’s usage is `./speller [dictionary] text`
    * There are two dictionaries available with which to run the program: one “small” (with just two words) and one “large” (with several thousand words), both of which can be found in `dictionaries/`.
    * If the argument is omitted, the program will default to the large dictionary.
    * There are a variety of texts with which to run the program, found in the `texts/`.
    * Answer keys demonstrating the correct program output using the large dictionary can be found in the `keys/`.
    * For example, the command  
        ```./speller dictionaries/small texts/carroll.txt```  
    will run the program on carroll.txt referencing the small dictionary while  
        ```./speller dictionaries/large texts/holmes.txt```  
    or simply  
        ```./speller texts/holmes.txt```  
    will run the program on holmes.txt referencing the large dictionary.
    * I suggest using
   ``` ./speller [dictionary] text > output.txt```
    to print the output to a `.txt` file instead of the terminal, making it easier to compare the output to the key and keeping the terminal free of output.

5. To compare the output of my program to an answer key one can run, for example:
   ```
      ./speller texts/holmes.txt > output.txt
       diff -y output.txt keys/holmes.txt > diff.txt
    ```
    The output file `diff.txt` will print the program’s output for `holmes.txt` and the corresponding key side by side with `<` characters indicating lines that differ. (Note that my program will have five lines of runtime information at the bottom of the output not included in the answer keys).
