// Jeffrey Petersen | jebpeter@ucsc.edu

#include <string>
#include <unistd.h>
using namespace std;

#include "auxlib.h"
#include "cppstrtok.h"

int main (int argc, char** argv) {
   set_execname (argv[0]);
   /*
   for (int argi = 1; argi < argc; ++argi) {
      char* filename = argv[argi];
      string command = CPP + " " + filename;
      printf ("command=\"%s\"\n", command.c_str());
      FILE* pipe = popen (command.c_str(), "r");
      if (pipe == NULL) {
         syserrprintf (command.c_str());
      }else {
         cpplines (pipe, filename);
         int pclose_rc = pclose (pipe);
         eprint_status (command.c_str(), pclose_rc);
      }
   }
   */
   
   int option;
   while((option = getopt(argc, argv, "ly@:D:")) != -1){
      switch(option){
			case '@':
				fprintf(stderr, "Option '-@' not yet implemented, flags: %s\n", optarg);
				break;
			case 'D':
				fprintf(stderr, "Option '-D' not yet implemented, string: %s\n", optarg);
				break;
         case 'l':
            fprintf(stderr, "Option '-l' not yet implemented\n");
            break;
         case 'y':
            fprintf(stderr, "Option '-y' not yet implemented\n");
            break;
         default:
            set_exitstatus(EXIT_FAILURE);
      }
   }
	if(optind < argc){
		fprintf(stderr, "File input not yet implemented, file: %s\n", argv[optind]);
		/*
			ADD: Support directories, find file and separate out filename if necessary
		*/
	}
	/*
		ADD: Filter input through the C preprocessor, /usr/bin/cpp
	*/
	/*
		ADD: Read line using fgets(3), 
		tokenize it along whitespace using strtok_r(3) with the string " \t\n",
		then insert it into the string set
   */
	/*
		ADD: Dump string set into trace file
	*/
   return get_exitstatus();
}
