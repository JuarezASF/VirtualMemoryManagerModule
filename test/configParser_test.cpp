//
// Created by jasf on 5/14/16.
//

#include "../src/ConfigParser.h"

int main() {

    ConfigParser::loadConfig("config.txt");

    cout << ConfigParser::getString("s_val") << endl;
    cout << ConfigParser::getInt("i_val") << endl;
    cout << ConfigParser::getDouble("d_val") << endl;
    cout << ConfigParser::getLong("l_val") << endl;

    return 0;


}

