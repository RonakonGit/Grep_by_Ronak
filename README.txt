Title of project : grep

Name : Ronak Prashant Patil

MIS ID: 141803010

Description of work done:
	1. I have created only single .c file which I felt is sufficient
	2. I have implemented  i w q n f o m b c v H h options .
	3. The main part is combinations where I  have created a printline function which first validates the line with respect to the options and if all the conditions are valid then the line is printed . For example, if -n -i -m 2 are given as input options then printline will first convert the pattern in case sensitive and if the pattern matches the it returns the line else it returns NULL then it check for -n options and -m and only line which is not NULL get printed that is after two pattern match -m will return NULL and hence other options are neglected.
	4.My most of functions return char* (line if true else NULL for False).Due to this I was able to reduce my code a lot when I went for printline function as I did not require more if else conditions.
