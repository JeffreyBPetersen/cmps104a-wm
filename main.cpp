// Jeffrey Petersen | jebpeter@ucsc.edu

#include <fstream>
#include <libgen.h>
#include <string.h>
#include <unistd.h>
using namespace std;

#include "auxlib.h"
#include "cppstrtok.h"
#include "lyutils.h"
#include "stringset.h"
#include "yyparse.h"

extern FILE* yyin;
extern int yylex();

int main (int argc, char** argv) {
   int option;
   string path;
   string program_name;
   
   set_execname(argv[0]);
   
   // get flag options
   yy_flex_debug = 0;
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
            yy_flex_debug = 1;
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
      if(program_name.substr(program_name.rfind(".")) != ".oc"){
         fprintf(stderr, "File format must be .oc\n");
         set_exitstatus(EXIT_FAILURE);
         continue;
      }
      program_name = program_name.substr(0, program_name.rfind("."));
      string command = CPP + " " + path;
      
      yyin = popen(command.c_str(), "r");
      if(yyin == NULL){
         fprintf(stderr, "Failed to open file input pipe\n");
         set_exitstatus(EXIT_FAILURE);
      }else{
         ofstream str_output;
         str_output.open(program_name + ".str");
         tok_output.open(program_name + ".tok");
         while(yylex() != YYEOF);
         dump_stringset(str_output);
      }
   }
   if(pclose(yyin) != 0)
      set_exitstatus(EXIT_FAILURE);
   return get_exitstatus();
}
