#pragma once
#include <stddef.h>
#include <pqxx/pqxx>
#include "../NewBase/NewMap.h"


//using namespace Aws;
using namespace std;

using namespace pqxx;

connection init_connection() {
    connection c("dbname = Coup user = postgres password = Qpka971v6 hostaddr = 127.0.0.1 port = 5432");
    if (c.is_open()) {
        cout << "Opened database successfully: " << c.dbname() << endl;
    }
    else {
        cout << "Can't open database" << endl;
    }
    return c;
}


int transfer_MultiStorage(int pkey, NewMap* pdoc) {



    return 0;
}
