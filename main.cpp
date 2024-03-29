// Jeffrey Petersen | jebpeter@ucsc.edu

#include <fstream>
#include <libgen.h>
#include <string.h>
#include <unistd.h>
using namespace std;

#include "astree.h"
#include "auxlib.h"
#include "lyutils.h"
#include "stringset.h"
#include "symtable.h"
#include "yyparse.h"

extern FILE* yyin;
extern int yylex();

int main (int argc, char** argv) {
   int option;
   string path;
   string program_name;
	bool assignment_option = false;
   
   set_execname(argv[0]);
   
   // get flag options
   yy_flex_debug = 0;
   while((option = getopt(argc, argv, "aly@:D:")) != -1){
      switch(option){
         case '@':
            fprintf(stderr, 
            "Option '-@' not yet implemented, flags: %s\n", optarg);
            break;
         case 'D':
            fprintf(stderr, 
            "Option '-D' not yet implemented, string: %s\n", optarg);
            break;
			case 'a':
				assignment_option = true;
				break;
         case 'l':
            yy_flex_debug = 1;
            break;
         case 'y':
            yydebug = 1;
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
      string command = "/usr/bin/cpp " + path;
      
      yyin = popen(command.c_str(), "r");
      if(yyin == NULL){
         fprintf(stderr, "Failed to open file input pipe\n");
         set_exitstatus(EXIT_FAILURE);
      }else{
         ofstream str_output;
         ofstream ast_output;
         str_output.open(program_name + ".str");
			tok_output.open(program_name + ".tok");
			ast_output.open(program_name + ".ast");
         if(!assignment_option)
				sym_output.open(program_name + ".sym");
         //while(yylex() != YYEOF);
         yyparse();
			if(!assignment_option)
				gen_symtable(yyparse_astree);
         dump_stringset(str_output);
         output_ast(ast_output, yyparse_astree);
      }
   }
   if(pclose(yyin) != 0)
      set_exitstatus(EXIT_FAILURE);
   return get_exitstatus();
}
