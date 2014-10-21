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
   while((option = getopt(argc, argv, "ly")) != -1){
      switch(option){
         case 'l':
            fprintf(stderr, "Option '-l' not yet implemented\n");
            break;
         case 'y':
            fprintf(stderr, "Option '-y' not yet implemented\n");
            break;
         default:
            fprintf(stderr, "Unrecognized option: '-%c'\n", optopt);
            set_exitstatus(EXIT_FAILURE);
      }
   }
   
   return get_exitstatus();
}
