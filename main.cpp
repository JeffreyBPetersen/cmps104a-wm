// Jeffrey Petersen | jebpeter@ucsc.edu

#include <string.h>
#include <unistd.h>
#include <libgen.h>
using namespace std;

#include "auxlib.h"
#include "cppstrtok.h"
#include "stringset.h"

int main (int argc, char** argv) {
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
   for(int arg_index = optind; arg_index < argc; arg_index++){
      vector<string> tokens;
      path = argv[arg_index];
      program_name = basename(strdup(path.c_str()));
      program_name = program_name.substr(0, program_name.find("."));
      string command = CPP + " " + path;
      printf("command=\"%s\"\n", command.c_str()); /// DEBUG
      FILE* pipe = popen(command.c_str(), "r");
      if(pipe == NULL){
         fprintf(stderr, "Failed to open file input pipe\n");
         set_exitstatus(EXIT_FAILURE);
      }else{
         tokens = cpplines(pipe, strdup(path.c_str()));
         pclose(pipe);
      }
      for(auto token_iter = tokens.begin();
      token_iter != tokens.end(); ++token_iter){
         printf("%s\n", token_iter->c_str()); /// DEBUG
         intern_stringset(token_iter->c_str());
      }
      /*
         ADD: Dump string set into [program].str
      */
   }
   return get_exitstatus();
}
