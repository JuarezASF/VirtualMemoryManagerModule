//
// Created by jasf on 5/14/16.
//

#ifndef VIRTUALMEMORYMANAGER_CONFIG_PARSER_H
#define VIRTUALMEMORYMANAGER_CONFIG_PARSER_H

#include <unordered_map>
#include <vector>
#include <iostream>
#include <string>

using namespace std;

class ConfigParser {
private:

    static unordered_map<string, string> config_map;

public:

    static void loadConfig(string filename);

    static int getInt(string name);
    static long getLong(string name);
    static string getString(string name);
    static double getDouble(string name);


    static void removeSpaces(string &name);
};


#endif //VIRTUALMEMORYMANAGER_CONFIG_PARSER_H
