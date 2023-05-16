//----------------------------------------------------------------
//  This part realises lib of classes which interacts with SQL databases
//  Designed for project Interactive Map (group project for 4sem in MIPT)
//
//  @copyright AlexZ
//----------------------------------------------------------------


#include "sql_new.h"


// ---------------------------------------------------------------------------------------------------------------------
// Constructions for communication with server
// 
// Operator std::string convert object to string, parsing by \n
// Construcotr of msg - construct the object from msg by parsing msg by \n
// ---------------------------------------------------------------------------------------------------------------------



User::operator std::string() const 
{
    std::string str = std::to_string(id_) + "\n";
    str += login_ + "\n";
    str += password_ + "\n";
    str += std::to_string(last_edit_time_) + "\n";
    str += std::to_string(archived_) + "\n";
    return str;
}



User::User(std::string msg)
{
    std::vector<std::string> data_vector;
    std::string temp;
    std::stringstream input_stream(msg);

    for (int i = 0; i < 5; i++)
    {
        std::getline(input_stream, temp);
        data_vector.push_back(temp);
        // spdlog::info("Readed part = '{}'", temp);
    }

    id_             = std::atoi(data_vector[0].c_str());
    login_          = data_vector[1];
    password_       = data_vector[2];
    last_edit_time_ = std::atol(data_vector[3].c_str());
    archived_       = std::atoi(data_vector[4].c_str());

}



Event::operator std::string() const 
{
    std::string str = std::to_string(id_) + "\n";
    str += name_ + "\n";
    str += info_ + "\n";
    str += address_ + "\n";
    str += date_ + "\n";
    str += time_ + "\n";
    str += owner_ + "\n";
    str += std::to_string(last_edit_time_) + "\n";
    str += std::to_string(archived_) + "\n";
    return str;
}



Event::Event(std::string msg)
{
    std::vector<std::string> data_vector;
    std::string temp;
    std::stringstream input_stream(msg);

    for (int i = 0; i < 9; i++)
    {
        std::getline(input_stream, temp);
        data_vector.push_back(temp);
    }


    id_             = std::atoi(data_vector[0].c_str());
    name_           = data_vector[1];
    info_           = data_vector[2];
    address_        = data_vector[3];
    date_           = data_vector[4];
    time_           = data_vector[5];
    owner_          = data_vector[6];
    last_edit_time_ = std::atol(data_vector[7].c_str());
    archived_       = std::atoi(data_vector[8].c_str());
}



// ---------------------------------------------------------------------------------------------------------------------
// std::cout operators <<
// ---------------------------------------------------------------------------------------------------------------------



std::ostream& operator<< (std::ostream &out, const User &User)
{
    out << User.id_ << "    " << User.login_ << "    " << User.password_ << "    " << User.last_edit_time_ << "    " 
        << User.archived_;

    return out;
}



std::ostream& operator<< (std::ostream &out, const Event &event)
{
    out << event.id_ << "    " << event.name_ << "    " << event.info_ << "    " << event.address_
                     << "    " << event.date_ << "    " << event.time_ << "    " << event.owner_ 
                     << "    " << event.last_edit_time_ << "    " << event.archived_;

    return out;
}



// ---------------------------------------------------------------------------------------------------------------------
// Initialize DataBase
// ---------------------------------------------------------------------------------------------------------------------



DataBase::DataBase()
{
    db_ = new sqlite::database("main.db");
    spdlog::info("Database 'main.db' openned");
}



DataBase::~DataBase()
{
    try
    {
        delete db_;
        spdlog::info("Database closed");
    }
    catch(std::exception& e)
    {
        spdlog::error("Can't close Database!");
    }
}



// ---------------------------------------------------------------------------------------------------------------------
// Table_Events
// ---------------------------------------------------------------------------------------------------------------------



void Table_Events::create_table_event(sqlite::database* db)
{
    db_ = db;

    try
    {
        *db_ << "create table if not exists EVENTS ("
                    "ID integer primary key autoincrement NOT NULL,"
                    "NAME                      TEXT       NOT NULL, "
                    "INFO                      TEXT               , "
                    "ADDRESS                   TEXT               , "
                    "DATE                      TEXT               , "
                    "TIME                      TEXT               , "
                    "OWNER                     TEXT               , "
                    "LAST_EDIT_TIME            INT                , "
                    "ARCHIVED                  INT               );";


        spdlog::info("Table EVENTS created");
    }
    catch (const sqlite::sqlite_exception& e) {

        spdlog::error("{}: {} during {}\nFile = '{}', function = '{}', line = '{}'", e.get_code(), e.what(), e.get_sql(),
                                                __FILE__, __FUNCTION__, __LINE__);
                                                
    }
}



void Table_Events::add_event(const Event& event) const
{
    // *db_ << u"INSERT into EVENTS (NAME,INFO,ADDRESS,DATE,TIME,OWNER,LAST_EDIT_TIME) values (?,?,?,?,?,?,?);"
    if (!db_) spdlog::critical("Zero db in add event");

    try
    {   
        int event_exists = 0;   // False

        *db_ << u"SELECT count(*) FROM EVENTS WHERE NAME=?;" << event.get_name() >> event_exists;

        if (event_exists)
        {
            spdlog::error("Can't add event! Event already exists. (name = '{}')", event.get_name());
            return;
        }


        *db_ << u"INSERT INTO EVENTS (NAME,INFO,ADDRESS,DATE,TIME,OWNER,LAST_EDIT_TIME,ARCHIVED) VALUES (?,?,?,?,?,?,?,?);"
            << event.get_name()
            << event.get_info()
            << event.get_address()
            << event.get_date()
            << event.get_time()
            << event.get_owner()
            << event.get_last_edit_time()
            << event.get_archived();

        spdlog::info("Event '{}' added", event.get_name());
    }
    catch (const sqlite::sqlite_exception& e) {

        spdlog::error("{}: {} during {}\nFile = '{}', function = '{}', line = '{}'", e.get_code(), e.what(), e.get_sql(),
                                                __FILE__, __FUNCTION__, __LINE__);

    }
}



Event Table_Events::find_event_by_name(const std::string& name) const
{
    try
    {
        std::vector<Event> events_vec;
        *db_ << u"SELECT * FROM EVENTS WHERE NAME=?" << name >> [&](int id, std::string name, std::string info, std::string address, 
                std::string date, std::string time, std::string owner, size_t last_edit_time, bool archived)
                {
                    Event temp(id, name, info, address, date, time, owner, last_edit_time, archived);
                    events_vec.push_back(temp);
                };

        if (events_vec.size() > 1)
        {
            spdlog::critical("Found more than 1 event by name");
            return events_vec[0];
        }
        if (events_vec.size() == 1)
        {   
            spdlog::info("Found event named '{}'", name);
            return events_vec[0];
        }

    }
    catch (const sqlite::sqlite_exception& e) {

        spdlog::error("{}: {} during {}\nFile = '{}', function = '{}', line = '{}'", e.get_code(), e.what(), e.get_sql(),
                                                __FILE__, __FUNCTION__, __LINE__);

    }

    spdlog::warn("No event found by name = '{}'", name);
    return Event();
}



Event Table_Events::find_event_by_id(const int& id) const
{
    try
    {
        std::vector<Event> events_vec;
        *db_ << u"SELECT * FROM EVENTS WHERE ID=?" << id >> [&](int id, std::string name, std::string info, std::string address, 
                std::string date, std::string time, std::string owner, size_t last_edit_time, bool archived)
                {
                    Event temp(id, name, info, address, date, time, owner, last_edit_time, archived);
                    events_vec.push_back(temp);
                };

        if (events_vec.size() > 1)
        {
            spdlog::critical("Found more than 1 event by name");
            return events_vec[0];
        }
        if (events_vec.size() == 1)
        {   
            spdlog::info("Found event by id '{}'", id);
            return events_vec[0];
        }

    }
    catch (const sqlite::sqlite_exception& e) {

        spdlog::error("{}: {} during {}\nFile = '{}', function = '{}', line = '{}'", e.get_code(), e.what(), e.get_sql(),
                                                __FILE__, __FUNCTION__, __LINE__);

    }

    spdlog::warn("No event found by id = '{}'", id);
    return Event();
}



void Table_Events::remove_event_by_name(const std::string& name) const
{
    try
    {
        *db_ << u"UPDATE EVENTS SET ARCHIVED=TRUE WHERE NAME=?;" << name;


        spdlog::info("Event '{}' removed", name);
    }
    catch (const sqlite::sqlite_exception& e) {

        spdlog::error("{}: {} during {}\nFile = '{}', function = '{}', line = '{}'", e.get_code(), e.what(), e.get_sql(),
                                                __FILE__, __FUNCTION__, __LINE__);

    }
}



void Table_Events::remove_event_by_id(const int& id) const
{
    try
    {
        *db_ << u"UPDATE EVENTS SET ARCHIVED=TRUE WHERE ID=?;" << id;

        spdlog::info("Event with id '{}' removed", id);
    }
    catch (const sqlite::sqlite_exception& e) {

        spdlog::error("{}: {} during {}\nFile = '{}', function = '{}', line = '{}'", e.get_code(), e.what(), e.get_sql(),
                                                __FILE__, __FUNCTION__, __LINE__);

    }
}



void Table_Events::print_all_events() const
{
    std::vector<Event> events_vec;
    std::cout << "Print all events vector:\n";

    try
    {    
        *db_ << "SELECT * FROM EVENTS;" >> [&](int id, std::string name, std::string info, std::string address, 
                std::string date, std::string time, std::string owner, size_t last_edit_time, bool archived) 
        {
            Event temp(id, name, info, address, date, time, owner, last_edit_time, archived);
            events_vec.push_back(temp);
        };

        for (const auto& item : events_vec)
        {
            std::cout << item << std::endl;
        }
    }
    catch (const sqlite::sqlite_exception& e) {

        spdlog::error("{}: {} during {}\nFile = '{}', function = '{}', line = '{}'", e.get_code(), e.what(), e.get_sql(),
                                                __FILE__, __FUNCTION__, __LINE__);

    }
}



std::vector<Event> Table_Events::get_all_events() const
{
    std::vector<Event> events_vec;

    try
    {    
        *db_ << "SELECT * FROM EVENTS WHERE ARCHIVED=FALSE;" >> [&](int id, std::string name, std::string info, std::string address, 
                std::string date, std::string time, std::string owner, size_t last_edit_time, bool archived) 
        {
            Event temp(id, name, info, address, date, time, owner, last_edit_time, archived);
            events_vec.push_back(temp);
        };
    }
    catch (const sqlite::sqlite_exception& e) {

        spdlog::error("{}: {} during {}\nFile = '{}', function = '{}', line = '{}'", e.get_code(), e.what(), e.get_sql(),
                                                __FILE__, __FUNCTION__, __LINE__);

    }

    spdlog::info("Vector of all events returned");
    return events_vec;
}



void Table_Events::update_event(const Event& event) const
{
    remove_event_by_name(event.get_name());
    add_event(event);

    spdlog::info("Event '{}' updated", event.get_name());
}



void Table_Events::rename_event(const std::string& old_name, const std::string& new_name) const
{
    try
    {
        *db_ << u"UPDATE EVENTS SET NAME=? WHERE NAME=?" << new_name << old_name;

        spdlog::info("Event '{}' renamed -> '{}'", old_name, new_name);
    }
    catch (const sqlite::sqlite_exception& e) {

        spdlog::error("{}: {} during {}\nFile = '{}', function = '{}', line = '{}'", e.get_code(), e.what(), e.get_sql(),
                                                __FILE__, __FUNCTION__, __LINE__);

    }
}



time_t Table_Events::get_last_edit_time_events() const
{
    long long last_edit_time = 0;

    try
    {
        *db_ << u"SELECT MAX(LAST_EDIT_TIME) FROM EVENTS" >> last_edit_time;

        spdlog::info("Last edit time of events = {}", last_edit_time);
    }
    catch (const sqlite::sqlite_exception& e) {

        spdlog::error("{}: {} during {}\nFile = '{}', function = '{}', line = '{}'", e.get_code(), e.what(), e.get_sql(),
                                                __FILE__, __FUNCTION__, __LINE__);

    }
    
    return last_edit_time;
}



std::vector<Event> Table_Events::get_events_to_sync(time_t time)    const
{
    std::vector<Event> events_vec;

    try
    {
        *db_ << u"SELECT * FROM EVENTS WHERE LAST_EDIT_TIME >= ?;" << time >> [&](int id, std::string name, std::string info, std::string address, 
                std::string date, std::string time, std::string owner, size_t last_edit_time, bool archived) 
        {
            Event temp(id, name, info, address, date, time, owner, last_edit_time, archived);
            events_vec.push_back(temp);
        };

        spdlog::info("'{}' events found for sync", events_vec.size());
    }
    catch (const sqlite::sqlite_exception& e) {

        spdlog::error("{}: {} during {}\nFile = '{}', function = '{}', line = '{}'", e.get_code(), e.what(), e.get_sql(),
                                                __FILE__, __FUNCTION__, __LINE__);

    }

    return events_vec;
}



// ---------------------------------------------------------------------------------------------------------------------
// Table_Users
// ---------------------------------------------------------------------------------------------------------------------



void Table_Users::create_table_users(sqlite::database* db)
{
    db_ = db;

    try
    {
        *db_ << "create table if not exists USERS ("
                    "ID integer primary key autoincrement NOT NULL,"
                    "LOGIN                     TEXT       NOT NULL, "
                    "PASSWORD                  TEXT       NOT NULL, "
                    "LAST_EDIT_TIME            INT                , "
                    "ARCHIVED                  INT                );";

        spdlog::info("Table USERS created");    
    }
    catch (const sqlite::sqlite_exception& e) {

        spdlog::error("{}: {} during {}\nFile = '{}', function = '{}', line = '{}'", e.get_code(), e.what(), e.get_sql(),
                                                __FILE__, __FUNCTION__, __LINE__);

    }
}



void Table_Users::add_user(const User& user) const
{
    if (!db_) spdlog::critical("Zero db in add user");

    try
    {
        int user_exists = 0;   // False

        *db_ << u"SELECT count(*) FROM USERS WHERE LOGIN=?;" << user.get_login() >> user_exists;

        if (user_exists)
        {
            spdlog::error("Can't add user! User already exists. (login = '{}')", user.get_login());
            return;
        }


        *db_ << u"INSERT INTO USERS (LOGIN,PASSWORD,LAST_EDIT_TIME) VALUES (?,?,?);"
            << user.get_login()
            << sha256(user.get_password())
            << user.get_last_edit_time();

        spdlog::info("User '{}' added", user.get_login());
    }
    catch (const sqlite::sqlite_exception& e) {

        spdlog::error("{}: {} during {}\nFile = '{}', function = '{}', line = '{}'", e.get_code(), e.what(), e.get_sql(),
                                                __FILE__, __FUNCTION__, __LINE__);

    }
}



User Table_Users::find_user_by_login(const std::string& login) const
{
    std::vector<User> users_vec;


    try
    {
        *db_ << u"SELECT * FROM USERS WHERE LOGIN=?" << login >> [&](int id, std::string login, std::string password,
                                                                     size_t last_edit_time, bool archived)
                {
                    User temp(id, login, password, last_edit_time, archived);
                    users_vec.push_back(temp);
                };
    }
    catch (const sqlite::sqlite_exception& e) {

        spdlog::error("{}: {} during {}\nFile = '{}', function = '{}', line = '{}'", e.get_code(), e.what(), e.get_sql(),
                                                __FILE__, __FUNCTION__, __LINE__);

    }


    if (users_vec.size() > 1)
    {
        spdlog::critical("Found more than 1 user by login");
        return User("", "");
    }

    if (users_vec.size() == 1)
    {   
        spdlog::info("Found user with login '{}'", login);
        return users_vec[0];
    }

    spdlog::warn("No users found by login = '{}'", login);
    return User("", "");
}



User Table_Users::find_user_by_id(const int& id) const
{
    std::vector<User> users_vec;


    try
    {
        *db_ << u"SELECT * FROM USERS WHERE ID=?" << id >> [&](int id, std::string login, std::string password,
                                                                     size_t last_edit_time, bool archived)
                {
                    User temp(id, login, password, last_edit_time, archived);
                    users_vec.push_back(temp);
                };
    }
    catch (const sqlite::sqlite_exception& e) {

        spdlog::error("{}: {} during {}\nFile = '{}', function = '{}', line = '{}'", e.get_code(), e.what(), e.get_sql(),
                                                __FILE__, __FUNCTION__, __LINE__);

    }


    if (users_vec.size() > 1)
    {
        spdlog::critical("Found more than 1 user by id");
        return User("", "");
    }

    if (users_vec.size() == 1)
    {   
        spdlog::info("Found user with id '{}'", id);
        return users_vec[0];
    }

    spdlog::warn("No users found by id '{}'", id);
    return User("", "");
}



void Table_Users::remove_user_by_login(const std::string& login)  const
{
    try
    {
        *db_ << u"UPDATE USERS SET ARCHIVED=TRUE WHERE LOGIN=?;" << login;

        spdlog::info("User '{}' removed", login);
    }
    catch (const sqlite::sqlite_exception& e) {

        spdlog::error("{}: {} during {}\nFile = '{}', function = '{}', line = '{}'", e.get_code(), e.what(), e.get_sql(),
                                                __FILE__, __FUNCTION__, __LINE__);

    }
}



void Table_Users::remove_user_by_id(const int& id)  const
{
    try
    {
        *db_ << u"UPDATE USERS SET ARCHIVED=TRUE WHERE ID=?;" << id;

        spdlog::info("User with id '{}' removed", id);
    }
    catch (const sqlite::sqlite_exception& e) {

        spdlog::error("{}: {} during {}\nFile = '{}', function = '{}', line = '{}'", e.get_code(), e.what(), e.get_sql(),
                                                __FILE__, __FUNCTION__, __LINE__);

    }
}



void Table_Users::print_all_users() const
{
    std::cout << "Print all users:\n";

    try
    {
        *db_ << "SELECT LOGIN FROM USERS;" >> [&](std::string login) 
        {
            std::cout << login << std::endl;
        };
    }
    catch (const sqlite::sqlite_exception& e) {

        spdlog::error("{}: {} during {}\nFile = '{}', function = '{}', line = '{}'", e.get_code(), e.what(), e.get_sql(),
                                                __FILE__, __FUNCTION__, __LINE__);
                                                
    }
}



std::vector<User> Table_Users::get_all_users() const
{
    std::vector<User> users_vec;
    

    try
    {
        *db_ << "SELECT * FROM USERS;" >> [&](int id, std::string login, std::string password, size_t last_edit_time,
                                              bool archived) 
        {
            User temp(id, login, password, last_edit_time, archived);
            users_vec.push_back(temp);
        };
    }
    catch (const sqlite::sqlite_exception& e) {

        spdlog::error("{}: {} during {}\nFile = '{}', function = '{}', line = '{}'", e.get_code(), e.what(), e.get_sql(),
                                                __FILE__, __FUNCTION__, __LINE__);
                                                
    }

    spdlog::info("Vector of all users returned");
    return users_vec;
}



bool Table_Users::verify_user(const User& user) const
{
    try
    {
        std::vector<User> users_vec;
        *db_ << u"SELECT * FROM USERS WHERE LOGIN=? and PASSWORD=?" << user.get_login() << sha256(user.get_password()) >> 
                        [&](int id, std::string login, std::string password, size_t last_edit_time, bool archived)
                {
                    User temp(id, login, password, last_edit_time, archived);
                    users_vec.push_back(temp);
                };

        if (users_vec.size() > 1)
        {
            spdlog::critical("Found more than 1 user by login");
            return true;
        }
        if (users_vec.size() == 1)
        {   
            spdlog::info("User '{}' verified", user.get_login());
            return true;
        }
    }
    catch (const sqlite::sqlite_exception& e) {
        spdlog::error("{}: {} during {}\nFile = '{}', function = '{}', line = '{}'", e.get_code(), e.what(), e.get_sql(),
                                                __FILE__, __FUNCTION__, __LINE__);
    }

    spdlog::warn("No users found by name = '{}'", user.get_login());
    return false;
}



void Table_Users::update_user_password(const User& user) const
{
    try
    {
        *db_ << u"UPDATE USERS SET PASSWORD=? WHERE LOGIN=?" << sha256(user.get_password()) << user.get_login();

        spdlog::info("Password for user '{}' updated", user.get_login());
    }
    catch (const sqlite::sqlite_exception& e) {

        spdlog::error("{}: {} during {}\nFile = '{}', function = '{}', line = '{}'", e.get_code(), e.what(), e.get_sql(),
                                                __FILE__, __FUNCTION__, __LINE__);
                                                
    }
}



void Table_Users::update_user_password(const std::string& user_name, const std::string& new_password) const
{
    User temp(user_name, new_password);
    update_user_password(temp);

    spdlog::info("Password for user '{}' updated", user_name);
}



std::string Table_Users::sha256(const std::string& data) const
{
    return data + "psw";
}



time_t Table_Users::get_last_edit_time_users() const
{
    long long last_edit_time = 0;

    try
    {
        *db_ << u"SELECT MAX(LAST_EDIT_TIME) FROM USERS" >> last_edit_time;

        spdlog::info("Last edit time of users = {}", last_edit_time);
    }
    catch (const sqlite::sqlite_exception& e) {

        spdlog::error("{}: {} during {}\nFile = '{}', function = '{}', line = '{}'", e.get_code(), e.what(), e.get_sql(),
                                                __FILE__, __FUNCTION__, __LINE__);

    }

    return last_edit_time;
}



std::vector<User> Table_Users::get_users_to_sync(time_t time) const
{
    std::vector<User> users_vec;

    try
    {
        *db_ << u"SELECT * FROM USERS WHERE LAST_EDIT_TIME >= ?;" << time >> 
                [&](int id, std::string login, std::string password, size_t last_edit_time, bool archived) 
        {
            User temp(id, login, password, last_edit_time, archived);
            users_vec.push_back(temp);
        };

        spdlog::info("'{}' users found for sync", users_vec.size());
    }
    catch (const sqlite::sqlite_exception& e) {

        spdlog::error("{}: {} during {}\nFile = '{}', function = '{}', line = '{}'", e.get_code(), e.what(), e.get_sql(),
                                                __FILE__, __FUNCTION__, __LINE__);

    }

    return users_vec;
}