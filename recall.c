#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>

char line[400];						//Declaring global variables
char answer[200];
char question[200];
FILE *fp;
char file_path[800];
char home_path[400];


int print_title(void) {
	printw("  |======================================|\n");
	printw("  |  |^^^\\    _      _       _    || ||  |\n");
	printw("  |  |[_]/  /^^^\\  //^\\\\  //^\\\\   || ||  |\n");
	printw("  |  || \\\\ ||===\' ||(    ||( )\\\\  || ||  |\n");
	printw("  |  || ||  \\\\=//  \\\\=//  \\\\=//\\\\ || ||  |\n");
	printw("  |             a memory training tool   |\n");
	printw("  |======================================|\n\n\n");
}

int get_line(int question_number){	//Read random line from file

	int i = 0;

	fp = fopen(file_path, "r");			//Open the file in read mode
	while (i <= question_number){
		fgets(line, 400, fp);			//Load first line of file into array "line"
		i++;
	}
	fclose(fp);					//Close file
}


int get_random_line_number() {				//Get random line number within the bounds of the file
	int number_of_lines_in_file = 1;
	char c;

	fp = fopen(file_path, "r");			//Open the file in read mode

	for (c = getc(fp); c != EOF; c = getc(fp)){	//Count number of line in file
		if (c == '\n'){
			number_of_lines_in_file++;
		}
	}

	fclose(fp);					//Close file
	return rand() % number_of_lines_in_file;	//Return number of lines in file
}


int get_question(){			//Get question and answer and store them in global variables

	char delim[] = "`";
	short answer_length = 0;
	short line_length = 0;
	short question_length = 0;

	while(true){
		get_line(get_random_line_number());		//Store random line from file in "line"
		if(strstr(line, "`") != NULL){			//Make sure line if properly formatted. If not, get a different line.
			break;
		}
	}

	char *start_of_answer = strchr(line, '`');	//Fine where '`' is that makes the beginning of the answer
	strcpy(answer, &start_of_answer[1]);		//Copy answer section on line to answer

	answer_length = strlen(answer);			//Find length of answer
	line_length = strlen(line);			//Fine length of whole line in file
	question_length = line_length - answer_length;	//Fine length of question
	strncpy(question, line + 0, question_length - 1);//Copy question section of line to question
	question[question_length - 1] = '\0';		//Add terminator

	return 0;
}


int ask_question(void) {				//Print question and answer to screen
	erase();					//Erase contents of screen
	printw("  Press shift-X to exit --- Press shift-F to open a file --- Press shift-H for help\n\n");
	print_title();
	printw("%s", question);				//Print line
	return 0;
}


int show_answer(void){
	erase();					//Erase contents of screen
	printw("  Press shift-X to exit --- Press shift-F to open a file --- Press shift-H for help\n\n");
	print_title();
	printw("%s\n\n%s\n", question, answer);		//Print line and answer
	return 0;
}


int get_home_path(void){
	strcpy(home_path, getenv("HOME"));		//Copy path to home directory to home_path
	strcat(home_path, "/Documents/RecallLists/");	//Cat path to RecallList folder from home directory
	return 0;
}


int open_file(void){
	char file_name[400];
	erase();					//Erase contents of screen
	print_title();
	printw("Type the name of the file you would like to open.\n\n:");
	scanw("%s", file_name);				//Store file name
	file_path[0] = '\0';				//Clear file_path for new file path
	strcat(file_path, home_path);
	strcat(file_path, file_name);
	strcat(file_path, ".txt");

	while(access(file_path, F_OK | W_OK) == -1){	//Keep showing an error message is the file does not exist
		erase();
		print_title();
		printw("\"%s\" cannot be found. Please type a different file name.\n\n:", file_path);

		scanw("%s", file_name);			//Store file name
		file_path[0] = '\0';			//Clear file_path for new file path
		strcat(file_path, home_path);		//Add path to RecallLists folder to file_path
		strcat(file_path, file_name);		//Add file name to file path
		strcat(file_path, ".txt");
	}
}


int help_message(void){
	erase();
	print_title();
	printw("Press any key to exit this help page.\n\n");
	printw("* * * WHAT IS RECALL * * *\n");
	printw("Recall is a free program designed to help with memorisation though \"virtual flashcards.\" It runs entirely within the Linux shell.\n\n");
	printw("* * * HOW TO OPEN A FILE * * *\n");
	printw("The flashcards are stored in text files. When the programs is first opened, it will promote you to enter the name of a file. If you type the name of a file in Document/RecallLists (minus the .txt extension), the file will open.\n When a file is already open, shift-f will allow you to open a different file.\n\n");
	printw("* * * HOW TO USE A FLASHCARD FILE * * *\n");
	printw("Once a flashcard file is open, a random question will be displayed. When you press any key, the answer will be displayed. Then, you can press a any key to display the next question.\n\n");
	printw("* * * CREATING NEW FLASHCARD FILES * * *\n");
	printw("Create a new .txt file in the Documents/RecallLists directory. The name of this file (minus the .txt extension) is the name the file can be accessed with from withing Recall. You can add questions within this file. Each question/answer pair should be put on its own line, and no lines should be blank. The question and answer should be separated by a \'`\' An example is shown below.\n");
	printw("This is a question!`Here is an answer");
	getch();
}


int main(void){
	char c;						//Variable that will be used to read keystrokes

	get_home_path();				//Store environment variable $HOME in home_path

	initscr();
	open_file();					//Get a file name from the user and store it

	while(true){
		get_question();				//Read random line from file, extract question and answer, and store them in variables "question" and "answer".
		ask_question();				//Show question.
		c = getch();				//Wait for keystroke

		if(c == 'X'){				//If keystroke was an 'X', close program.
			endwin();
			return 0;
		}if(c == 'F'){				//If keystroke was an 'F', allow user to open new file.
			open_file();
			continue;
		}if(c == 'H'){				//if keystroke was an 'H', show help screen.
			help_message();
		}

		show_answer();				//She answer along with question.

		c = getch();

		if(c == 'X'){				//If keystroke was an 'X', close program.
			endwin();
			return 0;
		}if(c == 'F'){				//If keystroke was an 'F', allow user to open new file.
			open_file();
			continue;
		}if(c == 'H'){				//if keystroke was an 'H', show help screen.
			help_message();
		}
	}
}
