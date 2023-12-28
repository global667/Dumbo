// CMakeProject1.cpp: Definiert den Einstiegspunkt für die Anwendung.
//
#include <iostream>

// TODO: Verweisen Sie hier auf zusätzliche Header, die Ihr Programm erfordert.
#include "position.h"
#include "uci.h"

int main() {

    UCIChessEngine chessEngine;
    chessEngine.run();

    return 0;
}
