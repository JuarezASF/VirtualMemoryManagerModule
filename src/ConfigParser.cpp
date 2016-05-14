//
// Created by jasf on 5/14/16.
//

#include "ConfigParser.h"
#include <fstream>
#include <algorithm>

unordered_map<string, string> ConfigParser::config_map;

void ConfigParser::loadConfig(string filename) {

    ifstream is(filename.c_str());

    if (!is.is_open()) {
        cerr << "CANNOT OPEN FILE " << filename << endl;
        exit(0);
    }

    string line;
    string name, val;

    config_map.clear();

    while (std::getline(is, line)) {
        if (line.empty())
            continue;

        name = line.substr(0, line.find(":"));
        val = line.substr(line.find(":") + 1, line.length());

        removeSpaces(name);
        removeSpaces(val);


        config_map[name] = val;
    }


}

void ConfigParser::removeSpaces(string &name) { name.erase(std::remove(name.begin(), name.end(), ' '), name.end()); }

int ConfigParser::getInt(string name) {
    if (config_map.find(name) == config_map.end()) {
        cerr << " CANNOT FIND CONFIG PARAMATER " << name << endl;
        return -1;
    }
    return stoi(((string)config_map[name]).c_str());
}

long ConfigParser::getLong(string name) {
    if (config_map.find(name) == config_map.end()) {
        cerr << " CANNOT FIND CONFIG PARAMATER " << name << endl;
        return -1;
    }
    return stol(((string)config_map[name]).c_str());
}

string ConfigParser::getString(string name) {
    if (config_map.find(name) == config_map.end()) {
        cerr << " CANNOT FIND CONFIG PARAMATER " << name << endl;
        return "";
    }
    return config_map[name];
}

double ConfigParser::getDouble(string name) {
    if (config_map.find(name) == config_map.end()) {
        cerr << " CANNOT FIND CONFIG PARAMATER " << name << endl;
        return -1;
    }
    return stod(((string)config_map[name]).c_str());
}
