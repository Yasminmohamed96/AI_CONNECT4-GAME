int main()
{
	int** board = create_board();
	drop_piece(board, 0, 2, 1);
	drop_piece(board, 1, 2, 1);
	drop_piece(board, 2, 2, 1);
	drop_piece(board, 3, 2, 1);

	
	//btraga3 1 or 0 ma3a eny 3amlaha bool
	bool w = is_valid_location(board, 2);
	int y = get_next_open_row(board, 2);
	print_board(board);
	cout << w <<" "<<y<<endl;
	int score = score_position(board, 1);
	cout << score;
	return 0;
}
