// Jeffrey Petersen | jebpeter@ucsc.edu

#include <string>
#include <vector>

const string CPP = "/usr/bin/cpp";
const size_t LINESIZE = 1024;

vector<string> cpplines (FILE* pipe, char* filename);
