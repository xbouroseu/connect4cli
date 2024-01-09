// Copyright 2012
#include <stdio.h>
#include <stdlib.h>
#define N 7
#define EPS 10E-8

typedef struct move {
    float value;
    int column;
    int depth;
} Move;

void play(int);
void play_human();
void play_cpu();
void print_table();
char get_sign(int);
void insert_sign(int,int);
int check_winner(int);
int get_col_cell(int);
int mark(int,int,int,int,int);
float evaluate_board();
int check_hyp_win(int);
float get_possible_triples(int);
int get_priority(int);
Move play_AI(int,int);

char table[N][N];
int X_PLAYED,Y_PLAYED;
int CPU_LEVEL=0;
int priority[N] = {3,2,4,1,5,0,6};

int main() {
    int winner = 0,player = 1,i,j;
    for(i=0;i<N;i++) {
        for(j=0;j<N;j++) {
            table[i][j] = ' ';
        }
    }
    
    do {
        printf("Give CPU Level (1-10): ");
        scanf("%d",&CPU_LEVEL);
    } while(CPU_LEVEL<1 || CPU_LEVEL>10);
    
    while(winner == 0) {
        printf("It is turn of player '%c'..\n",get_sign(player));
        play(player);
        winner = check_winner(player);
        player*=-1;
    }
    
    print_table();
    switch(winner){
        case 2: printf("\nWe have a tie!!\n\n"); break;
        default: printf("\nWe have a winner!!\n'%c' player wins!!\n\n",get_sign(winner)); break;
    }
    
    system("PAUSE");	
    return 0;
}

char get_sign(int player) {
    if(player==-1) {
        return '0';
    }
    
    return 'X';
}

void print_table() {
    int i,j,k;
    printf("\n");
    for(i=0;i<N;i++) {
        for(j=0;j<N;j++) {
            printf("| %c ",table[i][j]);
            if(j==N-1) {
                printf("|");
            }
        }
        printf("\n");
        
        for(k=0;k<N;k++) {
            printf("+---");
        }
        printf("+\n");
    }
    printf("  ");
    for(k=1;k<=N;k++) {
        printf("%d   ",k);
    }
    printf("\n");
}

int get_col_cell(int col) {
    int i=0;
    
    while(i<N && table[i][col] == ' ') {
        i++;
    }
    
    return i-1;
}

void insert_sign(int player,int col) {
    int row = get_col_cell(col);
    
    table[row][col] = get_sign(player);
    X_PLAYED = row;
    Y_PLAYED = col;
}

void play_cpu(int player) {
    int col;
    Move move;
    
    printf("Computer playing.....\n");
    printf("Computer scans for best move....\n");
    
    move = play_AI(player,0);
    col = move.column;
    
    printf("Computer chooses column %d which gives best board score: %10.6f..\n",col+1,move.value);
    insert_sign(player,col);
}

void play_human(int player) {
    int col;
    print_table();
    
    do {
        printf("\nWhich column do you want? ");
        scanf("%d",&col);
    } while(table[0][col-1]!= ' ' || col>N || col<1);
    
    insert_sign(player,col-1);
}

void play(int player) {
    if(player==-1) {
        play_cpu(player);
    }
    else {
        play_human(player);
    }
    printf("\n------------------------------\n");
}

int mark(int x,int y,int x_dir,int y_dir,int player) {
    char sign = get_sign(player);
    if(x<0 || y<0 || x>=N || y>=N || table[x][y]!=sign) {
        return 0;  
    }
    return 1 + mark(x+x_dir,y+y_dir,x_dir,y_dir,player);
}

int check_winner(int player) {
    char sign = get_sign(player);
    int full = 1,i,j,x=X_PLAYED,y=Y_PLAYED;
    
    //Check if is tied
    for(i=0;i<N;i++) {
        for(j=0;j<N;j++) {
            if(table[i][j] == ' ') {
                full = 0;
            }
        }
    }
    
    if( full == 1 ) {
        return 2;
    }
    
    //Check Diagonally
    int marks_diag1 = 1;
    marks_diag1 += mark(x+1,y+1,1,1,player);
    marks_diag1 += mark(x-1,y-1,-1,-1,player);
    
    if(marks_diag1 >= 4) {
        return player;
    }
    
    int marks_diag2 = 1;
    marks_diag2 += mark(x-1,y+1,-1,1,player);
    marks_diag2 += mark(x+1,y-1,1,-1,player);
    
    if(marks_diag2 >= 4) {
        return player;
    }
    //Check Horizontally
    int marks_hor = 1;
    marks_hor += mark(x,y+1,0,1,player);
    marks_hor += mark(x,y-1,0,-1,player);
    
    if(marks_hor >= 4) {
        return player;
    }
    //Check Vertically
    int marks_ver = 1;
    marks_ver += mark(x+1,y,1,0,player);
    marks_ver += mark(x-1,y,-1,0,player);
    if(marks_ver >= 4) {
        return player;
    }
    
    return 0;
}

int check_hyp_win(int player) {
    int i,j;
    int x=X_PLAYED,y=Y_PLAYED;
    
    for(i=0;i<N;i++) {
        X_PLAYED=get_col_cell(i);
        Y_PLAYED=i;
        if(X_PLAYED!=-1) {
            if(check_winner(player)==player) {
                X_PLAYED = x;
                Y_PLAYED = y;
                return i;
            }
        }
    }
    X_PLAYED=x;
    Y_PLAYED=y;
    return -1;
}

float get_possible_triples(int player) {
    int marks_hor=0,marks_diag1=0,marks_diag2=0,marks_ver=0;
    float triples = 0.0;
    int i,x,y;
    
    for(i=0;i<N;i++) {
        x = get_col_cell(i);
        y = i;
        
        if( x!= -1 ) {
            marks_hor = 1 + mark(x,y+1,0,1,player) + mark(x,y-1,0,-1,player);
            marks_ver = 1 + mark(x+1,y,1,0,player) + mark(x-1,y,-1,0,player);
            marks_diag1 = 1 + mark(x+1,y+1,1,1,player) + mark(x-1,y-1,-1,-1,player);
            marks_diag2 = 1 + mark(x-1,y+1,-1,1,player) + mark(x+1,y-1,1,-1,player);
            
            triples += (marks_hor>=3) + (marks_ver>=3) + (marks_diag1>=3) + (marks_diag2>=3);
        }
    }
    return triples;
}

float evaluate_board() {
    float triples_1 = 0;
    float triples_2 = 0;
    int i,x,y;
    
    if(check_hyp_win(1)>-1) {
        return 100;
    }
    else if(check_hyp_win(-1)>-1) {
        return -100;
    }
    
    triples_1 = get_possible_triples(1);
    triples_2 = get_possible_triples(-1);
    
    if(triples_2 > triples_1) {
        return (triples_2/(triples_1+triples_2)) * -75;
    }
    else if(triples_1 > triples_2) {
        return (triples_1/(triples_1 + triples_2)) * 75;
    }
    
    return 0;
}

int get_priority(int column) {
    int i=0,found=0;
    
    while(!found) {
        if(priority[i++]==column) {
            found = 1;
        }
    }
    return i;
}

Move play_AI(int player,int depth) {
    int i,x,player_wins,k,p,forced_move;
    char sign = get_sign(player);
    Move ret_move,moves[7] = { {-5000*player} };
    
    ret_move.value = player*-100;
    
    player_wins = check_hyp_win(player);
    
    if(player_wins > -1) {
        ret_move.value = 100*player;
        ret_move.column = player_wins;
        ret_move.depth = depth;
        return ret_move;
    }
    
    if(depth==CPU_LEVEL) {
        ret_move.value = evaluate_board();
        ret_move.depth = depth;
        return ret_move;
    }
    else {
        for(i=0;i<N;i++) {
            x = get_col_cell(i);
            if(x!=-1) {
                table[x][i] = sign;
                moves[i] = play_AI(player*-1,depth+1);
                table[x][i] = ' ';
                if(depth==0) {
                    printf("Column %d outcome: %5.7f Depth: %d\n",i+1,moves[i].value,moves[i].depth);
                }
            }
            else {
                moves[i].value = player*-5000;
                if(depth==0) {
                    printf("Column %d invalid..\n",i+1);
                }
            }
        }
        
        
        //Minimize Maximize
        // Player 1 wants the best move for him so he is going to take the maximum possible value
        // Player 2 wants the best move for him so he is going to take the minimum possible value
        if(player==1) {
            for(k=0;k<N;k++) {
                p=priority[k];
                if(moves[p].value>ret_move.value) {
                    ret_move.value = moves[p].value;
                    ret_move.column = p;
                    ret_move.depth = moves[p].depth;
                }
                else if(moves[p].value-ret_move.value < EPS) {
                    forced_move = p;
                }
            }
        }
        else {
            for(k=0;k<N;k++) {
                p=priority[k];
                if(moves[p].value<ret_move.value) {
                    ret_move.value = moves[p].value;
                    ret_move.column = p;
                    ret_move.depth = moves[p].depth;
                }
                else if(moves[p].value-ret_move.value < EPS) {
                    forced_move = p;
                }
            }
        }
        
        //In case all moves are against, make a forced move
        if(ret_move.column>=N) {
            ret_move.column = forced_move;
            ret_move.value = moves[forced_move].value;
        }
        
        return ret_move;
    }
}
