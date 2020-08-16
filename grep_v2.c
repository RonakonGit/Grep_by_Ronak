#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>
#include <error.h>
/*colors*/
#define G_RED     "\x1b[31m"
#define G_GREEN   "\x1b[32m"
#define G_YELLOW  "\x1b[33m"
#define G_BLUE    "\x1b[34m"
#define G_MAGENTA "\x1b[35m"
#define G_CYAN    "\x1b[36m"
#define G_RESET   "\x1b[0m"

typedef struct options {
	int i_flag;
	int o_flag;
	int n_flag;
	int normal_flag;
	int v_flag;
	int b_flag;
	int f_flag;
	int w_flag;
	int c_flag;
	int q_flag;
	int h_flag;
	int H_flag;
	int m_flag;
	int r_flag;
} options;
/*globals*/
static int b_count = 0;
static int m_remember = 0;
static int count = 1 ;
static int n_count = 0;
static int o_count = 0;
static int c_count = 0;
static options var;
static options outputvar;
/*function declaration*/
char *lower(char *str);
void setvariables(options var);
void printline(char *line, char *pattern, options var, options outputvar , char *argv[], int argc, int m_arg);
char *grep_o(char *oneline, char *pattern);
char *grep_n(char *oneline, char *pattern);
char *grep_f(char *oneline, char *filename);
char *grep_i(char *oneline, char *pattern);
char *grep_v(char *oneline, char *pattern);
char *grep_w(char *oneline, char *pattern);
char *grep_default(char *oneline, char *pattern);
char *grep_b(char *oneline, char *pattern);
char *grep_m(char *oneline, int itration, char *pattern);
/*funtion declaration*/
int main (int argc, char *argv[]) {
	int c ;
	int m_arg;
	int respace = 100000;
	char *pattern;
	char *line ;
	FILE *fmain;
	line = (char *)malloc(respace);
	void setvariables(options var);
	void setvariables(options outputvar);
	if(argc < 3 ) {
			printf(G_GREEN" invalid arrgument \n Usage: grep [OPTION]... PATTERN [FILE]...\n" G_RESET);
	}
	while ((c = getopt (argc, argv, "wcvqhHiobm:rfn")) != -1) {
		switch (c)
		{
			case 'c':
				var.c_flag = 1;
				break;
			case 'v':
				var.v_flag = 1;
				break;
			case 'w':
				var.w_flag = 1;
				break;
			case 'o':
				var.o_flag = 1;
				break;		
			case 'q':
				var.q_flag = 1;
				break;
			case 'h':
				var.h_flag = 1;
				break;
			case 'i':
				var.i_flag = 1;
				break;
			case 'H':
				var.H_flag = 1;
				break;
			case 'b':
				var.b_flag = 1;
				break;
			case 'm':
				var.m_flag = 1;
				m_arg = atoi(optarg);
				break;
			case 'r':
				var.r_flag = 1;
				break;
			case 'f':
				var.f_flag = 1;
				break;	
			case 'n':
				var.n_flag = 1;
				break;
			default:
				if (var.q_flag != 1) {
					printf("Usage: grep [OPTION]... PATTERN [FILE]...\n");
				}
			}
		}
	count = optind;
	pattern = argv[count++];
	while(count != argc){
		fmain = fopen(argv[count], "r");
		if(fmain == NULL ) {
			if(var.q_flag != 1) {
				printf(G_RED"grep:"G_CYAN" %s "G_GREEN": No such file or directory\n" G_RESET, argv[count]);
			}
			goto nextfile;
		}
		while(fgets(line, respace, fmain)) { 
			printline(line, pattern, var, outputvar, argv, argc, m_arg); 
			free(line);
			line = (char *)malloc(respace);
		} 
		if(var.c_flag == 1 && var.q_flag != 1) {
			if(var.H_flag == 1 ) {
				printf(G_MAGENTA"%s:"G_RESET, argv[count]);
			}
			printf("%d\n", c_count);
			c_count = 0;
		}
		fclose(fmain);
		nextfile:
		count++;
		b_count = 0;
		m_remember = 0;
		n_count = 0;
	}
	return 0;
}
/*function*/
void setvariables(options var) {
	var.i_flag = 0;
	var.o_flag = 0;
	var.n_flag = 0;
	var.normal_flag = 0;
	var.v_flag = 0;
	var.b_flag = 0;
	var.f_flag = 0;
	var.w_flag = 0;
	var.c_flag = 0;
	var.q_flag = 0;
	var.h_flag = 0;
	var.H_flag = 0;
	var.m_flag = 0;
	var.r_flag = 0;
}
/*function to printline*/
void printline(char *line, char *pattern, options var, options outputvar , char *argv[], int argc, int m_arg) {
	int llen = strlen(line);
	char backup_l[llen];
	strcpy(backup_l, line);
	char *n_line = (char *)malloc(llen); 
	char *b_line = (char *)malloc(llen);
	char *v_line = (char *)malloc(llen);
	char *f_line = (char *)malloc(llen);
	char *m_line = (char *)malloc(llen);
	char *w_line = (char *)malloc(llen);
	char *o_line = (char *)malloc(llen);
	char *normal_line = (char *)malloc(llen);
	if(var.v_flag == 1 && var.o_flag == 1 ) {
		return;
	}
/*grep default handling*/
	if(var.i_flag != 1 && var.v_flag != 1) {
		normal_line = grep_default(line, pattern);
		if(normal_line != NULL) {
		 }
	}
/*grep_i and grep_v handling*/
	if(var.i_flag == 1 &&  var.v_flag != 1 && var.o_flag != 1 && var.f_flag != 1 && var.n_flag != 1 && var.b_flag != 1 && var.w_flag != 1) {
		char *i_line = (char *)malloc(llen);
		i_line = grep_i(line, pattern);
		if(i_line != NULL && var.q_flag != 1) {
			if(var.H_flag == 1) { 
				printf(G_MAGENTA "%s" G_CYAN ":"G_RESET, argv[count]);
			}
			printf("%s", i_line); 
		}
		return;
	}
/*v_flag*/
	if(var.v_flag == 1) {
		v_line = grep_v(line, pattern);
	}

/*grep_b*/
	if(var.b_flag == 1 &&  var.v_flag != 1 && var.f_flag != 1 && var.n_flag != 1  && var.w_flag != 1 && var.o_flag != 1) {
		b_line = grep_b(line, pattern);
		if(b_line != NULL && var.q_flag != 1) {
			printf(G_YELLOW"%d :"G_RESET" %s", (b_count - llen ), b_line); 
		 }
		 return;
	} 
	if(var.b_flag == 1 ) {
		b_line = grep_b(line, pattern);
	} 
/*w_flag*/
	if(var.w_flag == 1) {
		w_line = grep_w(line, pattern);
	}
/*m_flag*/
	if(var.m_flag == 1 &&  var.v_flag != 1 && var.f_flag != 1 && var.n_flag != 1 ){
		if((m_line = grep_m(line, m_arg, pattern)) != NULL) 
		{
		}
		if(m_line != NULL && var.q_flag != 1) {
			if(var.H_flag == 1) { 
				printf(G_MAGENTA " %s" G_CYAN ":"G_RESET, argv[count]);
			}
			printf(" %s",	m_line);
		}
		return;
	}
	
	if(var.m_flag == 1 ) {
		if((m_line = grep_m(line, m_arg, pattern)) != NULL) 
		{
		}
	}
/*f_flag_alone*/ 
	if(var.f_flag == 1  && var.v_flag != 1 && var.o_flag != 1  && var.n_flag != 1 && var.i_flag != 1  && var.b_flag != 1 && var.w_flag != 1) {
		if((f_line = grep_f(line, pattern)) != NULL) {
		}
		if(f_line != NULL) {
			if(var.H_flag == 1 && var.c_flag != 1) { 
				printf(G_MAGENTA " %s" G_CYAN ":"G_RESET, argv[count]);
			}
			if(var.c_flag != 1) {
				printf("%s",f_line);
			}
			if(var.c_flag == 1) {
				c_count++;
			}
		}
		return;
	}
/*f_flag*/
	if(var.f_flag == 1) {
		if((f_line = grep_f(line, pattern)) != NULL) {
		}
	}	
/*nflag*/
	if(var.n_flag == 1) {
		if(line != NULL) {
			n_line = grep_n(line, pattern);
		}
	}
/*o_flag*/ 
	if(var.o_flag == 1) {
		if((o_line = grep_o(line, pattern)) != NULL) {
			o_count = 0;
		}
	}

		if(var.H_flag == 1 && var.c_flag != 1 && normal_line != NULL && v_line != NULL && n_line != NULL && f_line != NULL && m_line != NULL && w_line != NULL && o_line != NULL && var.q_flag != 1) {
			printf(G_MAGENTA"%s"G_CYAN":"G_RESET, argv[count]);
		}
		if(var.n_flag == 1 && var.c_flag != 1 && normal_line != NULL  && n_line != NULL && m_line != NULL && f_line != NULL && o_line != NULL  && w_line != NULL && v_line != NULL && var.q_flag != 1) {
			printf(G_GREEN"%d" G_CYAN":"G_RESET, n_count);
		}
		if(var.b_flag == 1 && var.c_flag != 1 && normal_line != NULL  && n_line != NULL && f_line != NULL && m_line != NULL && w_line != NULL && v_line != NULL && o_line != NULL && var.q_flag != 1) {
			printf(G_GREEN"%d:"G_RESET, b_count - llen);
		}
		if(n_line != NULL  && normal_line != NULL && f_line != NULL && m_line != NULL && w_line != NULL && o_line != NULL && v_line != NULL ) {
			if(var.o_flag == 1 && var.q_flag != 1) {
				printf("%s\n", o_line);
				return;
			}
			if(var.v_flag == 1 && var.q_flag != 1) {
				if(v_line != NULL) {
					c_count ++;
					if(var.c_flag != 1) {
						printf("%s", v_line);
					}
				}
			}
			else {
				c_count ++;
				if(var.c_flag != 1 && var.q_flag != 1) {
					printf("%s", line);
				}
			}
		}
}
/*grep default
* prints matching lines 
*/
char *grep_default(char *oneline, char *pattern) { /*returns 0 if pattern found in line else return -1*/
	int llen = strlen(oneline);
	int plen = strlen(pattern);
	char *newoneline = (char *)malloc(llen + 1);	/*copy oneline in new string say newoneline*/
	char *newpattern = (char *)malloc(plen + 1);	/*convert copy pattern to new string say newpattern*/
	strcpy(newoneline, oneline);
	strcpy(newpattern, pattern);
	if(var.i_flag == 1) {
		strcpy(newoneline, lower(newoneline));
		strcpy(newpattern, lower(newpattern));
	}
	if(oneline == NULL) {
		return NULL;
	}
	if(strstr(newoneline, newpattern) == NULL ) {	/*search for newpattern in newoneline using strtr*/ 
		return NULL;
		
	}
	else {
		return oneline;
	}
}
/*grep_i
* case sensetive
* eg : pattern "ronak"
* it will match with all cases eg : RONAK , Ronak, rOnaK ,ronak etc;
*/
char *grep_i(char *oneline, char *pattern) { 
	int llen = strlen(oneline);
	int plen = strlen(pattern);
	int i;
	char *newoneline = (char *)malloc(llen + 1);
	char *newpattern = (char *)malloc(plen + 1);
	strcpy(newoneline, oneline);
	strcpy(newpattern, pattern);
	if(oneline == NULL) {
		return NULL;
	}
	for (i = 0; newpattern[i]!='\0'; i++) {
		if(newpattern[i] >= 'A' && newpattern[i] <= 'Z') { 
			newpattern[i] = newpattern[i] + 32;
		}
	}
	for (i = 0; newoneline[i]!='\0'; i++) {
		if(newoneline[i] >= 'A' && newoneline[i] <= 'Z') { 
			newoneline[i] = newoneline[i] + 32;
		}
	}
	if(strstr(newoneline, newpattern) == NULL ) {
		return NULL;
	}
	else {
		return oneline;
	}
}

/*grep_v
* print lines which do not match
*/
char *grep_v(char *oneline, char *pattern) {
	if(oneline == NULL) {
		return NULL;
	}
	if(grep_default(oneline, pattern) != NULL ) { 
		return NULL;
	}
	else {
		return oneline;
	}
}
/*grep_w
* Select  only  those  lines  containing  matches  that form whole words.  The test is that the matching substring must
	either be at the beginning of the line, or preceded by a non-word  constituent  character.   Similarly,  it  must  be
	either  at  the  end  of  the  line or followed by a non-word constituent character.  Word-constituent characters are
	letters, digits, and the underscore.
*/
char *grep_w(char *oneline, char *pattern) {
	if(oneline == NULL) {
		return NULL;
	}
	int llen = strlen(oneline);
	int plen = strlen(pattern);
	int set = -1;
	int i = 0, j = 0, k = 0;
	char *newoneline = (char *)malloc(llen + 1);
	char *newpattern = (char *)malloc(plen + 1);
	char *temp = (char *)malloc(llen + 1);
	strcpy(newoneline, oneline);
	strcat(temp, " ");
	strcat(temp, newoneline);
	strcat(temp, " ");
	strcpy(newpattern, pattern);
	if(var.i_flag == 1) {
		strcpy(newoneline, lower(newoneline));
		strcpy(newpattern, lower(newpattern));
		strcpy(temp, lower(temp));
	}
	if(strstr(newoneline, newpattern) == NULL) {
		return NULL;
	}
	for(i = 0; temp[i] != '\0'; i++) {
			if(temp[i] == newpattern[0] && temp[i - 1] != '_'&& isalpha(temp[i - 1]) == 0 &&  isdigit(temp[i - 1]) == 0 &&  isdigit(temp[i + plen]) == 0 && isalpha(temp[i + plen]) == 0 && temp[i + plen] != '_') {
				for(j = i, k = 0; j < i + plen; j++, k++ ) {
					if(temp[j] == pattern[k]) {
						 set = 1;
					}	
					else {
						set = 0;
					}
				}
			}
	}
	if(set == 1) {
		return oneline;
	}
	else {
		return NULL;
	}
	
}
/*grep_f : 
*opens a file to read pattern 
*match pattern from pattern file to other files also works on multiple files
*print line only ones if pattern is found
*eg : if pattern file contains "ronak " , "coep" and serch file conations line "ronak dislikes coep canteen food " 
	output will display this line only ones not twice;
*/
char *grep_f(char *oneline, char *filename) {
	FILE *fp;
	fp = fopen(filename, "r"); 
	int llen = strlen(oneline);
	int set = 0;	
	char *token;
	char newoneline[llen];
	char str2[llen];
	strcpy(newoneline, oneline);
	if(fp == NULL && var.q_flag != 1) {
		printf(G_CYAN" %s "G_GREEN": No such file or directory\n" G_RESET, filename);
		goto rewind;
	}
	while(fgets(str2, llen, fp)) {
		if(strlen(str2) == 0) {
			return NULL;
		}
		token = strtok(str2, "\n");
		while(token != NULL) {
			if(var.i_flag == 1) {
				strcpy(newoneline, lower(newoneline));
				strcpy(token, lower(token));
			}
			if(strstr(newoneline, token) != NULL) {
				set = set + 1;
			}		
			token = strtok(NULL, "\n");	
		}
	}
	if(set > 0) {
		rewind(fp);
		return oneline;
	}
	
	rewind : 
	rewind(fp);
	return NULL;
}
/*grep_m
*match til given number times
*eg : -m 2 means it will print first 2 matching lines
*/
char *grep_m(char *oneline, int itration, char *pattern) {
	char *set;
	int llen = strlen(oneline);	
	int plen = strlen(pattern);
	char *newoneline = (char *)malloc(llen + 1);	/*copy oneline in new string say newoneline*/
	char *newpattern = (char *)malloc(plen + 1);	/*convert copy pattern to new string say newpattern*/
	strcpy(newoneline, oneline);
	strcpy(newpattern, pattern);
	if(var.i_flag == 1) {
		strcpy(newoneline, lower(newoneline));
		strcpy(newpattern, lower(newpattern));
	}
	if(oneline == NULL) {
		return NULL;
	}
	if(itration == m_remember) {
		return NULL;	
	} 
	set = grep_default(newoneline, newpattern);
	if(set != NULL) {
		m_remember = m_remember + 1;
		return oneline;
	}
	return NULL;
}
/*grep_o
*print matched pattern only
*/
char *grep_o(char *oneline, char *pattern) {
	int llen = strlen(oneline);
	int plen = strlen(pattern);
	char newoneline[llen];	/*copy oneline in new string say newoneline*/
	char newpattern[plen];	/*convert copy pattern to new string say newpattern*/
	char *token;
	static int o_send = 0;
	strcpy(newoneline, oneline);
	strcpy(newpattern, pattern);
	if(oneline == NULL) {
		return NULL;
	}
	if(var.i_flag == 1) {
		strcpy(newoneline, lower(newoneline));
		strcpy(newpattern, lower(newpattern));
	}
	o_send = 0;
	token = strtok(newoneline, " \n\0");
	while(token != NULL) {
		if(strstr(token, newpattern) != NULL) {
				o_count = o_count + 1;
				o_send = 1;
		}
		token = strtok(NULL, " \n\0");	
	}
	if(o_send == 1) {
		return  pattern;
	}
	else {
		return NULL;
	} 
}
/*grep_n
*print line number before lines
*/
char *grep_n(char *oneline, char *pattern) { 
	int llen = strlen(oneline);
	int plen = strlen(pattern);
	char *newoneline = (char *)malloc(llen + 1);	/*copy oneline in new string say newoneline*/
	char *newpattern = (char *)malloc(plen + 1);	/*convert copy pattern to new string say newpattern*/
	strcpy(newoneline, oneline);
	strcpy(newpattern, pattern);
	if(var.i_flag == 1) {
		strcpy(newoneline, lower(newoneline));
		strcpy(newpattern, lower(newpattern));
	}
	if(strstr(newoneline, newpattern) == NULL ) {	/*serch for newpattern in newoneline using strtr*/ 
		n_count = n_count + 1;
		free(newoneline);
		free(newpattern);
		if(var.v_flag == 1) {
			return oneline;
		}
		return NULL;
	}
	else {
		n_count = n_count + 1;
		free(newoneline);
		free(newpattern);
		if( var.v_flag == 1) {
			return NULL;
		}
		return oneline;
	}
}
/*grep_b
* frint first byte number of line
*/
char *grep_b(char *oneline, char *pattern) {
	int llen = strlen(oneline);
	int plen = strlen(pattern);
	int y;
	char *newoneline = (char *)malloc(llen + 1);	/*copy oneline in new string say newoneline*/
	char *newpattern = (char *)malloc(plen + 1);	/*convert copy pattern to new string say newpattern*/
	strcpy(newoneline, oneline);
	strcpy(newpattern, pattern);
	y = strlen(oneline);
		if(var.i_flag == 1) {
		strcpy(newoneline, lower(newoneline));
		strcpy(newpattern, lower(newpattern));
	}
	if(oneline == NULL) {
		free(newoneline);
		free(newpattern);
		return NULL;
	}
	if(strstr(newoneline, newpattern) == NULL ) {
		b_count = b_count + y;
		free(newoneline);
		free(newpattern);
		return NULL;	
	}
	else {
		b_count = b_count + y;
		free(newoneline);
		free(newpattern);
		return oneline;
	}
}
/*lowercase*/
char *lower(char *str) {
	int i;
	for(i = 0; str[i]; i++) {
		str[i] = tolower(str[i]);
	}
	return str;
}
