#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int degree = 0, row = 0, column = 0, **data, **open, **mark, mined_num = 0, remain_cell = 0, mark_num = 0, open_times = 0;

//ゲームの初期設定を案内する関数
void start_guide(){
	while(row < 5 || row > 15){
		printf("Enter the number of rows (an integer within the range from 5 to 15):");
		scanf("%d", &row);
		if(row < 5 || row > 15){
			printf("Invalid value! Enter an integer within the range from 5 to 15.\n");
		}
	}

	while(column < 5 || column > 15){
		printf("Enter the number of columns (an integer within the range from 5 to 15):");
		scanf("%d", &column);
		if(column < 5 || column > 15){
			printf("Invalid value! Enter an integer within the range from 5 to 15.\n");
		}
	}

	while(degree != 1 && degree != 2 && degree != 3){
		printf("Select the degree of diffculty of the game(1: novice, 2: middle, 3: advanced):\n");
		scanf("%d", &degree);
		if(degree != 1 && degree != 2 && degree != 3){
			printf("Invalid value!\n");
		}
	}
}

//初期設定に従って、必要な領域を確保する関数
void mallocBoard(){
	int i;

	data = (int **)malloc(sizeof(int *) * row);
	open = (int **)malloc(sizeof(int *) * row);
	mark = (int **)malloc(sizeof(int *) * row);

	for(i = 0; i < row; i++){
		data[i] = (int *)malloc(sizeof(int) * column);
		open[i] = (int *)malloc(sizeof(int) * column);
		mark[i] = (int *)malloc(sizeof(int) * column);
	}
}

//各セルに関する変数を初期化する関数
void initBoard(){
	int i, j;
	double random;

	srand((unsigned int)time(NULL));

	for(i = 0; i < row; i++){
		for(j = 0; j < column; j++){
			data[i][j] = 0;
			open[i][j] = 0;
			mark[i][j] = 0;

			random = (double)rand() / RAND_MAX;
			if(random <= 0.2 * degree){
				data[i][j] = -1;
				mined_num++;
			}
		}
	}
	remain_cell = row * column - mined_num;
}

//地雷ではないセルに周りの地雷の数を計算する関数
void calc_neighboring_cells(){
	int i, j;

	for(i = 0; i < row; i++){
		for(j = 0; j < column; j++){
			if(data[i][j] != -1){
				if((j - 1) >= 0){
					if((i - 1) >= 0){
						if(data[i - 1][j - 1] == -1){
							data[i][j] += 1;
						}
					}
					if((i + 1) <= row - 1){
						if(data[i + 1][j - 1] == -1){
							data[i][j] += 1;
						}
					}
					if(data[i][j - 1] == -1){
						data[i][j] += 1;
					}
				}
				if((j + 1) <= column - 1){
					if((i - 1) >= 0){
						if(data[i - 1][j + 1] == -1){
							data[i][j] += 1;
						}
					}
					if((i + 1) <= row - 1){
						if(data[i + 1][j + 1] == -1){
							data[i][j] += 1;
						}
					}
					if(data[i][j + 1] == -1){
						data[i][j] += 1;
					}
				}
				if(((i - 1) >= 0) && (data[i - 1][j] == -1)){
					data[i][j] += 1;
				}
				if(((i + 1) <= row - 1) && (data[i + 1][j] == -1)){
					data[i][j] += 1;
				}
			}
		}
	}
}

//ゲームボードを表示する関数
void showBoard(){
	int i, j;

	printf("  ");
	for(i = 0; i < column; i++){
		printf("|%3d", i);
	}
	printf("\n");

	printf("--");
	for(i = 0; i < column; i++){
		printf("+---");
	}
	printf("\n");

	for(i = 0; i < row; i++){
		printf("%2d", i);
		for(j = 0; j < column; j++){
			if(open[i][j] == 1){
				if(data[i][j] != -1){
					printf("|%2d ", data[i][j]);
				}
				else{
					printf("| X ");
				}
			}
			else if(mark[i][j] == 1){
				printf("| M ");
			}
			else{
				printf("| ? ");
			}
		}
		printf("\n");
	}
}

//全てのセルをオープンする関数
void open_all(){
	int i, j;

	for(i = 0; i < row; i++){
		for(j = 0; j < column; j++){
			open[i][j] = 1;
		}
	}
	showBoard();
}

//周りに地雷がないセルを再帰的にオープンする関数
void rec_open(int x, int y){
	if((y - 1) >= 0){
		if((x - 1) >= 0){
			if(data[x - 1][y - 1] != -1 && open[x - 1][y - 1] == 0){
				remain_cell -= 1;
				open[x - 1][y - 1] = 1;
				if(data[x - 1][y - 1] == 0){
					rec_open(x - 1, y - 1);
				}
			}
		}
		if((x + 1) <= row - 1){
			if(data[x + 1][y - 1] != -1 && open[x + 1][y - 1] == 0){
				remain_cell -= 1;
				open[x + 1][y - 1] = 1;
				if(data[x + 1][y - 1] == 0){
					rec_open(x + 1, y - 1);
				}
			}
		}
		if(data[x][y - 1] != -1 && open[x][y - 1] == 0){
			remain_cell -= 1;
			open[x][y - 1] = 1;
			if(data[x][y - 1] == 0){
				rec_open(x, y - 1);
			}
		}
	}
	if((y + 1) <= column - 1){
		if((x - 1) >= 0){
			if(data[x - 1][y + 1] != -1 && open[x - 1][y + 1] == 0){
				remain_cell -= 1;
				open[x - 1][y + 1] = 1;
				if(data[x - 1][y + 1] == 0){
					rec_open(x - 1, y + 1);
				}
			}
		}
		if((x + 1) <= row - 1){
			if(data[x + 1][y + 1] != -1 && open[x + 1][y + 1] == 0){
				remain_cell -= 1;
				open[x + 1][y + 1] = 1;
				if(data[x + 1][y + 1] == 0){
					rec_open(x + 1, y + 1);
				}
			}
		}
		if(data[x][y + 1] != -1 && open[x][y + 1] == 0){
			remain_cell -= 1;
			open[x][y + 1] = 1;
			if(data[x][y + 1] == 0){
				rec_open(x, y + 1);
			}
		}
	}
	if((x - 1) >= 0){
		if(data[x - 1][y] != -1 && open[x - 1][y] == 0){
			remain_cell -= 1;
			open[x - 1][y] = 1;
			if(data[x - 1][y] == 0){
				rec_open(x - 1, y);
			}
		}
	}
	if((x + 1) <= row - 1){
		if(data[x + 1][y] != -1 && open[x + 1][y] == 0){
			remain_cell -= 1;
			open[x + 1][y] = 1;
			if(data[x + 1][y] == 0){
				rec_open(x + 1, y);
			}
		}
	}
}

//第1回目にオープンしたセルは地雷の時、地雷ではないようにする関数
void re_initBoard(int x, int y){
	int i, j, substitute, counter = 0;
	double random;

	srand((unsigned int)time(NULL));
	
	random = (double)rand() / RAND_MAX;
	substitute = random * (remain_cell - 1);
	
	for(i = 0; i < row; i++){
		for(j = 0; j < column; j++){
			if(data[i][j] != -1){
				counter += 1;
			}
			if(counter == substitute){
				data[i][j] = -1;
				data[x][y] = 0;
				break;
			}
		}
		if(counter == substitute){
			break;
		}
	}

	for(i = 0; i < row; i++){
		for(j = 0; j < column; j++){
			if(data[i][j] != -1){
				data[i][j] = 0;
			}
		}
	}
	calc_neighboring_cells();
}

int main(){
	int operation, x, y;
	
	start_guide();

	mallocBoard();
	
	initBoard();

	calc_neighboring_cells();

	showBoard();

	printf("The number of mined cells: %d\n", mined_num);
	printf("Select the operation for the selected cell (1: open, 2: mark): ");
	scanf("%d", &operation);
	
	printf("Enter the coordinate of a cell to operate\n");
	printf("(row and column numbers divided by an en space):");
	scanf("%d %d", &x, &y);

	//第1回目にオープンしたセルは地雷の時の処理
	if(operation == 1 && x >= 0 && x <= row - 1 && y >= 0 && y <= column - 1){
		open_times += 1;
		if(data[x][y] == -1){
			re_initBoard(x, y);
		}
	}

	//すべての地雷ではセルをオープンしていない限り
	while(remain_cell != 0){
		//入力する座標が範囲内かどうかを判断する
		if(x < 0 || x > row - 1 || y < 0 || y > column - 1){
			printf("Incorrect coordinate!\n");
		}
		else{
			//第1回目にオープンしたセルは地雷の時の処理
			if(operation == 1){
				open_times += 1;
				if(open_times == 1 && data[x][y] == -1){
					re_initBoard(x, y);
				}
			}
			//地雷を踏んだ後の処理
			if(operation == 1 && data[x][y] == -1 && open_times != 1){
				open_all();
				showBoard();
				printf("GAME OVER!\n");
				
				return 0;
			}
			else{
				//すでにオープンしたセルをオープンする時の処理
				if(open[x][y] == 1){
					printf("The cell is already open.\n");
					if(operation == 2 || operation == 3){
						showBoard();
						printf("The number of mined cells: %d\n", mined_num);
					}
				}
				//マークしていないセルのマークを消す時の処理
				else if(operation == 3 && mark[x][y] == 0){
					printf("This cell is not marked\n");
					showBoard();
					printf("The number of mined cells: %d\n", mined_num);
				}
				else{
					//地雷ではないセルをオープンする時の処理
					if(operation == 1){
						remain_cell -= 1;
						open[x][y] = 1;
						if(data[x][y] == 0){
							rec_open(x, y);
						}
						if(remain_cell != 0){
							showBoard();
						}
					}
					//セルをマークする時の処理
					else if(operation == 2){
						mark[x][y] = 1;
						mark_num += 1;
						showBoard();
					}
					//マークを消す時の処理
					else{
						mark[x][y] = 0;
						mark_num -= 1;
						showBoard();
					}
					if(remain_cell == 0){
						break;
					}
					printf("The number of remaining cells to open: %d\n", remain_cell);
					printf("Select the operation for the selected cell (1: open");
					//マークできるセルがある時の処理
					if(mark_num != remain_cell + mined_num){
						printf(", 2: mark");
					}
					//マークを消せるセルがある時の処理
					if(mark_num > 0){
						printf(", 3: remove marking");
					}
					printf("): ");
					scanf("%d", &operation);
				}
			}
		}
		printf("Enter the coordinate of a cell to operate\n");
		printf("(row and column numbers divided by an en space):");
		scanf("%d %d", &x, &y);
	}
	//ゲームをクリアする時の処理
	open_all();
	printf("Congratulations!\n");
	
	return 0;
}
