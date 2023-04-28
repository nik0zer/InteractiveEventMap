#include "sql_tools.h"


//----------------------------------------------------------------
//  This part realises lib of classes which interacts with SQL databases
//  Designed for project Interactive Map (group project for 4sem in MIPT)
//
//  @copyright AlexZ
//----------------------------------------------------------------


#define PADDING 30



//----------------------------------------------------------------
//!  Open database for subsequent interactions
//
//!  @param  [in]   a - coefficient
//!  @return        sqlite* DB - pointer for openned database
//!  @note          In case of error in open DB, throw runtime error
//!  @copyright     AlexZ
//----------------------------------------------------------------

sqlite3* open_db(const std::string path) 
{
    sqlite3* DB = nullptr; 
  
    if (sqlite3_open(path.c_str(), &DB)) 
    {
        std::string error_msg = "Can't open Data Base";
        spdlog::error("{}: {}", error_msg, sqlite3_errmsg(DB));

        throw std::runtime_error(error_msg);
    }

    spdlog::info("{:30} ... {}", "Openning Data Base", "OK");

    return DB;
}



//----------------------------------------------------------------
//!  Creating 2 tables:
//!  - "CREDS"  - users logins and passwords
//!  - "EVENTS" - info about events
//
//!  @param  [in]   DB - pointer on DB
//!  @return        None
//!  @note          In case of error in open DB, throw runtime error
//!  @copyright     AlexZ
//----------------------------------------------------------------

void create_tables(sqlite3* DB) 
{
    if (!DB)
    {
        std::string error_msg = "DB doesn't exists, can't create tables!";
        spdlog::critical("{}: {}", error_msg, sqlite3_errmsg(DB));

        throw std::runtime_error(error_msg);
    }


    // CMD to create table for autetification
    std::string sql_cmd = "CREATE TABLE CREDS("
                          "ID INT PRIMARY KEY     NOT NULL, "
                          "LOGIN          TEXT    NOT NULL, "
                          "PASSWORD       TEXT    NOT NULL  );";


    // Reserve pointer for collect errors of sql commands
    char* messaggeError;


    // Creating table
    if (sqlite3_exec(DB, sql_cmd.c_str(), NULL, 0, &messaggeError) != SQLITE_OK) 
    {
        std::string error_msg = "Can't create table CREDS";
        spdlog::warn("{}: {}", error_msg, messaggeError ? messaggeError : "");
        sqlite3_free(messaggeError);
    }

    spdlog::info("{:30} ... {}", "Creating table CREDS", "OK");


    // CMD to create table for events info
    sql_cmd = "CREATE TABLE EVENTS("
                      "ID INT PRIMARY KEY     NOT NULL, "
                      "NAME           TEXT    NOT NULL, "
                      "INFO           TEXT            , "
                      "ADDRESS        VARCHAR(100)    );";


    // Creating table
    if (sqlite3_exec(DB, sql_cmd.c_str(), NULL, 0, &messaggeError) != SQLITE_OK) 
    {
        std::string error_msg = "Can't create table EVENTS";
        spdlog::warn("{}: {}", error_msg, messaggeError ? messaggeError : "");
        sqlite3_free(messaggeError);
    }

    spdlog::info("{:30} ... {}", "Creating table EVENTS", "OK");

}







//----------------------------------------------------------------
//!  Creating 2 tables:
//!  - "CREDS"  - users logins and passwords
//!  - "EVENTS" - info about events
//
//!  @param  [in]   DB - pointer on DB
//!  @return        None
//!  @note          In case of error in open DB, throw runtime error
//!  @copyright     AlexZ
//----------------------------------------------------------------

int callback_creds(void* data, int argc, char** argv, char** azColName) 
{
    if (data)
    {
        spdlog::warn("Reveived data: {}", (const char*)data ? (const char*)data : "");
        return 0;
    }

    Person temp(atoi(argv[0]), argv[1], argv[2]);

    for (int i = 0; i < argc; i++) {
        printf("%s = %s; ", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");

    std::cout << temp;

    DataBase::get_instance().persons_list_.push_back(temp);
    DataBase::get_instance().print_persons_list();

    return 0;
}



int test_db(sqlite3* DB) 
{
    if (!DB) 
    {
        std::cout << "Error! Null DB in " <<  __func__ << std::endl;
        return -1;
    }


    std::string query = "SELECT * FROM CREDS;";
    std::string sql   = "INSERT INTO CREDS VALUES(1, 'STEVE', 'STEVE');"
                        "INSERT INTO CREDS VALUES(2, 'ATEVE', 'STEVE');";

  
    std::cout << "STATE OF CREDS BEFORE INSERT" << std::endl;
    sqlite3_exec(DB, query.c_str(), callback_creds, NULL, NULL);

  
    char* messaggeError;
    int exit;
    

    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);

    if (exit != SQLITE_OK) 
    {
        std::cerr << "Error Insert" << std::endl;
        sqlite3_free(messaggeError);
        exit = 0;
    }

    else
    {
        std::cout << "Records created Successfully!" << std::endl;
    }
  

    std::cout << "\nSTATE OF TABLE AFTER INSERT" << std::endl;
    sqlite3_exec(DB, query.c_str(), callback_creds, NULL, NULL);
  

    sql = "DELETE FROM CREDS WHERE ID = 2;";
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);


    if (exit != SQLITE_OK) 
    {
        std::cerr << "Error DELETE" << std::endl;
        sqlite3_free(messaggeError);
    }

    else
    {
        std::cout << "Record deleted Successfully!" << std::endl;
    }
  

    std::cout << "STATE OF TABLE AFTER DELETE OF ELEMENT" << std::endl;
    sqlite3_exec(DB, query.c_str(), callback_creds, NULL, NULL);


    std::cout << "End of test_db\n";
    return 0;
}



void main_test() 
{
    sqlite3* DB;
    const std::string path_to_database = "example.db";

    std::ofstream outfile(path_to_database);
    std::remove(path_to_database.c_str());

    open_db(path_to_database);
    create_tables(DB);

    // if (test_db(DB) != 0) {std::cout << "Error in test_db\n"; return;}

    sqlite3_close(DB);

    std::remove(path_to_database.c_str());
}


// ---------------------------------------------------------------------------------------------------------------------
// Realisation of class "DataBase"
// ---------------------------------------------------------------------------------------------------------------------


DataBase::DataBase()
{
    ptr_ = open_db("main.db");
    create_tables(ptr_);
}


DataBase& DataBase::get_instance()
{
    static DataBase base;

    return base;
}


void DataBase::add_person(Person& person)
{
    // Construct sql cmd
    std::string sql_cmd = "INSERT INTO CREDS VALUES(";
    sql_cmd += std::to_string(get_next_id()) + ", '";
    sql_cmd += person.login_ + "', '";
    sql_cmd += person.password_ + "');";


    char* messaggeError;

    if (sqlite3_exec(ptr_, sql_cmd.c_str(), NULL, 0, &messaggeError) != SQLITE_OK) 
    {
        std::string error_msg = "INSERT Error (cmd = '''" + sql_cmd + "''')";
        spdlog::error ("{}: {}", error_msg, messaggeError ? messaggeError : "");
        sqlite3_free(messaggeError);
        return;
    }

    spdlog::info("{:30} ... {}", "Adding person '" + person.login_ + "'", "OK");

}



int DataBase::get_next_id()
{
    int i;
    for (i = 1; reserved_id_.size() != 0 && reserved_id_.find(i) != reserved_id_.end(); i++) ;
    reserved_id_.insert(i);

    return i;
}



void DataBase::print_all_creds()
{
    std::string sql_cmd = "SELECT * FROM CREDS;";
    sqlite3_exec(ptr_, sql_cmd.c_str(), callback_creds, NULL, NULL);
}



void DataBase::print_persons_list()
{
    std::cout << "Persons:\n";

    for (const auto& item : persons_list_)
    {
        std::cout << item;
    }

    std::cout << std::endl;
}

// ---------------------------------------------------------------------------------------------------------------------
// Realisation of class "DataBase"
// ---------------------------------------------------------------------------------------------------------------------

std::ostream& operator<< (std::ostream &out, const Person &person)
{
    out << person.id_ << "    " << person.login_ << "    " << person.password_ << std::endl;

    return out;
}