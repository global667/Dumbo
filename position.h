#pragma once
#include <iostream>
#include <vector>

enum class Color { Red, Black };

enum class PieceType {
	General,
	Advisor,
	Elephant,
	Horse,
	Chariot,
	Cannon,
	Soldier
};

struct Piece {
	PieceType piece_type;
	Color color;
};

class Position {
public:
	Position() {
		for (int rank = 0; rank < 10; ++rank) {
			for (int file = 0; file < 9; ++file) {
				board[rank][file].piece = nullptr;
			}
		}
	}

	void initBoard()
	{
		for (int rank = 0; rank < 10; ++rank) {
			for (int file = 0; file < 9; ++file) {
				board[rank][file].piece = nullptr;
			}
		}

		// Adding red pieces
		add_piece(new Piece{ PieceType::Chariot, Color::Red }, 0, 0);
		add_piece(new Piece{ PieceType::Chariot, Color::Red }, 8, 0);
		add_piece(new Piece{ PieceType::Horse, Color::Red }, 1, 0);
		add_piece(new Piece{ PieceType::Horse, Color::Red }, 7, 0);
		add_piece(new Piece{ PieceType::Elephant, Color::Red }, 2, 0);
		add_piece(new Piece{ PieceType::Elephant, Color::Red }, 6, 0);
		add_piece(new Piece{ PieceType::Advisor, Color::Red }, 3, 0);
		add_piece(new Piece{ PieceType::Advisor, Color::Red }, 5, 0);
		add_piece(new Piece{ PieceType::General, Color::Red }, 4, 0);
		add_piece(new Piece{ PieceType::Cannon, Color::Red }, 1, 2);
		add_piece(new Piece{ PieceType::Cannon, Color::Red }, 7, 2);
		for (int i = 0; i < 9; i = i + 2) {
			add_piece(new Piece{ PieceType::Soldier, Color::Red }, i, 3);
		}

		// Adding black pieces
		add_piece(new Piece{ PieceType::Chariot, Color::Black }, 0, 9);
		add_piece(new Piece{ PieceType::Chariot, Color::Black }, 8, 9);
		add_piece(new Piece{ PieceType::Horse, Color::Black }, 1, 9);
		add_piece(new Piece{ PieceType::Horse, Color::Black }, 7, 9);
		add_piece(new Piece{ PieceType::Elephant, Color::Black }, 2, 9);
		add_piece(new Piece{ PieceType::Elephant, Color::Black }, 6, 9);
		add_piece(new Piece{ PieceType::Advisor, Color::Black }, 3, 9);
		add_piece(new Piece{ PieceType::Advisor, Color::Black }, 5, 9);
		add_piece(new Piece{ PieceType::General, Color::Black }, 4, 9);
		add_piece(new Piece{ PieceType::Cannon, Color::Black }, 1, 7);
		add_piece(new Piece{ PieceType::Cannon, Color::Black }, 7, 7);
		for (int i = 0; i < 9; i = i + 2) {
			add_piece(new Piece{ PieceType::Soldier, Color::Black }, i, 6);
		}
	}

	bool is_inside_board(int file, int rank) const {
		return file >= 0 && file < 9 && rank >= 0 && rank < 10;
	}

	void add_piece(Piece* piece, int file, int rank) {
		if (is_inside_board(file, rank)) {
			board[rank][file].piece = piece;
		}
	}

	std::vector<std::pair<int, int>> generate_piece_moves(
		PieceType piece_type, int file, int rank) const {
		std::vector<std::pair<int, int>> moves;

		switch (piece_type) {
		case PieceType::General:
			generate_general_moves(file, rank, moves);
			break;
		case PieceType::Advisor:
			generate_advisor_moves(file, rank, moves);
			break;
		case PieceType::Elephant:
			generate_elephant_moves(file, rank, moves);
			break;
		case PieceType::Horse:
			generate_horse_moves(file, rank, moves);
			break;
		case PieceType::Chariot:
			generate_chariot_moves(file, rank, moves);
			break;
		case PieceType::Cannon:
			generate_cannon_moves(file, rank, moves);
			break;
		case PieceType::Soldier:
			generate_soldier_moves(file, rank, moves);
			break;
		default:
			break;
		}
		return moves;
	}


	bool is_check(Color color) const {
		int general_file = -1;
		int general_rank = -1;

		// Find the position of the general
		for (int rank = 0; rank < 10; ++rank) {
			for (int file = 0; file < 9; ++file) {
				Piece* piece = board[rank][file].piece;
				if (piece != nullptr && piece->piece_type == PieceType::General && piece->color == color) {
					general_file = file;
					general_rank = rank;
					break;
				}
			}
			if (general_file != -1 && general_rank != -1) {
				break;
			}
		}

		// Check if the opponent's pieces can attack the general
		for (int rank = 0; rank < 10; ++rank) {
			for (int file = 0; file < 9; ++file) {
				Piece* piece = board[rank][file].piece;
				if (piece != nullptr && piece->color != color) {
					std::vector<std::pair<int, int>> moves = generate_piece_moves(piece->piece_type, file, rank);
					for (const auto& move : moves) {
						if (move.first == general_file && move.second == general_rank) {
							return true;
						}
					}
				}
			}
		}

		return false;
	}

	bool is_checkmate(Color color) {
		// Check if the general is in check
		if (!is_check(color)) {
			return false;
		}

		// Try moving each of the player's pieces to see if the check can be escaped
		for (int rank = 0; rank < 10; ++rank) {
			for (int file = 0; file < 9; ++file) {
				Piece* piece = board[rank][file].piece;
				if (piece != nullptr && piece->color == color) {
					std::vector<std::pair<int, int>> moves = generate_piece_moves(piece->piece_type, file, rank);
					for (const auto& move : moves) {
						// Try the move
						Piece* captured_piece = board[move.second][move.first].piece;
						board[move.second][move.first].piece = piece;
						board[rank][file].piece = nullptr;

						// Check if the check is still present
						bool is_still_check = is_check(color);

						// Undo the move
						board[rank][file].piece = piece;
						board[move.second][move.first].piece = captured_piece;

						if (!is_still_check) {
							// The check can be escaped
							return false;
						}
					}
				}
			}
		}

		// All possible moves have been checked and the general is still in check
		// Checkmate!
		return true;
	}

	bool is_evil_glare(Color color) const {
		int general_file = -1;
		int general_rank = -1;
		int opp_general_file = -1;
		int opp_general_rank = -1;
		Piece* general = nullptr;
		Piece* opp_general = nullptr;

		Color opp_color;

		// Find the position of the general
		for (int rank = 0; rank < 10; ++rank) {
			for (int file = 0; file < 9; ++file) {
				Piece* piece = board[rank][file].piece;
				if (piece != nullptr && piece->piece_type == PieceType::General && piece->color == color) {
					general_file = file;
					general_rank = rank;
					break;
				}
			}
			if (general_file != -1 && general_rank != -1) {
				break;
			}

		}
		if (general_file != -1 && general_rank != -1) {
			general = board[general_rank][general_file].piece;
		}
		else
			return true;


		if (color == Color::Red)
			opp_color = Color::Black;
		else
			opp_color = Color::Red;

		// Find the position of the opposite general
		for (int rank = 0; rank < 10; ++rank) {
			for (int file = 0; file < 9; ++file) {
				Piece* piece = board[rank][file].piece;
				if (piece != nullptr && piece->piece_type == PieceType::General && piece->color == opp_color) {
					opp_general_file = file;
					opp_general_rank = rank;
					break;
				}
			}
			if (opp_general_file != -1 && opp_general_rank != -1) {
				break;
			}

		}
		if (opp_general_file != -1 && opp_general_rank != -1) {
			opp_general = board[opp_general_rank][opp_general_file].piece;
		}
		else
			return true;

		if (opp_general_rank == general_rank)
			for (size_t i = general_rank + 1; i < opp_general_rank; i++)
				if (board[i][general_file].piece != nullptr)
					return false;

		return true;
	}

	struct Square {
		Piece* piece;
	};
	Square board[10][9];

private:
	void add_move(int file, int rank, std::vector<std::pair<int, int>>& moves) const {
		if (is_inside_board(file, rank)) {
			Piece* piece = board[rank][file].piece;
			if (piece == nullptr || piece->color != board[rank][file].piece->color) {
				moves.push_back(std::make_pair(file, rank));
			}
		}
	}

	void generate_general_moves(int file, int rank, std::vector<std::pair<int, int>>& moves) const {
		int directions[4][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
		for (const auto& dir : directions) {
			add_move(file + dir[0], rank + dir[1], moves);
		}
	}

	void generate_advisor_moves(int file, int rank, std::vector<std::pair<int, int>>& moves) const {
		int directions[4][2] = { {-1, -1}, {1, 1}, {1, -1}, {-1, 1} };
		for (const auto& dir : directions) {
			Piece* piece = board[rank][file].piece;
			if ((file + dir[0] >= 0 && file + dir[0] <= 2) && rank + dir[1] >= 3 && rank + dir[1] <= 5 && piece->color == Color::Red)
				add_move(file + dir[0], rank + dir[1], moves);
			if ((file + dir[0] >= 7 && file + dir[0] <= 9) && rank + dir[1] >= 3 && rank + dir[1] <= 5 && piece->color == Color::Black)
				add_move(file + dir[0], rank + dir[1], moves);
		}
	}

	void generate_elephant_moves(int file, int rank, std::vector<std::pair<int, int>>& moves) const {
		int directions[4][2] = { {-2, -2}, {2, 2}, {2, -2}, {-2, 2} };
		int between_directions[4][2] = { {-1, -1}, {1, 1}, {1, -1}, {-1, 1} };
		for (int i = 0; i < 4; ++i) {
			int new_file = file + directions[i][0];
			int new_rank = rank + directions[i][1];
			int between_file = file + between_directions[i][0];
			int between_rank = rank + between_directions[i][1];
			if (is_inside_board(new_file, new_rank) && board[new_rank][new_file].piece == nullptr &&
				is_inside_board(between_file, between_rank) && board[between_rank][between_file].piece == nullptr) {
				add_move(new_file, new_rank, moves);
			}
		}
	}

	void generate_horse_moves(int file, int rank, std::vector<std::pair<int, int>>& moves) const {
		int directions[8][2] = { {-2, -1}, {-2, 1}, {2, -1}, {2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2} };
		for (const auto& dir : directions) {
			int new_file = file + dir[0];
			int new_rank = rank + dir[1];
			int between_file = file + dir[0] / 2;
			int between_rank = rank + dir[1] / 2;
			if (is_inside_board(new_file, new_rank) &&
				(board[new_rank][new_file].piece == nullptr ||
					board[new_rank][new_file].piece->color != board[rank][file].piece->color)) {
				if (dir[0] == -2 || dir[0] == 2) {
					if (is_inside_board(between_file, rank) && board[rank][between_file].piece == nullptr) {
						add_move(new_file, new_rank, moves);
					}
				}
				else {
					if (is_inside_board(file, between_rank) && board[between_rank][file].piece == nullptr) {
						add_move(new_file, new_rank, moves);
					}
				}
			}
		}
	}

	void generate_chariot_moves(int file, int rank, std::vector<std::pair<int, int>>& moves) const {
		int directions[4][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
		for (const auto& dir : directions) {
			int new_file = file + dir[0];
			int new_rank = rank + dir[1];
			while (is_inside_board(new_file, new_rank)) {
				Piece* piece = board[new_rank][new_file].piece;
				if (piece == nullptr) {
					add_move(new_file, new_rank, moves);
				}
				else {
					if (piece->color != board[rank][file].piece->color) {
						add_move(new_file, new_rank, moves);
					}
					break;
				}
				new_file += dir[0];
				new_rank += dir[1];
			}
		}
	}

	void generate_cannon_moves(int file, int rank, std::vector<std::pair<int, int>>& moves) const {
		int directions[4][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
		for (const auto& dir : directions) {
			int new_file = file + dir[0];
			int new_rank = rank + dir[1];
			bool found_piece = false;
			while (is_inside_board(new_file, new_rank)) {
				Piece* piece = board[new_rank][new_file].piece;
				if (piece == nullptr) {
					if (!found_piece) {
						add_move(new_file, new_rank, moves);
					}
				}
				else {
					if (!found_piece) {
						found_piece = true;
					}
					else {
						if (piece->color != board[rank][file].piece->color) {
							add_move(new_file, new_rank, moves);
						}
						break;
					}
				}
				new_file += dir[0];
				new_rank += dir[1];
			}
		}
	}

	void generate_soldier_moves(int file, int rank, std::vector<std::pair<int, int>>& moves) const {
		int directions[2][2];
		if (board[rank][file].piece->color == Color::Red) {
			directions[0][0] = 0;
			directions[0][1] = 1;
			if (rank > 5) {
				directions[1][0] = -1;
				directions[1][1] = 0;
			}
		}
		else {
			directions[0][0] = 0;
			directions[0][1] = -1;

			if (rank < 5) {
				directions[1][0] = -1;
				directions[1][1] = 0;
			}
		}

		for (const auto& dir : directions) {
			int new_file = file + dir[0];
			int new_rank = rank + dir[1];
			if (is_inside_board(new_file, new_rank)) {
				Piece* piece = board[new_rank][new_file].piece;
				if (piece == nullptr || piece->color != board[rank][file].piece->color) {
					add_move(new_file, new_rank, moves);
				}
			}
		}
	}
};
