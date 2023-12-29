#pragma once

#include <cstring>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "position.h"

class UCIChessEngine {
public:
	UCIChessEngine() {
		std::cout << "UCIChessEngine erstellt." << std::endl;
		position.initBoard();
        players_color = Color::Black;
    }

    void run()
    {
        std::string input = "";
        while (true) {
            std::getline(std::cin, input, '\n');
            if (input == "uci") {
                handleUCICommand();
            } else if (input == "isready") {
                handleIsReadyCommand();
            } else if (input == "ucinewgame") {
                handleUcinewgameCommand();
            } else if (input.substr(0, 24) == "position startpos moves ") {
                if (input.length() <= 24)
                    continue;
                handleStartposCommand(input.substr(24, input.length()));

            } else if (input == "go") {
                handleGoCommand();
                print_board();

            } else if (input == "quit") {
                handleQuitCommand();
                break;
            } else {
                std::cout << "Unbekannter Befehl: " << input << std::endl;
            }
        }
    }

private:
    void handleUCICommand()
    {
        std::cout << "id name Dumbo" << std::endl;
        std::cout << "id author global667" << std::endl;
        std::cout << "uciok" << std::endl;
    }

    void handleIsReadyCommand() { std::cout << "readyok" << std::endl; }

    void handleUcinewgameCommand() { position.initBoard(); }

    void handleStartposCommand(std::string input)
    {
        position.initBoard();
        std::string s;
        std::istringstream i(input);
        int m = 0;

        while (i >> s) {
            std::string from = s.substr(0, 2);
            std::string to = s.substr(2, 3);
            std::cout << "info " << from << " " << to << std::endl;
            make_move(from, to);
            m++;
        }
        if (m % 2 == 1)
            players_color = Color::Black;
        else
            players_color = Color::Red;
    }

    void handleGoCommand()
    {
        std::string bestmove = make_bestmove();

        std::cout << "bestmove " << bestmove << std::endl;
    }

    void handleQuitCommand()
    {
        std::cout << "Bye!" << std::endl;
    }

    Position position;
    Color players_color;

    void make_move(std::string from, std::string to)
    {
        int file_from = from[0] - 'a';
        int rank_from = from[1] - '0';
        int file_to = to[0] - 'a';
        int rank_to = to[1] - '0';
        std::cout << "info " << file_from << rank_from << file_to << rank_to << std::endl;
        position.add_piece(position.board[rank_from][file_from].piece, file_to, rank_to);
        position.board[rank_from][file_from].piece = nullptr;

    }

    std::string make_bestmove()
    {
        // Generate moves for a specific piece

        std::vector<std::pair<int, int>> moves = {};
        std::vector<std::pair<int, int>> all_moves_to = {};
        std::vector<std::pair<int, int>> all_moves_from = {};

        srand(time(NULL));

        // find all possible moves
        for (std::size_t file = 0; file < 9; file++) {
            for (std::size_t rank = 0; rank < 10; rank++) {
                if (position.board[rank][file].piece != nullptr)
                    if (position.board[rank][file].piece->color == players_color
                        && position.is_inside_board(file, rank)) {
                        PieceType piece_type = position.board[rank][file].piece->piece_type;
                        moves = position.generate_piece_moves(piece_type, file, rank);
                        for (auto m : moves) {
                            all_moves_from.push_back(std::make_pair(file, rank));
                            all_moves_to.push_back(m);
                        }
                    }
            }
        }

        // Check and checkmate rules
        if (position.is_check(players_color)) {
            std::cout << "Check!" << std::endl;
        }

        if (position.is_checkmate(players_color)) {
            std::cout << "Checkmate!" << std::endl;
        }


        // select one valid move randomly
        int move = rand() % all_moves_from.size();

        char f1 = all_moves_from[move].first;
        f1 = f1 + 'a';
        char t1 = all_moves_to[move].first;
        t1 = t1 + 'a';

        std::string bestmove = f1 + std::to_string(all_moves_from[move].second) + t1
                               + std::to_string(all_moves_to[move].second);

        return bestmove;
    }

    void print_board()
    {
        for (std::size_t file = 0; file < 10; file++) {
            for (std::size_t rank = 0; rank < 9; rank++) {
                if (position.board[file][rank].piece == nullptr)
                    std::cout << " 0 ";
                else
                    switch (position.board[file][rank].piece->piece_type) {
                    case PieceType::Advisor:
                        std::cout << " A ";
                        break;
                    case PieceType::General:
                        std::cout << " G ";
                        break;
                    case PieceType::Cannon:
                        std::cout << " C ";
                        break;
                    case PieceType::Chariot:
                        std::cout << " R ";
                        break;
                    case PieceType::Elephant:
                        std::cout << " E ";
                        break;
                    case PieceType::Horse:
                        std::cout << " H ";
                        break;
                    case PieceType::Soldier:
                        std::cout << " S ";
                        break;
                    default:
                        break;
                    }
            }
            std::cout << std::endl;
        }            
        std::cout << std::endl;
    }
};
