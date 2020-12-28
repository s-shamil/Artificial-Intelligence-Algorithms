/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ai_offline2_new;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.PriorityQueue;

/**
 *
 * @author lelmil
 */
public class Solver {
    PriorityQueue<State> pq1;
    PriorityQueue<State> pq2;
    PriorityQueue<State> pq3;
    ArrayList<State> closedList;
    HashMap<Matrix, Integer> openList;
    State startStateOfSolver;
    
    public Solver(State startState){
        startStateOfSolver = new State(startState);
        openList = new HashMap<>();
        closedList = new ArrayList<>();
        pq1 = new PriorityQueue<>(1, (State n1, State n2) -> {
            // compare n1 and n2
            int ham1 = n1.getHammingDistance() + n1.prev_cost;
            int ham2 = n2.getHammingDistance() + n2.prev_cost;
            if(ham1<ham2) return -1;
            else if(ham1==ham2) return 0;
            else return 1;
        });
        pq1.add(new State(startState));
        
        pq2 = new PriorityQueue<>(1, (State n1, State n2) -> {
            // compare n1 and n2
            int man1 = n1.getManhattanDistance() + n1.prev_cost;
            int man2 = n2.getManhattanDistance() + n2.prev_cost;
            if(man1<man2) return -1;
            else if(man1==man2) return 0;
            else return 1;
        });
        pq2.add(new State(startState));
        
        pq3 = new PriorityQueue<>(1, (State n1, State n2) -> {
            // compare n1 and n2
            int lin1 = n1.getLinearConflictDistance() + n1.prev_cost;
            int lin2 = n2.getLinearConflictDistance() + n2.prev_cost;
            if(lin1<lin2) return -1;
            else if(lin1==lin2) return 0;
            else return 1;
        });
        pq3.add(new State(startState));
    }
    
    public State solveUsingHamming(){
        closedList.clear();
        openList.clear();
        Matrix matrix = new Matrix(startStateOfSolver.k, startStateOfSolver.map);
        openList.put(matrix, 0);
        while(!pq1.isEmpty()){
            State front = pq1.remove();
            if(front.getHammingDistance()==0){
                //everybody in right place
                return front;
            }
            closedList.add(front);
            ArrayList<State> children = front.getChildren();
            for(State child: children){
                if(!closedList.contains(child)){
                    //closed list doesnt contain this child - so proceed to open list insertion
                    Matrix matrix_new = new Matrix(child.k, child.map);
                    if(openList.get(matrix_new)==null){
                        //not found in openList too - so insertion
                        pq1.add(child);
                        openList.put(matrix_new, child.prev_cost);
                    }
                    else{
                        int cost = openList.get(matrix_new);
                        if(cost > child.prev_cost){
                            //this child provides same config with less moves
                            //so remove temp from queue and hashmap
                            //and insert child to both
                            openList.replace(matrix, child.prev_cost);
                            pq1.remove(child);
                            pq1.add(child);
                        }
                    }
                }
            }
        }
        return null;
    }
    
    public State solveUsingManhattan(){
        closedList.clear();
        openList.clear();
        Matrix matrix = new Matrix(startStateOfSolver.k, startStateOfSolver.map);
        openList.put(matrix, 0);
        while(!pq2.isEmpty()){
            State front = pq2.remove();
            if(front.getHammingDistance()==0){
                //everybody in right place
                return front;
            }
            closedList.add(front);
            ArrayList<State> children = front.getChildren();
            for(State child: children){
                if(!closedList.contains(child)){
                    //closed list doesnt contain this child - so proceed to open list insertion
                    Matrix matrix_new = new Matrix(child.k, child.map);
                    if(openList.get(matrix_new)==null){
                        //not found in openList too - so insertion
                        pq2.add(child);
                        openList.put(matrix_new, child.prev_cost);
                    }
                    else{
                        int cost = openList.get(matrix_new);
                        if(cost > child.prev_cost){
                            //this child provides same config with less moves
                            //so remove temp from queue and hashmap
                            //and insert child to both
                            openList.replace(matrix, child.prev_cost);
                            pq2.remove(child);
                            pq2.add(child);
                        }
                    }
                }
            }
        }
        return null;
    }
    
    public State solveUsingLinearConflict(){
        closedList.clear();
        openList.clear();
        Matrix matrix = new Matrix(startStateOfSolver.k, startStateOfSolver.map);
        openList.put(matrix, 0);
        while(!pq3.isEmpty()){
            State front = pq3.remove();
            if(front.getHammingDistance()==0){
                //everybody in right place
                return front;
            }
            closedList.add(front);
            ArrayList<State> children = front.getChildren();
            for(State child: children){
                if(!closedList.contains(child)){
                    //closed list doesnt contain this child - so proceed to open list insertion
                    Matrix matrix_new = new Matrix(child.k, child.map);
                    if(openList.get(matrix_new)==null){
                        //not found in openList too - so insertion
                        pq3.add(child);
                        openList.put(matrix_new, child.prev_cost);
                    }
                    else{
                        int cost = openList.get(matrix_new);
                        if(cost > child.prev_cost){
                            //this child provides same config with less moves
                            //so remove temp from queue and hashmap
                            //and insert child to both
                            openList.replace(matrix, child.prev_cost);
                            pq3.remove(child);
                            pq3.add(child);
                        }
                    }
                }
            }
        }
        return null;
    }
    
    //utility
    public int printSolution(State state){
        if(state.prev_node==null){
            int x = 0;
            System.out.println("\nInitial board:");
            System.out.println(state);
            return x;
        }
        int x = printSolution(state.prev_node) + 1;
        System.out.println("Step " + x + ": " + state.edge);
        System.out.println(state);
        return x;
    }
}
