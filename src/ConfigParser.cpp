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
    string val = (string) config_map[name];
    if (val.find("0x", 0) >= 0)
        return std::stoi(val.c_str(), 0, 16);
    else if (val[0] == '0')
        return std::stoi(val.c_str(), 0, 8);
    else
        return std::stoi(val.c_str(), 0, 10);
}

long ConfigParser::getLong(string name) {
    if (config_map.find(name) == config_map.end()) {
        cerr << " CANNOT FIND CONFIG PARAMATER " << name << endl;
        return -1;
    }
    string val = config_map[name];
    if (val.find("0x", 0) >= 0)
        return std::stol(((string) config_map[name]).c_str(), 0, 16);
    else if (val[0] == '0')
        return std::stol(((string) config_map[name]).c_str(), 0, 8);
    else
        return std::stol(((string) config_map[name]).c_str(), 0, 10);
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
    return stod(((string) config_map[name]).c_str());
}
