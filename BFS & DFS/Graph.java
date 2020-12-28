/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package aioffline_1;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.Queue;
import java.util.Stack;

/**
 *
 * @author lelmil
 */
public class Graph {
    int N_m;
    int N_c;
    int boat_capacity;
    
    long timeOut;
    int expand_node_lim;
    //open list
    Stack<State> stateStack;
    Queue<State> stateQueue;
    //closed list
    ArrayList<State> closedList;
    
    public Graph(int n_m, int n_c, int cap){
        
        //to set timeout
        timeOut = 30 * 1000; //30 sec
        //set expanded nodes limit
        expand_node_lim = 1000000; // max of this num of nodes are allowed to expand
        
        this.boat_capacity = cap;
        this.N_m = n_m;
        this.N_c = n_c;
        
        stateStack = new Stack<>();
        stateQueue = new LinkedList<>();
        this.closedList = new ArrayList<>();
        
        State root= new State(N_m, N_c, 0, N_m, N_c);
        stateStack.push(root);
        stateQueue.add(root);
    }
    
    public State bfs(){
        
        long startTime = System.currentTimeMillis();
        //System.out.println(startTime);
        closedList.clear();
        //using queue
        int expanded = 0;
        while(!stateQueue.isEmpty()){
            long currTime = System.currentTimeMillis() - startTime;
            //System.out.println(currTime);
            if(currTime>this.timeOut){
                System.out.println("BFS Timeout!");
                return null;
            }
            State tmp = stateQueue.remove();
            //goal test
            if(tmp.isGoal()){
                return tmp;
            }
            //System.out.println(stateQueue);
            closedList.add(tmp);
            //generate nodes - check if in closed list - push in open list if needed
            for(int x=0; x<=this.boat_capacity; x++){
                for(int y=0; y<= (this.boat_capacity-x); y++){
                    //add here of for boat constraint - "&& (x==0 || x>=y) "
                    if((x+y)>0 && (x==0 || x>=y)){
                        if(tmp.isValid(x,y,0)){
                            State new_state = tmp.afterTransition(x,y,0);
                            //System.out.println(new_state);
                            if(new_state.isGoal()){
                                return new_state;
                            }
                            if(!stateQueue.contains(new_state) && !closedList.contains(new_state))stateQueue.add(new_state);
                        }
                        if(tmp.isValid(x,y,1)){
                            State new_state = tmp.afterTransition(x,y,1);
                            //System.out.println(new_state);
                            if(new_state.isGoal()){
                                return new_state;
                            }
                            if(!stateQueue.contains(new_state) && !closedList.contains(new_state))stateQueue.add(new_state);
                        }
                    }
                }
            }
            expanded++;
            if(expanded>=expand_node_lim){
                System.out.println("Number of expanded nodes crossed limit!");
                return null;
            }
        }
        return null;
    }
    
    public State dfs(){
        long startTime = System.currentTimeMillis();
        closedList.clear();
        //using stack
        int expanded = 0;
        while(!stateStack.empty()){
            long currTime = System.currentTimeMillis() - startTime;
            //System.out.println(currTime);
            if(currTime>this.timeOut){
                System.out.println("DFS Timeout!");
                return null;
            }
            State tmp = stateStack.pop();
            //goal test
            if(tmp.isGoal()){
                return tmp;
            }
            closedList.add(tmp);
            for(int x=0; x<=this.boat_capacity; x++){
                for(int y=0; y<=(this.boat_capacity-x); y++){
                    //add here of for boat constraint - "&& (x==0 || x>=y) "
                    if((x+y)>0 && (x==0 || x>=y)){
                        if(tmp.isValid(x, y, 0)){
                            State new_state = tmp.afterTransition(x, y, 0);
                            if(new_state.isGoal()){
                                return new_state;
                            }
                            if(!stateStack.contains(new_state) && !closedList.contains(new_state)) stateStack.push(new_state);
                        }
                        if(tmp.isValid(x, y, 1)){
                            State new_state = tmp.afterTransition(x, y, 1);
                            if(new_state.isGoal()){
                                return new_state;
                            }
                            if(!stateStack.contains(new_state) && !closedList.contains(new_state)) stateStack.push(new_state);
                        }
                        
                    }
                }
            }
            expanded++;
            if(expanded>=expand_node_lim){
                System.out.println("Number of expanded nodes crossed limit!");
                return null;
            }
        }
        return null;
    }
}
