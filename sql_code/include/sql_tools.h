#ifndef SQL_TOOLS
#define SQL_TOOLS

#include <iostream>
#include <fstream>
#include "sqlite3.h"


static int callback(void* data, int argc, char** argv, char** azColName);
sqlite3* open_db(const std::string path);
int create_main_table(sqlite3* DB);
int test_db(sqlite3* DB);
void main_test();



#endif