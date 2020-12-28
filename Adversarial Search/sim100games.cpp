/*
  Takes two heuristics options to simulate 100 games
  Make a toss. decide player 1 and 2.
  simulate the game.
  show stats.
*/

#include <bits/stdc++.h>
using namespace std;

#define SORT_CHILDREN 1 //sorts the child nodes by evaluation heuristics
#define DEBUG 0
#define INF 1e9

/*
#define W1 4
#define W2 7
#define W3 6
#define W4 9
*/

int W1, W2, W3, W4;

int mm_call = 0;

struct Board{
    //places
    int slot[14];
    int whoseturn;
    int additional_move_earned;
    int stones_captured;
    int heuristic_val;
    //Description
    //0-5 -> Player 1 slots Left to Right
    //  6 -> Player 1 Mancala
    //7-12 -> Player 2 slots Left to Right
    //  13 -> Player 2 Mancala

    Board(){
        whoseturn = 1;
        additional_move_earned = 0;
        stones_captured = 0;
        heuristic_val = 0;
        slot[6] = slot[13] = 0;
        for(int i = 0; i<=5; i++) slot[i] = slot[i+7] = 4;
    }
    void printBoard(){
        cout<<"\n>>>   BOARD STATE   <<<\n"<<endl;
        int cnt = 0;
        for(int i = 0; i<14; i++) cnt += slot[i];
        if(cnt!=48){
          cout<<"P R O B L E M !!! STONES : "<<cnt<<endl;
        }
        //player 1 upper
        //player 2 lower
        cout<<"----- Player 1-----"<<endl;
        printf("\t");
        for(int i = 5; i>=0; i--) printf("%4d", slot[i]);
        printf("\t\n");

        printf("(%4d)", slot[6]);
        for(int i = 5; i>=0; i--) printf("\t");
        printf("(%4d)\n", slot[13]);

        printf("\t");
        for(int i = 7; i<=12; i++) printf("%4d", slot[i]);
        printf("\t\n");
        cout<<"----- Player 2-----"<<endl;

        cout<<"Next move: Player "<<whoseturn<<endl;
    }

    bool isLeaf(){
      int stones = 0;
      for(int i = 0; i<=5; i++){
        stones += slot[i];
      }
      if(stones==0) return true;

      stones = 0;
      for(int i = 7; i<=12; i++){
        stones += slot[i];
      }
      if(stones==0) return true;
      return false;
    }

    int evaluate(int heuristic_num){
      int stones_in_store1 = slot[6], stones_in_store2 = slot[13];

      if(heuristic_num==1){
        return stones_in_store1 - stones_in_store2;
      }

      int stones_on_player1_side = 0, stones_on_player2_side = 0;
      for(int i = 0; i<6; i++) stones_on_player1_side += slot[i];
      for(int i = 0; i<6; i++) stones_on_player2_side += slot[i+7];

      if(heuristic_num==2){
      	W1 = 16, W2 = 1, W3 = 0, W4 = 0;
        return W1*(stones_in_store1-stones_in_store2) + W2*(stones_on_player1_side-stones_on_player2_side);
      }

      //another way to look at additional_move_earned - begins
      additional_move_earned = 0;
      if(whoseturn==1){
        for(int i = 0; i<6; i++) if(slot[i]==(6-i)) additional_move_earned++;
      } else{
        for(int i = 7; i<14; i++) if(slot[i]==(14-i)) additional_move_earned++;
        additional_move_earned = (-additional_move_earned);
      }
      //another way to look at additional_move_earned - ends
		
      if(heuristic_num==3){
      	W1 = 16, W2 = 1, W3 = 4, W4 = 0;
        return W1*(stones_in_store1-stones_in_store2) + W2*(stones_on_player1_side-stones_on_player2_side) + W3*additional_move_earned;
      }
      //another way to look at stones_captured - begins
      if(whoseturn==1){
        stones_in_store1 -= stones_captured;
      } else{
        stones_in_store2 += stones_captured;
      }
      //another way to look at stones_captured - ends
      if(heuristic_num==4){
      	W1 = 16, W2 = 1, W3 = 10, W4 = 20;
        return W1*(stones_in_store1-stones_in_store2) + W2*(stones_on_player1_side-stones_on_player2_side) + W3*additional_move_earned + W4*stones_captured;
      }
    }

} typedef board;

struct less_than_key
{
    inline bool operator() (const board& struct1, const board& struct2)
    {
        return (struct1.heuristic_val < struct2.heuristic_val);
    }
};


vector<board> generateChildren(board parent, int heuristic_choice);
int minimax(board b, int depth, int alpha, int beta, int heuristic_choice);

//gameplay
board human_agent(board curr){
  int side = curr.whoseturn;

  cout<<"Choose slot (1~6): ";
  int choice ; cin>>choice;
  int now_at = (side-1)*7 + choice; // so if i'm player 2 and make choice 4, now_at is (2-1)*7+4 = 11, just where i should start dropping bodies
  int stones = curr.slot[now_at-1];
  curr.slot[now_at-1] = 0;
  while(stones){
    if((side==1 && now_at==13) || (side==2 && now_at==6)){
      //opponent mancala - so do nothing
    }
    else{
      curr.slot[now_at]++;
      stones--;
      if(stones==0){
        //last drop - check if own mancala
        if(now_at!=((side-1)*7+6)){
          curr.whoseturn = 3 - side;
        }
        //check if last slot is of own and was empty
        int low = (side-1)*7, high = low+5;
        if(curr.slot[now_at]==1 && now_at>=low && now_at<=high){
          curr.slot[(side-1)*7+6] += (curr.slot[now_at] + curr.slot[12-now_at]);
          curr.slot[now_at] = curr.slot[12-now_at] = 0;
        }
      }
    }
    now_at = (now_at+1)%14;
  }
  return curr;
}

int main()
{
    /*
    board b;
    //b.whoseturn = 2;
    int d, heu; cin>>d>>heu;
    cout<<minimax(b, d, -INF, +INF, heu)<<endl;
    cout<<mm_call<<endl;
    */
    int p1, p2;
    //int depth_for_heuristics[] = {8, 8, 8, 8};
    //int depth_for_heuristics[] = {2, 2, 2, 2};
    int depth_for_heuristics[] = {6, 6, 6, 6};
    
    cout<<"Heuristics(Enter two heuristic ids 1~4): ";
    int opt1, opt2;
    cin>>opt1>>opt2;
    int num_of_games, game_id = 1;
    cout<<"Number of games? ";
    cin>>num_of_games;
    
    int toss_win_1 = 0;
    int game_win_1 = 0;
    int game_win_2 = 0;
    
      freopen("output.txt", "w", stdout);
      freopen ("/dev/tty", "a", stdout);
    
    while(game_id <= num_of_games){
      int toss = rand()%2+1;
      cout<<"Simulating game #"<<game_id<<":::"<<endl;
      game_id++;
      if(toss==1){
        //opt1 wins the toss, gets to be player 1
        toss_win_1++;
        p1 = opt1;
        p2 = opt2;
        cout<<"Heuristic "<<opt1<<" wins the toss..."<<endl;
      }
      else{
        //opt2 wins the toss, gets to be player 1
        p1 = opt2;
        p2 = opt1;
        cout<<"Heuristic "<<opt2<<" wins the toss..."<<endl;
      }

      freopen("output.txt", "a", stdout);
      board state;
      while(true){
        state.printBoard();
        //termination check
        if(state.isLeaf()){
          cout<<"\n---GAME OVER---\n";
          int stones = 0;
          for(int i = 0; i<14; i++){
            if(i==6 || i==13){
              state.slot[i] += stones;
              stones = 0;
            }
            else{
              stones += state.slot[i];
              state.slot[i] = 0;
            }
          }
          int diff = state.slot[6]-state.slot[13];
          if(diff>0){
            cout<<"Heuristic "<<p1<<" wins!!!"<<endl;
            freopen ("/dev/tty", "a", stdout);
            cout<<"GAME OVER! Heuristic "<<p1<<" wins!!!"<<endl<<endl;
            if(toss==1){
              //p1 must be opt1
              game_win_1++;
            }
            else{
              //p1 must be opt2
              game_win_2++;
            }
          }
          else if(diff<0){
            cout<<"Heuristic "<<p2<<" wins!!!"<<endl;
            freopen ("/dev/tty", "a", stdout);
            cout<<"GAME OVER! Heuristic "<<p2<<" wins!!!"<<endl<<endl;
            if(toss==1){
              //p2 must be opt2
              game_win_2++;
            }
            else{
              //p2 must be opt1
              game_win_1++;
            }
          }
          else{
            cout<<"Match Draw!"<<endl;
            freopen ("/dev/tty", "a", stdout);
            cout<<"GAME OVER! Match Draw!"<<endl<<endl;
          }
          break;
        }
        //who playes
        int agent;
        if(state.whoseturn==1) agent = p1;
        else agent = p2;

        if(agent==0){
          state = human_agent(state);
        }
        else {
          cout<<"AI is Thinking ... "<<endl;
          //cout<<"Whose turn : "<<state.whoseturn<<endl;
          vector<board> children = generateChildren(state, agent);
          int sz = children.size();
          board next_state;
          if(state.whoseturn==1){
            //max node
            int val = -INF;
            for(int i = 0; i<sz; i++){
              int tmp = minimax(children[i], depth_for_heuristics[agent-1], -INF, +INF, agent);
              if(tmp>val){
                val = tmp;
                next_state = children[i];
              }
            }
            //cout<<"BOX 1 :: "<<val<<endl<<endl;
          }
          else if(state.whoseturn==2){
            //min node
            int val = INF;
            for(int i = 0; i<sz; i++){
              int tmp = minimax(children[i], depth_for_heuristics[agent-1], -INF, +INF, agent);
              if(tmp<val){
                val = tmp;
                next_state = children[i];
              }
            }
            //cout<<"BOX 1 :: "<<val<<endl<<endl;
          }
          state = next_state;
        }
      }
      //a game ends
    }
    cout<<"\n:::Stats over all matches:::"<<endl;
    cout<<"Heuristic "<<opt1<<" wins "<<game_win_1<<" matches!"<<endl;
    cout<<"Heuristic "<<opt2<<" wins "<<game_win_2<<" matches!"<<endl;
    cout<<"Draw "<<(num_of_games- game_win_1 - game_win_2)<<" matches!"<<endl;

    cout<<"\n\nPercentage:"<<endl;
    cout<<"Heuristic "<<opt1<<": "<<(game_win_1*100.0/num_of_games)<<"%\n";
    cout<<"Heuristic "<<opt2<<": "<<(game_win_2*100.0/num_of_games)<<"%\n";
    cout<<"Draw: "<<((num_of_games- game_win_1 - game_win_2)*100.0/num_of_games)<<"%\n";

    cout<<endl<<"TOSS ratio>> "<<(toss_win_1*100.0)/num_of_games<<"% : "<<((num_of_games - toss_win_1)*100.0)/num_of_games<<"%"<<endl;
    return 0;
}



vector<board> generateChildren(board parent, int heuristic_choice){
    vector<board> children;
    int start, mancala_this, mancala_that;
    if(parent.whoseturn==1){//player 1
        start = 0; mancala_this = 6; mancala_that = 13;
    } else{//player 2
        start = 7; mancala_this = 13; mancala_that = 6;
    }
    for(int i = 0; i<6; i++){
        int selected_slot_id = start+i, stones = parent.slot[selected_slot_id];
        if(stones>0){
          board child = parent; //valid choice
          child.slot[selected_slot_id] = 0;
          child.stones_captured = 0;
          child.additional_move_earned = 0;
          for(int j = 1;  ; j++){
              int slot_id = (selected_slot_id + j) % 14;
              if(slot_id!=mancala_that){
                stones--;
                child.slot[slot_id]++;
              }
              if(stones==0){
                //If the last piece you drop is in your own store, you get a free turn.
                if(slot_id==mancala_this) {
                  child.whoseturn = parent.whoseturn;
                  child.additional_move_earned = 1;
                } else child.whoseturn = 3 - parent.whoseturn;
                //If the last piece you drop is in an empty hole on your side, you capture that piece and any pieces in the hole directly opposite.
                if(slot_id>=start && slot_id<mancala_this && child.slot[slot_id]==1){
                  int opposite_slot_id = 12 - slot_id;
                  child.stones_captured += (child.slot[slot_id] + child.slot[opposite_slot_id]);
                  child.slot[mancala_this] += (child.slot[slot_id] + child.slot[opposite_slot_id]);
                  child.slot[slot_id] = 0; child.slot[opposite_slot_id] = 0;
                }
                break;
              }
          }
          //in case of min node, negate stones_captured - these stones were captured by the player whoseturn - guaranteed
          if(parent.whoseturn==2) {
            child.stones_captured = (-1) * child.stones_captured;
            //if parent.whoseturn was 1, more stones_captured is good, so +
            //if not, more stones_captured is good for player 2, so -
            child.additional_move_earned = (-1) * child.additional_move_earned;
          }
          if(child.isLeaf()){
            //game over - need to capture all pieces to mancala
            int stones = 0;
            for(int i = 0; i<=5; i++) {
              stones += child.slot[i];
              child.slot[i] = 0;
            }
            child.slot[6] += stones;

            child.stones_captured += stones; //good for player 1, max node, so add

            stones = 0;
            for(int i = 7; i<=12; i++) {
              stones += child.slot[i];
              child.slot[i] = 0;
            }
            child.slot[13] += stones;

            child.stones_captured -= stones; //good for player 2, min node, so neg
          }

          child.heuristic_val = child.evaluate(heuristic_choice);
          children.push_back(child);
        }
    }
    //ADD CODE
    //maybe sorting here based on heuristic value should be done here for better performance
    if(SORT_CHILDREN){
      sort(children.begin(), children.end(), less_than_key());
      if(parent.whoseturn==1){
        //bring best child first for max node
        reverse(children.begin(), children.end());
      }
    }
    //for(int i = 0; i<children.size(); i++) cout<<children[i].heuristic_val<<" ";
    //cout<<endl;
    return children;
}



int minimax(board b, int depth, int alpha, int beta, int heuristic_choice){
  mm_call++;
  if(DEBUG && mm_call%100000==0) cout<<"Depth: "<<depth<<" Func Call: "<<mm_call<<endl;
  if(b.isLeaf() || depth==0) {
    //b.additional_move_earned = 0;
    return b.evaluate(heuristic_choice);
  }
  vector<board> ch = generateChildren(b, heuristic_choice);
  int ch_cnt = ch.size();
  //if b is a max node
  if(b.whoseturn==1){
    int v = alpha;
    for(int i = 0; i<ch_cnt; i++){
      int v1 = minimax(ch[i], depth-1, v, beta, heuristic_choice);
      if(v1>v) v = v1;
      if(v>beta) return beta;
    }
    //cout<<"***p 1 - s turn *** at "<<depth<<" : "<<v<<endl<<endl;
    return v;
  }
  //if b is a min node
  if(b.whoseturn==2){
    int v = beta;
    for(int i = 0; i<ch_cnt; i++){
      int v1 = minimax(ch[i], depth-1, alpha, v, heuristic_choice);
      if(v1<v) v = v1;
      if(v<alpha) return alpha;
    }
    //cout<<"***p 2 - s turn *** at "<<depth<<" : "<<v<<endl<<endl;
    return v;
  }
}
