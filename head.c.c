/***************************************************************
 * -> Use ./head -h for usage instructions after compiling
 * ->The options (-e, -V, -h ,-n --numlines) are checked for
 * ->The options can be used on their own or with eachother
 * ->If no file is input the user must input data from stdin
 * ->This data is stored in a temporary file which is deleted 
 * after at the end of the program, I tried use a very specific
 * filename so that no file is overwritten by the program.
 * -> If a file is input it is opened for reading
 * -> The first 10 lines from the file are printed if no options 
 * are used
 * -> If options are used the amount of lines printed may vary
 * -> After the program prints the output the program terminates
 ***************************************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	
	int opt, numflag = 0, num = 10;
	int version = 0, even = 0;
	
	while((opt = getopt(argc, argv, "n:Veh")) != -1) 	//loop checks for the vaarious options
	{
		switch(opt)
		{
			case 'n': numflag = 1;
					  num = atoi(optarg);
					  break;
			case 'V': version = 1;
					  break;
			case 'e': even = 1;
					  break;
			case 'h': fprintf(stdout,"****************************************************************\n                           HEAD        \n****************************************************************\n");
					  fprintf(stdout,"Usage: %s [-n numlines] [-V] [-e] [-h] [file]\n", argv[0]);
					  fprintf(stdout,"\n                          OPTIONS        \n*****************************************************************\n");
					  fprintf(stdout,"-> -n --line : Changes the number of lines printed\n at the start of the file to the integer following -n (--line)\n");
					  fprintf(stdout,"-> -V : Prints my name, email and student number\n");
					  fprintf(stdout,"-> -e : Prints only even lines\n");
					  fprintf(stdout,"-> -h : Prints the help message on how to use head\n");
					  fprintf(stdout,"[file] : if no file is input the system reads from standard input\n");
					  exit(EXIT_SUCCESS);
					  
					  break;
			default : fprintf(stderr,"Usage: %s [-n numlines] [-V] [-e] [-h] [file]\n", argv[0]);
					  fprintf(stderr,"Type head -h for more information on how to use the head function");
					  exit(EXIT_FAILURE);
		}
	}

	FILE *fp;
	char  *line = NULL ;
	size_t len = 0;
	size_t read;
	int count = 0;
	int end = 0;

	if(argv[optind] == NULL) //if there is no file input after the options takes input from user
	{
		fprintf(stdout, "Please input text, type 'EOF' to stop the text input\n");
		if((fp = fopen("tempfileforinputhead.txt", "w+")) == NULL) //if the file cannot be opened, exits program
		{
			puts("ERROR File could not be opened"); //alerts user to the error
			return -1;
		}

		while(end == 0 && (read = getline(&line, &len, stdin)) != -1) //reads user input until a line containing the substring "EOF" IS entered
		{
			if(strstr(line,"EOF") != NULL)	//if the line contains the substring "EOF"
			{
				end = 1;
			}
			else
			{
				fprintf(fp, "%s", line);	//prints the line to the temporary file
			}	
		}
		fprintf(stdout, "\n---------------------------------------------\n\n");
		rewind(fp);		//resets the file pointer for the reading of the file
	}
	else	//opens file input
	{
		if((fp = fopen(argv[optind], "r")) == NULL) //if the file cannot be opened, exits file
		{
			puts("ERROR File could not be opened"); //alerts user to the error
			return -1;
		}
	}

	
	if(even == 1)	//if the -e option was input 
	{
		num *= 2;	// multiply the amount of lines to print by 2 so that the same number of lines are printed even though only every second line is printed
	}
	
	if(version == 1)//if the -V option was input prints my information
	{
		fprintf(stdout, "Sean Mcdonnell\nsean.mcdonnell1@ucdconnect.ie\n18391961\n\n");
	}
	
	while((read = getline(&line, &len, fp)) != -1 && count < num)
	{
		if(even == 1) //if even flag was input only prints even lines
		{
			if(count % 2 == 1)	//if its an even line
			{
				fprintf(stdout, "%s", line);
			}
		}
		else	//if the ven flag was not input print every line
		{
			fprintf(stdout, "%s", line);
		}
		
		count++;
	}

	fclose(fp);

	if(argv[optind] == NULL)	//if there was no file input
	{
		remove("tempfileforinputhead.txt"); //deletes the temporary file that was used to store the stdin input
	}

	return 0;
}

