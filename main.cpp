#include <limits>
int main()
{
	int** board = create_board();
	print_board(board);
	bool game_over = false;
	int turn = rand() % 1 + 1;
	int ro = 0, co = 0;
	while (game_over == false)
	{

		if (turn == PLAYER)
		{
			cout << "please enter the positionto play (row,column):" << endl;
			cin >> ro;
			cin >> co;
			if (is_valid_location(board, co) == 1)
			{
				int row = get_next_open_row(board, co);
				drop_piece(board, row, co, PLAYER_PIECE);
			
			/*if (winning_move(board, PLAYER_PIECE))
			{
				game_over = true;
			}*/
			turn += 1;
			turn = turn % 2;

			print_board(board);
		    }
	    }
		if ((turn == AI) && (game_over==false))
		{
			//col, minimax_score = minimax(board, 3, -(std::numeric_limits<double>::infinity()), (std::numeric_limits<double>::infinity()), true);
			if (is_valid_location(board, col)) 
			{

				int row = get_next_open_row(board, col);
				drop_piece(board, row, col, AI_PIECE);

				/*if (winning_move(board, AI_PIECE))
				{

					game_over = true;
				}*/
				print_board(board);


				turn += 1;
				turn = turn % 2;
			}
					
		}
				  
	}


	return 0;
}