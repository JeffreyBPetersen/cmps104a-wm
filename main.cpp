// Jeffrey Petersen | jebpeter@ucsc.edu

#include <string>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
using namespace std;

#include "auxlib.h"
#include "cppstrtok.h"

int main (int argc, char** argv) {
   set_execname (argv[0]);
   /*
   // main from cppstrtok.cpp
   
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
   string path;
   string program_name;
   while((option = getopt(argc, argv, "ly@:D:")) != -1){
      switch(option){
         case '@':
            fprintf(stderr, 
            "Option '-@' not yet implemented, flags: %s\n", optarg);
            break;
         case 'D':
            fprintf(stderr, 
            "Option '-D' not yet implemented, string: %s\n", optarg);
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
      path = argv[optind];
      program_name = basename(strdup(path.c_str()));
      program_name = program_name.substr(0, program_name.find("."));
      fprintf(stderr,
      "File input not yet implemented, path: %s, filename: %s\n",
      path.c_str(), program_name.c_str());
   }
   /*
      ADD: modify cpplines in cppstrtok to return tokens
      pass file path to cpplines and get tokens
   */
   /*
      ADD: Insert tokens into the string set
   */
   /*
      ADD: Dump string set into [program].str
   */
   return get_exitstatus();
}
