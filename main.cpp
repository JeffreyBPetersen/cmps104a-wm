// Jeffrey Petersen | jebpeter@ucsc.edu

#include <fstream>
#include <libgen.h>
#include <string.h>
#include <unistd.h>
using namespace std;

#include "auxlib.h"
#include "cppstrtok.h"
#include "stringset.h"

int main (int argc, char** argv) {
   int option;
   string path;
   string program_name;
   
   // get flag options
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
      
      // get program name
      path = argv[arg_index];
      program_name = basename(strdup(path.c_str()));
      //ADD: check for .oc extension
      program_name = program_name.substr(0, program_name.find("."));
      string command = CPP + " " + path;
      
      // get piped input from c preprocessor
      FILE* pipe = popen(command.c_str(), "r");
      if(pipe == NULL){
         fprintf(stderr, "Failed to open file input pipe\n");
         set_exitstatus(EXIT_FAILURE);
      }else{
      
         // tokenize input from pipe
         tokens = cpplines(pipe, strdup(path.c_str()));
         int cpp_exit = pclose(pipe);
         if(cpp_exit != 0)
            set_exitstatus(1);
      }
      
      // add tokens to stringset
      for(auto token_iter = tokens.begin();
      token_iter != tokens.end(); ++token_iter){
         intern_stringset(token_iter->c_str());
      }
      
      // output stringset to .str file
      ofstream str_output;
      str_output.open(program_name + ".str");
      dump_stringset(str_output);
      str_output.close();
   }
   return get_exitstatus();
}
