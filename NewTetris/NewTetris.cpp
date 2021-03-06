#include "stdafx.h"
#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <cstdint>
#include <ctime>

using namespace std;


enum piece {I, O, S, Z, J, L};
enum rotation { left, top, right, bottom };


void try_place_piece(int p);
void get_pieces();
int get_next_permutation (int items[], int itemsLength);
int compare(const void * x1, const void * x2);
bool place_to_field(piece p, int rot, int x, int y, char target[][8]);
int check();
int evaluate();
void clean();
void draw();
void draw_1();

char field[10][8];
char field_res[10][8];
char field_final[10][8];
char pieces_char[6];
int pieces[6];

int main()
{
	
	for (uint8_t i = 0; i < 10; i++)
		for (uint8_t j = 0; j < 8; j++)
		{
			field[i][j] = ' ';
			field_res[i][j] = ' ';
			field_final[i][j] = ' ';
		}

	get_pieces();											//filling the int array 'pieces'

	int t = clock();

	get_next_permutation(pieces, (int)(sizeof(pieces) / sizeof(int)));
	cin.get();

	cout << "Execution time: " << (clock() - t) << " ms" << endl;

	cin.get();

    return 0;
}


void get_pieces()
{
	char first;
	char second;
	char third;
	char fourth;
	char fifth;
	char sixth;

	cout << "		   FIGURES NOTATION: " << endl << endl << "	I	O	S	Z	J	L" << endl << endl << "Notice: R uses as reverse G piece" << endl << "Input 6 pieces separated by space: ";
	cin >> first >> second >> third >> fourth >> fifth >> sixth;

	pieces_char[0] = toupper(first);
	pieces_char[1] = toupper(second);
	pieces_char[2] = toupper(third);
	pieces_char[3] = toupper(fourth);
	pieces_char[4] = toupper(fifth);
	pieces_char[5] = toupper(sixth);

	for (uint8_t i = 0; i < 6; i++)
	{
		switch (pieces_char[i])
		{
		case 'S':
			pieces[i] = piece::S;
			break;

		case 'Z':
			pieces[i] = piece::Z;
			break;

		case 'I':
			pieces[i] = piece::I;
			break;

		case 'O':
			pieces[i] = piece::O;
			break;

		case 'J':
			pieces[i] = piece::J;
			break;

		case 'L':
			pieces[i] = piece::L;
			break;
		}
	}
}

void try_place_piece(int p)
{
	uint8_t free_cells = 8;
	uint8_t free_cells_count = 0;

	uint8_t second_free_cells = 8;
	uint8_t second_free_cells_count = 0;
	uint8_t prev_free_cells_count = 0;
	uint8_t prev2_free_cells_count = 0;


	uint8_t best_r = 0;
	uint8_t best_x = 0;
	uint8_t best_y = 0;

	uint8_t max_nearby_free_cells = 0;
	uint8_t current_max_nearby_free_cells = 0;
	uint8_t nearby_free_cells_count = 0;
	bool flag1 = false;
	bool flag2 = false;

	for (int i = -2; i < 0; i++)
		for (uint8_t j = 0; j < 8; j++)
		{
			field[i][j] = '-';
		}

	for (uint8_t i = 0; i < 10; i++)
		for (uint8_t j = 0; j < 8; j++)
			field[i][j] = field_res[i][j];
	
	for (uint8_t d_r = 0; d_r < 3; d_r++)
	{
		for (uint8_t d_x = 0; d_x < 8; d_x++)
		{
			for (uint8_t r = 0 + d_r; r < 4; r++)
			{
				for (int y = -2; y < 8; y++)
				{
					for (uint8_t x = 0 + d_x; x < 8; x++)
					{
						if ((y >= 0) && (place_to_field(piece(p), r, x, y, field)))
						{
							/////////////////////////////////////////////
							//draw();
							////////////////////////////////////////////

							for (uint8_t j = 0; j < 8; j++)
							{
								if (field[y][j] == ' ') free_cells_count++;
								if (field[y + 1][j] == ' ') second_free_cells_count++;
								if (field[y - 1][j] == ' ') prev_free_cells_count++;
							}

							if (free_cells >= free_cells_count)
							{
								free_cells = free_cells_count;

								for (uint8_t j = 0; j < 8; j++)
								{
									if (field[y][j] == ' ')
									{
										nearby_free_cells_count++;
									}
									else
									{
										if (current_max_nearby_free_cells < nearby_free_cells_count) current_max_nearby_free_cells = nearby_free_cells_count;
										nearby_free_cells_count = 0;
									}
								}
								if (current_max_nearby_free_cells < nearby_free_cells_count) current_max_nearby_free_cells = nearby_free_cells_count;

								if (current_max_nearby_free_cells >= max_nearby_free_cells)
								{
									max_nearby_free_cells = current_max_nearby_free_cells;
									best_r = r;
									best_x = x;
									best_y = y;
								}
							}
							else if (free_cells > second_free_cells_count)
							{
								free_cells = second_free_cells_count;

								for (uint8_t j = 0; j < 8; j++)
								{
									if (field[y + 1][j] == ' ')
									{
										nearby_free_cells_count++;
									}
									else
									{
										if (current_max_nearby_free_cells < nearby_free_cells_count) current_max_nearby_free_cells = nearby_free_cells_count;
										nearby_free_cells_count = 0;
									}
								}
								if (current_max_nearby_free_cells < nearby_free_cells_count) current_max_nearby_free_cells = nearby_free_cells_count;

								if (current_max_nearby_free_cells >= max_nearby_free_cells)
								{
									max_nearby_free_cells = current_max_nearby_free_cells;
									best_r = r;
									best_x = x;
									best_y = y;
								}
							}
							

							
							flag1 = true;
							break;
						}
						else clean();
					}
					if (flag1 == true)
					{
						flag2 = true;
						break;
					}
				}
				if (flag2 == true)
				{
					flag1 = false;
					flag2 = false;
					break;
				}
			}
			free_cells_count = 0;
			second_free_cells_count = 0;
			prev_free_cells_count = 0;
			prev2_free_cells_count = 0;
			nearby_free_cells_count = 0;
			current_max_nearby_free_cells = 0;
			clean();
		}
		free_cells_count = 0;
		second_free_cells_count = 0;
		prev_free_cells_count = 0;
		prev2_free_cells_count = 0;
		nearby_free_cells_count = 0;
		current_max_nearby_free_cells = 0;
		max_nearby_free_cells = 0;
		clean();
	}
	clean();

	place_to_field(piece(p), best_r, best_x, best_y, field_res);
	//draw_1();

}

bool place_to_field(piece p, int rot, int x, int y, char target[][8])
{
	bool legal;
	switch (p)
	{
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////					        				/////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////					OOOOOO					/////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////					OOOOOO					/////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////					OOOOOO					/////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////					OOOOOO					/////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////					OOOOOO					/////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////					OOOOOO					/////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////					OOOOOO					/////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////					OOOOOO					/////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////					OOOOOO					/////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////					OOOOOO					/////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////					OOOOOO					/////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////					OOOOOO					/////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////					        				/////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case I:

		if (rot == rotation::left)
		{
			if ((target[y][x] == ' ') && (target[y][x + 1] == ' ') && (target[y][x + 2] == ' ') && (target[y][x + 3] == ' ') && (x < 5))
			{
				legal = true;
			}
			else return false;
		}
		else if (rot == top)
		{
			if ((target[y][x] == ' ') && (target[y + 1][x] == ' ') && (target[y + 2][x] == ' ') && (target[y + 3][x] == ' '))
			{
				legal = true;
			}
			else return false;
		}
		else return false;

		if (legal)
		{
			if (rot == top)
			{
				target[y][x] = 'I';
				target[y + 1][x] = 'I';
				target[y + 2][x] = 'I';
				target[y + 3][x] = 'I';
				return true;
			}
			else if (rot == rotation::left)
			{
				target[y][x] = 'I';
				target[y][x + 1] = 'I';
				target[y][x + 2] = 'I';
				target[y][x + 3] = 'I';
				return true;
			}
			else return false;
		}

		break;

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////					        				    /////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////					OOOOOOOOOOOO				/////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////					OOOOOOOOOOOO				////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////					OOOOOOOOOOOO				/////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////					OOOOOOOOOOOO				////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////					OOOOOOOOOOOO				/////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////					OOOOOOOOOOOO				/////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////					        				    /////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	case O:
		if ((target[y][x] == ' ') && (target[y + 1][x] == ' ') && (target[y][x + 1] == ' ') && (target[y + 1][x + 1] == ' ') && (x < 7))
		{
			legal = true;
		}
		else return false;

		if (legal)
		{
			target[y][x] = 'O';
			target[y + 1][x] = 'O';
			target[y][x + 1] = 'O';
			target[y + 1][x + 1] = 'O';
			return true;
		}

		else return false;
		break;

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////					        							/////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////					       OOOOOOOOOOOOO				/////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////					       OOOOOOOOOOOOO				/////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////					       OOOOOOOOOOOOO				/////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////					OOOOOOOOOOOOO						/////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////					OOOOOOOOOOOOO						/////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////					OOOOOOOOOOOOO						/////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////					        							/////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	case S:

		if (rot == rotation::left)
		{
			if ((target[y][x] == ' ') && (target[y][x + 1] == ' ') && (target[y + 1][x + 1] == ' ') && (target[y + 1][x + 2] == ' ') && (x < 6))
			{
				legal = true;
			}
			else return false;
		}
		else if (rot == top)
		{
			if ((target[y][x] == ' ') && (target[y + 1][x] == ' ') && (target[y + 1][x - 1] == ' ') && (target[y + 2][x - 1] == ' ') && (x > 0) && (x < 8))
			{
				legal = true;
			}
			else return false;
		}
		else return false;

		if (legal)
		{
			if (rot == rotation::left)
			{
				target[y][x] = 'S';
				target[y][x + 1] = 'S';
				target[y + 1][x + 1] = 'S';
				target[y + 1][x + 2] = 'S';
				return true;
			}
			else if (rot == top)
			{
				target[y][x] = 'S';
				target[y + 1][x] = 'S';
				target[y + 1][x - 1] = 'S';
				target[y + 2][x - 1] = 'S';
				return true;
			}
			else return false;
		}

		break;

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////					        					    /////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////				  OOOOOOOOOOOOO						/////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////				  OOOOOOOOOOOOO						/////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////				  OOOOOOOOOOOOO						/////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////					     OOOOOOOOOOOOO				/////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////					     OOOOOOOOOOOOO				/////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////					     OOOOOOOOOOOOO				/////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////					        						/////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	case Z:

		if (rot == rotation::left)
		{
			if ((target[y][x] == ' ') && (target[y][x + 1] == ' ') && (target[y + 1][x] == ' ') && (target[y + 1][x - 1] == ' ') && (x < 7) && (x > 0))
			{
				legal = true;
			}
			else return false;
		}
		else if (rot == top)
		{
			if ((target[y][x] == ' ') && (target[y + 1][x] == ' ') && (target[y + 1][x + 1] == ' ') && (target[y + 2][x + 1] == ' ') && (x < 7))
			{
				legal = true;
			}
			else return false;
		}
		else return false;

		if (legal)
		{
			if (rot == rotation::left)
			{
				target[y][x] = 'Z';
				target[y][x + 1] = 'Z';
				target[y + 1][x] = 'Z';
				target[y + 1][x - 1] = 'Z';
				return true;
			}
			else if (rot == top)
			{
				target[y][x] = 'Z';
				target[y + 1][x] = 'Z';
				target[y + 1][x + 1] = 'Z';
				target[y + 2][x + 1] = 'Z';
				return true;
			}
			else return false;
		}

		break;

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////					        					    /////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////				  OOOOOOO			   				/////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////				  OOOOOOO		  					/////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////				  OOOOOOO		   					/////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////				  OOOOOOOOOOOOOOOOOOOOO				/////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////				  OOOOOOOOOOOOOOOOOOOOO				/////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////				  OOOOOOOOOOOOOOOOOOOOO				/////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////					        						/////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	case J:

		if (rot == rotation::left)
		{
			if ((target[y][x] == ' ') && (target[y][x + 1] == ' ') && (target[y][x + 2] == ' ') && (target[y + 1][x] == ' ') && (x < 6))
			{
				legal = true;
			}
			else return false;
		}
		else if (rot == top)
		{
			if ((target[y][x] == ' ') && (target[y + 1][x] == ' ') && (target[y + 2][x] == ' ') && (target[y + 2][x + 1] == ' ') && (x < 7))
			{
				legal = true;
			}
			else return false;
		}
		else if (rot == rotation::right)
		{
			if ((target[y][x] == ' ') && (target[y + 1][x] == ' ') && (target[y + 1][x - 1] == ' ') && (target[y + 1][x - 2] == ' ') && (x > 1))
			{
				legal = true;
			}
			else return false;
		}
		else if (rot == bottom)
		{
			if ((target[y][x] == ' ') && (target[y][x + 1] == ' ') && (target[y + 1][x + 1] == ' ') && (target[y + 2][x + 1] == ' ') && (x < 7))
			{
				legal = true;
			}
			else return false;
		}
		else return false;

		if (legal)
		{
			if (rot == rotation::left)
			{
				target[y][x] = 'J';
				target[y][x + 1] = 'J';
				target[y][x + 2] = 'J';
				target[y + 1][x] = 'J';
				return true;
			}
			else if (rot == top)
			{
				target[y][x] = 'J';
				target[y + 1][x] = 'J';
				target[y + 2][x] = 'J';
				target[y + 2][x + 1] = 'J';
				return true;
			}
			else if (rot == rotation::right)
			{
				target[y][x] = 'J';
				target[y + 1][x] = 'J';
				target[y + 1][x - 1] = 'J';
				target[y + 1][x - 2] = 'J';
				return true;
			}
			else if (rot == bottom)
			{
				target[y][x] = 'J';
				target[y][x + 1] = 'J';
				target[y + 1][x + 1] = 'J';
				target[y + 2][x + 1] = 'J';
				return true;
			}
			else return false;
		}

		break;

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////					        					    /////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////								OOOOOOO			   	/////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////								OOOOOOO		  		/////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////								OOOOOOO		   		/////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////				  OOOOOOOOOOOOOOOOOOOOO				/////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////				  OOOOOOOOOOOOOOOOOOOOO				/////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////				  OOOOOOOOOOOOOOOOOOOOO				/////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////					        						/////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	case L:

		if (rot == rotation::left)
		{
			if ((target[y][x] == ' ') && (target[y][x + 1] == ' ') && (target[y][x + 2] == ' ') && (target[y + 1][x + 2] == ' ') && (x < 6))
			{
				legal = true;
			}
			else return false;
		}
		else if (rot == top)
		{
			if ((target[y][x] == ' ') && (target[y + 1][x] == ' ') && (target[y + 2][x] == ' ') && (target[y][x + 1] == ' ') && (x < 7))
			{
				legal = true;
			}
			else return false;
		}
		else if (rot == rotation::right)
		{
			if ((target[y][x] == ' ') && (target[y + 1][x] == ' ') && (target[y + 1][x + 1] == ' ') && (target[y + 1][x + 2] == ' ') && (x < 6))
			{
				legal = true;
			}
			else return false;
		}
		else if (rot == bottom)
		{
			if ((target[y][x] == ' ') && (target[y + 1][x] == ' ') && (target[y + 2][x] == ' ') && (target[y + 2][x - 1] == ' ') && (x > 0))
			{
				legal = true;
			}
			else return false;
		}
		else return false;

		if (legal)
		{
			if (rot == rotation::left)
			{
				target[y][x] = 'L';
				target[y][x + 1] = 'L';
				target[y][x + 2] = 'L';
				target[y + 1][x + 2] = 'L';
				return true;
			}
			else if (rot == top)
			{
				target[y][x] = 'L';
				target[y + 1][x] = 'L';
				target[y + 2][x] = 'L';
				target[y][x + 1] = 'L';
				return true;
			}
			else if (rot == rotation::right)
			{
				target[y][x] = 'L';
				target[y + 1][x] = 'L';
				target[y + 1][x + 1] = 'L';
				target[y + 1][x + 2] = 'L';
				return true;
			}
			else if (rot == bottom)
			{
				target[y][x] = 'L';
				target[y + 1][x] = 'L';
				target[y + 2][x] = 'L';
				target[y + 2][x - 1] = 'L';
				return true;
			}
			else return false;
		}

		break;
	}
}

int get_next_permutation (int items [], int itemsLength)
{
	int num = 100;
	qsort(items, itemsLength, sizeof(int), compare);

	vector<int> out;
	vector<int> v;
	for (uint8_t i = 0; i != itemsLength; ++i) v.push_back(items[i]);

	copy(v.begin(), v.end(), inserter(out, out.begin()));

	for (int p : out)
	{
		try_place_piece(p);
	}

	if (evaluate() <= num)
	{
		num = evaluate();
		for (uint8_t i = 0; i < 10; i++)
			for (uint8_t j = 0; j < 8; j++)
				field_final[i][j] = field_res[i][j];

		for (int i = 9; i >= 0; i--)
		{
			cout << "     |";
			for (int j = 0; j < 8; j++)
				cout << field_final[i][j];
			cout << "|" << endl;
		}
		cout << "      ````````" << endl;

		cout << evaluate() << endl << endl;
	}

	for (uint8_t i = 0; i < 10; i++)
		for (uint8_t j = 0; j < 8; j++)
			field[i][j] = ' ';

	for (uint8_t i = 0; i < 10; i++)
		for (uint8_t j = 0; j < 8; j++)
			field_res[i][j] = ' ';

		out.clear();

	while (next_permutation(v.begin(), v.end()))
	{
		copy(v.begin(), v.end(), inserter(out, out.begin()));
		
		for (int p : out)
		{
			try_place_piece(p);
		}

		if (evaluate() <= num)
		{
			num = evaluate();
			for (uint8_t i = 0; i < 10; i++)
				for (uint8_t j = 0; j < 8; j++)
					field_final[i][j] = field_res[i][j];

			for (int i = 9; i >= 0; i--)
			{
				cout << "     |";
				for (int j = 0; j < 8; j++)
					cout << field_final[i][j];
				cout << "|" << endl;
			}
			cout << "      ````````" << endl;

			cout << evaluate() << endl << endl;

		}

		for (uint8_t i = 0; i < 10; i++)
			for (uint8_t j = 0; j < 8; j++)
				field[i][j] = ' ';

		for (uint8_t i = 0; i < 10; i++)
			for (uint8_t j = 0; j < 8; j++)
				field_res[i][j] = ' ';

		out.clear();
	}
	return 0;
}



int compare(const void * x1, const void * x2)
{
	return (*(int*)x1 - *(int*)x2); 
}

int check()
{
	bool flag = false;
	int c = 0;
	int sum = 0;

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (field_res[i][j] != ' ')
			{
				continue;
			}
			else
			{
				c++;
				sum++;
			}
		}
		if (c == 8)
		{
			sum -= 8;
			break;
		}
		else c = 0;
	}

	return sum;

}

void draw()
{
	for (int i = 9; i >= 0; i--)
	{
		cout << "     |";
		for (int j = 0; j < 8; j++)
			cout << field[i][j];
		cout << "|" << endl;
	}
	cout << "      ````````" << endl;

	cout << check() << endl << endl;
}

void draw_1()
{
	for (int i = 9; i >= 0; i--)
	{
		cout << "     |";
		for (int j = 0; j < 8; j++)
			cout << field_res[i][j];
		cout << "|" << endl;
	}
	cout << "      ````````" << endl;

	cout << check() << endl << endl;

	cout << "------------------------------------------------------------------------------------------------------";
}

void clean()
{
	for (uint8_t i = 0; i < 10; i++)
		for (uint8_t j = 0; j < 8; j++)
		{
			field[i][j] = ' ';
			field[i][j] = field_res[i][j];
		}

}

int evaluate()
{
	bool flag = false;
	int c = 0;
	int k = 0;
	int sum = 0;
	int full_rows = 0;
	int streak = 0;
	int max_streak = 1;
	int streak_c = -1;

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (field_res[i][j] != ' ')
			{
				k++;
				continue;
			}
			else
			{
				c++;
				sum++;
			}
		}
		if (c == 8)
		{
			sum -= 8;
			break;
		}
		if (k == 8)
		{
			full_rows++;
			k = 0;
			if (streak_c == i - 1)
			{
				streak++;
				streak_c = i;
			}
			else
			{
				if (streak > max_streak) max_streak = streak;
				streak = 0;
			}
		}
		else
		{
			c = 0;
			k = 0;
		}
	}
	if (streak > max_streak) max_streak = streak;

	return sum - (full_rows * 8);
}