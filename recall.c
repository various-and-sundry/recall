#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>

char line[4000];						//Declaring global variables
char answer[3000];
char question[3000];
FILE *fp;
char file_path[800];
char list_dir_path[400];
char home_dir_path[400];
char recallrc_path[400];


int jump(void) {
	int screenHight;
        int screenWidth;

	getmaxyx(stdscr, screenHight, screenWidth);

	for(int i=0; i < ((screenWidth-55)/4); ++i) {
		printw(" ");
	}
}


int print_head(void){
	jump();
	attron(A_REVERSE);
	jump();

	printw("j = next  ||  x = exit  ||  f = open file  ||  h = help");

	jump();
	attroff(A_REVERSE);
	jump();

	printw("\n\n");
}


int print_title(void) {
	jump();printw("|======================================|\n");
	jump();printw("|  |^^^\\    _      _       _    || ||  |\n");
	jump();printw("|  |[_]/  /^^^\\  //^\\\\  //^\\\\   || ||  |\n");
	jump();printw("|  || \\\\ ||===\' ||(    ||( )\\\\  || ||  |\n");
	jump();printw("|  || ||  \\\\=//  \\\\=//  \\\\=//\\\\ || ||  |\n");
	jump();printw("|             a memory training tool   |\n");
	jump();printw("|======================================|\n\n\n");
}

int get_line(int question_number){			//Read random line from file

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


int get_question(){					//Get question and answer and store them in global variables

	short answer_length = 0;
	short line_length = 0;
	short question_length = 0;

	while(true){
		get_line(get_random_line_number());	//Store random line from file in "line"
		if(strstr(line, "`") != NULL){		//Make sure line if properly formatted. If not, get a different line.
			break;
		}
	}

	char *start_of_answer = strchr(line, '`');	//Fine where '`' is that marks the beginning of the answer
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
	print_head();
	print_title();
	jump();
	printw("%s", question);				//Print line
	return 0;
}


int show_answer(void){
	erase();					//Erase contents of screen
	print_head();
	print_title();
	jump();
	printw("%s\n\n\n\n", question);			//Print line and answer
	jump();
	printw("%s\n", answer);				//Print line and answer
	return 0;
}

int get_recallrc_path(void){
	strcpy(home_dir_path, getenv("HOME"));				//Copy path to home directory to home_dir_path

	strcpy(recallrc_path, home_dir_path);
	strcat(recallrc_path, "/.recall/recallrc");			//Store location of recallrc in recallrc_path

	if(access(recallrc_path, F_OK | W_OK) == -1){
		attron(A_REVERSE);
		printw("Error\n\n");
		attroff(A_REVERSE);

		printw("Recall cannot find your recallrc file. It was expected in the following location: ");

		attron(A_BOLD);
		printw("%s\n\n", recallrc_path);
		attroff(A_BOLD);

		printw("Without it, Recall can't find the directory in which you store your Recall list files.\n\n\n");

		attron(A_REVERSE);
		printw("Press any key to exit.");
		attroff(A_REVERSE);
		
		getch();
		return(-1);
	}
}


int get_list_dir_path(void){
	fp = fopen(recallrc_path, "r");					//Open recallrc in read mode

	while(true){							//Cycle through the lines of recallrc to find a line containing "PATH"
		fgets(list_dir_path, 400, fp);
		if(strstr(list_dir_path, "PATH") != NULL){
			break;
		}
		if(feof(fp)){
			attron(A_REVERSE);
			printw("Error\n\n");
			attroff(A_REVERSE);

			printw("It appears that there is no properly formatted path in your recallrc file (");

			attron(A_BOLD);
			printw("%s", recallrc_path);
			attroff(A_BOLD);

			printw("). Please modify your recallrc (");

			attron(A_BOLD);
			printw("%s", recallrc_path);
			attroff(A_BOLD);

			printw("). There needs to be a line that contains the string \"PATH\" and then the path to your Recall list directory. An example is shown two lines below this one.\n\n");
			printw("PATH ~/Documents/RecallLists\n\n\n\n");

			attron(A_REVERSE);
		       	printw("Press any key you exit.");
			attroff(A_REVERSE);

			getch();
			return(-1);
		}
	}

	list_dir_path[strcspn(list_dir_path, "\r\n")] = 0;		//Remove end of line characters

	if(strstr(list_dir_path, "~") != NULL){				//Check whether path is relative
		char *start_of_path = strchr(list_dir_path, '/');
		strcpy(list_dir_path, &start_of_path[0]);		//Remove everything up to the ~

		char rel_path[400];
		strcpy(rel_path, list_dir_path);
		strcpy(list_dir_path, home_dir_path);
		strcat(list_dir_path, rel_path);			//Cat relative path to the end of home path
	}
	else if(strstr(list_dir_path, "/") != NULL){			//Check whether path is absolute
		char *start_of_path = strchr(list_dir_path, '/');
		strcpy(list_dir_path, &start_of_path[0]);		//Remove everything up to the /
	}
	else {								//If path is neither identified as relative or absolute, consider it invalid
		attron(A_REVERSE);
		printw("Error\n\n");
		attroff(A_REVERSE);

		printw("Your recallrc file (");

		attron("A_BOLD");
		printw("%s", recallrc_path);
		attroff("A_BOLD");

		printw(") does not appear to contain a valid path to your Recall list directory. A valid path must either begin with a \"/\" or a \"~\". Please review your recallrc file (");

		attron("A_BOLD");
		printw("%s", recallrc_path);
		attroff("A_BOLD");

		printw(") to make sure that a valid path is given on the PATH line. Below is a valid example of a path\n\n");

		attron("A_REVERSE");
		printw("PATH ~/Documents/RecallLists");
		attroff("A_REVERSE");

		getch();
		return(-1);
	}

	fclose(fp);							//Close file
	
	if(access(list_dir_path, F_OK | W_OK) == -1){
		attron(A_REVERSE);
		printw("Error\n\n");
		attroff(A_REVERSE);

		printw("Your recallrc file (");

		attron(A_BOLD);
		printw("%s", recallrc_path);
		attroff(A_BOLD);

		printw(") indicates that your Recall list files are stored in \"");

		attron(A_BOLD);
		printw("%s", list_dir_path);
		attroff(A_BOLD);

		printw("\", but that directory does not seem to exist.\n\n\n");

		attron(A_REVERSE);
		printw("Press any key to exit.");
		attroff(A_REVERSE);

		getch();
		return(-1);
	}
	return 0;
}


int open_file(void){
	char file_name[400];
	curs_set(1);					//Show cursor
	erase();					//Erase contents of screen
	printw("\n\n");
	print_title();
	jump();
	printw("Type the name of the file that you want to open.\n\n\n\n");
	jump();
	printw(":");
	scanw("%s", file_name);				//Store file name
	file_path[0] = '\0';				//Clear file_path for new file path
	strcat(file_path, list_dir_path);
	strcat(file_path, file_name);
	strcat(file_path, ".txt");

	while(access(file_path, F_OK | W_OK) == -1){	//Keep showing an error message if the file does not exist
		erase();
		printw("\n\n");
		print_title();
		jump();
		printw("\"%s\" cannot be found. Please type a different file name.\n\n", file_path);
		jump();
		printw(" ");

		scanw("%s", file_name);			//Store file name
		file_path[0] = '\0';			//Clear file_path for new file path
		strcat(file_path, list_dir_path);	//Add path to RecallLists folder to file_path
		strcat(file_path, file_name);		//Add file name to file path
		strcat(file_path, ".txt");
	}
	curs_set(0);					//Hide cursor
}


int help_message(void){
	erase();

	attron(A_REVERSE);
	attron(A_BOLD);
	printw("Press any key to exit this help page.\n\n\n");
	attroff(A_BOLD);

	printw("* * * WHAT IS RECALL * * *\n");
	attroff(A_REVERSE);
	printw("Recall is a free program designed to aid in memorization. It stimulates a user's memory through active recall by randomly cycling through sets of questions and answers.\n\n");

	attron(A_REVERSE);
	printw("* * * HOW TO OPEN A FILE * * *\n");
	attroff(A_REVERSE);
	printw("The questions and answers are stored in .txt files in a specific directory. This directory's location is stored in the ~/.recall/recallrc file. When Recall is launched, it prompts the user to enter the name of a file. The file name should be entered without including the .txt file extension. The file will then load into Recall. If a file is already open, pressing 'f' will allow for a different file to be selected.\n\n");

	attron(A_REVERSE);
	printw("* * * HOW TO CYCLE QUESTIONS * * *\n");
	attroff(A_REVERSE);
	printw("Once a flashcard file is open, one of the questions will be displayed at random. Then, the space bar or 'j' key can be pressed to display the question's answer. Then, the space bar or 'j' key can be pressed to display the next question.\n\n");

	attron(A_REVERSE);
	printw("* * * CREATING NEW FLASHCARD FILES * * *\n");
	attroff(A_REVERSE);
	printw("Questions can be added to a .txt file with any text editor. Each question/answer pair should be on its own line. The question and answer should be separated by a \'`\'. An example is shown below.\n\n");
	printw("What is the name of our nearest star?`The Sun\n");
	printw("What shape has eight sides?`An Octagon\n");
	printw("What colors make green?`Blue and yellow make green.\n\n");

	attron(A_REVERSE);
	printw("* * * More Information * * *\n");
	attroff(A_REVERSE);
	printw("For more information, please see https://various-and-sundry.com/recall.html");
	getch();
}


int main(void){
	initscr();
	curs_set(0);					//Hide cursor

	char c;						//Variable that will be used to read keystrokes

	if(get_recallrc_path() == -1){			//Store environment variable $HOME in home_dir_path; Store recallrc file location in recallrc_path.
		endwin();
		return(-1);				//If recallrc or the RecallLists directory do not exist, exit the program.
	}
	if(get_list_dir_path() == -1){			//Store environment variable $HOME in list_dir_path.
		endwin();
		return(-1);				//If recallrc or the RecallLists directory do not exist, exit the program.
	}
	open_file();					//Get a file name from the user and store it

	while(true){
		get_question();				//Read random line from file, extract question and answer, and store them in variables "question" and "answer".

		while(true){
			ask_question();				//Show question.

			c = getch();				//Wait for keystroke

			if(c == 'j' || c == 'J' || c == ' '){	//If keystroke was a 'j', 'J', and ' ', exit loop to show answer
				break;
			}if(c == 'x' || c == 'X'){		//If keystroke was an 'x' or 'X', close program.
				endwin();
				return 0;
			}if(c == 'f' || c == 'F'){		//If keystroke was an 'f' or 'F', allow user to open new file.
				open_file();
				continue;
			}if(c == 'h' || c == 'H'){		//if keystroke was an 'h' or 'H', show help screen.
				help_message();
				continue;
			}
		}


		while(true){
			show_answer();				//She answer along with question.

			c = getch();				//Wait for keystroke

			if(c == 'j' || c == 'J' || c == ' '){	//If keystroke was a 'j', 'J', and ' ', exit loop to show next question
				break;
			}if(c == 'x' || c == 'X'){		//If keystroke was an 'x' or 'X', close program.
				endwin();
				return 0;
			}if(c == 'f' || c == 'F'){		//If keystroke was an 'f' or 'F', allow user to open new file.
				open_file();
				continue;
			}if(c == 'h' || c == 'H'){		//if keystroke was an 'h' or 'H', show help screen.
				help_message();
				continue;
			}
		}
	}
}
