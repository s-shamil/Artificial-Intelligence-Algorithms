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
    string players[] = {"Human agent", "AI with heuristic-1", "AI with heuristic-2", "AI with heuristic-3", "AI with heuristic-4"};
    //int depth_for_heuristics[] = {8, 8, 8, 8};
    //int depth_for_heuristics[] = {2, 2, 2, 2};
    int depth_for_heuristics[] = {6, 6, 6, 6};
    
    cout<<"Player options: ";
    for(int i = 0; i<5; i++){
      cout<<i+1<<". "<<players[i]<<" ";
    }
    cout<<"\nPlayer-1 : ";
    cin>>p1; p1--;
    cout<<"\nPlayer-2 : ";
    cin>>p2; p2--;
    
    
    freopen("output.txt", "w", stdout);
    

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
          cout<<"Player 1 wins!!!"<<endl;
        }
        else if(diff<0){
          cout<<"Player 2 wins!!!"<<endl;
        }
        else{
          cout<<"Match Draw!"<<endl;
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
