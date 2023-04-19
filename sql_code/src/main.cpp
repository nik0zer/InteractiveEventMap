#include <iostream>
#include <fstream>
#include "sqlite3.h"

static int callback(void* data, int argc, char** argv, char** azColName) {
    int i;
    fprintf(stderr, "Data = %s:\n", (const char*)data ? (const char*)data : "OK");
  
    for (i = 0; i < argc; i++) {
        printf("%s = %s; ", azColName[i], argv[i] ? argv[i] : "NULL");
    }
  
    printf("\n");
    return 0;
}


sqlite3* open_db(const std::string path) {
    sqlite3* DB = nullptr;
  
    if (sqlite3_open(path.c_str(), &DB)) {
        std::cerr << "Error open DB: " << sqlite3_errmsg(DB) << std::endl;
        return nullptr;
    }
    std::cout << "Database openned Successfully!" << std::endl;
    return DB;
}


int create_main_table(sqlite3* DB) {
    std::string sql = "CREATE TABLE PERSON("
                      "ID INT PRIMARY KEY     NOT NULL, "
                      "NAME           TEXT    NOT NULL, "
                      "SURNAME        TEXT    NOT NULL, "
                      "AGE            INT     NOT NULL, "
                      "ADDRESS        CHAR(50), "
                      "SALARY         REAL );";

    char* messaggeError;
    if (sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError) != SQLITE_OK) {
        std::cerr << "ERROR! Create Table" << std::endl;
        sqlite3_free(messaggeError);
        return -1;
    }
    std::cout << "Table    created Successfully!" << std::endl;
    return 0;
}


int test_db(sqlite3* DB) {
    if (!DB) {
        std::cout << "Error! Null DB in " <<  __func__ << std::endl;
        return -1;
    }

    std::string sql = "INSERT INTO PERSON VALUES(1, 'STEVE', 'GATES', 30, 'PALO ALTO', 1000.0);"
                      "INSERT INTO PERSON VALUES(2, 'BILL', 'ALLEN', 20, 'SEATTLE', 300.22);"
                      "INSERT INTO PERSON VALUES(3, 'PAUL', 'JOBS', 24, 'SEATTLE', 9900.0);";
    std::string query = "SELECT * FROM PERSON;";
  
    std::cout << "STATE OF TABLE BEFORE INSERT" << std::endl;

    sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);
  
    char* messaggeError;
    int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
    if (exit != SQLITE_OK) {
        std::cerr << "Error Insert" << std::endl;
        sqlite3_free(messaggeError);
        exit = 0;
    }
    else
        std::cout << "Records created Successfully!" << std::endl;
  
    std::cout << "\nSTATE OF TABLE AFTER INSERT" << std::endl;
  
    sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);
  
    sql = "DELETE FROM PERSON WHERE ID = 2;";
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
    if (exit != SQLITE_OK) {
        std::cerr << "Error DELETE" << std::endl;
        sqlite3_free(messaggeError);
    }
    else
        std::cout << "Record deleted Successfully!" << std::endl;
  
    std::cout << "STATE OF TABLE AFTER DELETE OF ELEMENT" << std::endl;
    sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);

    std::cout << "End of test_db\n";
    return 0;
}


int main(int argc, char** argv) {
    sqlite3* DB;
    const std::string path_to_database = "example.db";

    std::ofstream outfile(path_to_database.c_str());
    std::remove(path_to_database.c_str());

    if ((DB = open_db(path_to_database)) == nullptr) return -1;
    if (create_main_table(DB) != 0) {std::cout << "Error in create_main_table\n"; return -1;}
    if (test_db(DB) != 0) {std::cout << "Error in test_db\n"; return -1;}
    sqlite3_close(DB);

    std::remove(path_to_database.c_str());


    return (0);
}