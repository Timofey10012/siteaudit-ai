#include "server/server.h"

#include <iostream>

int main() {
    setlocale(LC_ALL, ".UTF-8");

    server::runServer();

    return 0;
}