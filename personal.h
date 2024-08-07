#pragma once
#include <iostream>
#include <pqxx/pqxx>
#include <string>

class PersonalDB
{

public:
    static void CreateTables(pqxx::connection& conn);
    static void AddClient(pqxx::connection& conn);
    static void AddNumber(pqxx::connection& conn);
    static void ChangeInfo(pqxx::connection& conn);
    static void DeleteNumber(pqxx::connection& conn);
    static void DeleteClient(pqxx::connection& conn);
    static void SearchClient(pqxx::connection& conn);
};