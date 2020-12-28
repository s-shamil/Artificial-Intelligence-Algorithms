/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package aioffline_1;

import java.util.ArrayList;
import java.util.Arrays;

/**
 *
 * @author lelmil
 */
public class State {
    public int num_missionaries;
    public int num_cannibals;
    public int boat_side; // 0 hoile left, 1 hoile right
    
    public int N_m;
    public int N_c;
    
    ArrayList<String> path;
    
    public State(int m, int c, int side, int n_m, int n_c){
        this.num_missionaries = m;
        this.num_cannibals = c;
        this.boat_side = side;
        this.N_m = n_m;
        this.N_c = n_c;
        path = new ArrayList<>();
    }

    public State(int n_m, int n_c) {
        this.num_missionaries = -1;
        this.num_cannibals = -1;
        this.boat_side = -1;
        this.N_m = n_m;
        this.N_c = n_c;
        path = new ArrayList<>();
    }
    @Override
    public boolean equals(Object s){
        if(s==null) return false;
        else if(!(s instanceof State)) return false;
        else{
            return (this.num_missionaries==((State)s).num_missionaries && this.num_cannibals==((State)s).num_cannibals && this.boat_side==((State)s).boat_side);
        }
    }

    @Override
    public int hashCode() {
        int hash = 7;
        hash = 71 * hash + this.num_missionaries;
        hash = 71 * hash + this.num_cannibals;
        hash = 71 * hash + this.boat_side;
        return hash;
    }
    
    //given content and direction of boat, check if valid transition
    public boolean isValid(int m, int c, int dir){
        //dir -> 0 for left to right => for this we need the boat currently at left, similar for dir -> 1
        if(this.boat_side != dir){
            return false;
        }
        if(dir==0){
            int new_miss = (this.num_missionaries-m), new_cann = (this.num_cannibals-c);
            int opashe_miss = this.N_m - new_miss, opashe_cann = this.N_c - new_cann;
            return new_miss>=0 && new_cann>=0 && (new_miss>=new_cann || new_miss==0) && (opashe_miss>=opashe_cann || opashe_miss==0);
        }
        //dir -> 1 for right to left
        else {
            int new_miss = (this.num_missionaries+m), new_cann = (this.num_cannibals+c);
            int opashe_miss = this.N_m - new_miss, opashe_cann = this.N_c - new_cann;
            return opashe_miss>=0 && opashe_cann>=0 && (new_miss>=new_cann || new_miss==0) && (opashe_miss>=opashe_cann || opashe_miss==0);
        }
    }
    
    //for valid transition, get the new state
    public State afterTransition(int m, int c, int dir){
        State tmp = new State(this.N_m, this.N_c);
        if(dir==0){
            tmp.num_missionaries = this.num_missionaries - m;
            tmp.num_cannibals = this.num_cannibals - c;
            tmp.boat_side = 1;
        }
        else{
            tmp.num_missionaries = this.num_missionaries + m;
            tmp.num_cannibals = this.num_cannibals + c;
            tmp.boat_side = 0;
        }
        tmp.path = (ArrayList)this.path.clone();
        String p = "";
        p += (Integer.toString(m) + ":");
        p += (Integer.toString(c) + ":");
        p += (Integer.toString(dir));
        tmp.addPath(p);
        return tmp;
    }
    
    public void addPath(String s){
        this.path.add(s);
    }
    
    public boolean isGoal(){
        return this.num_missionaries==0 && this.num_cannibals == 0 && this.boat_side==1;
    }
    
    public void printPath(){
        System.out.println(this.path);
    }
    
    @Override
    public String toString(){
        String s = this.num_missionaries + " " + this.num_cannibals + " " + this.boat_side + "\n";
        //s += this.path.toString();
        return s;
    }
    
    
    //used for printing solution
    
    public void printSolution(){
        int left_m = this.N_m, left_c = this.N_c, right_m = 0, right_c = 0;
        System.out.println("(M:" + left_m + ", C:" + left_c + ") -------------------- (" + "M:" + right_m + ", C:" + right_c + ")");
        
        for (String s: path) {
            //System.out.println(s);
            String[] arr = s.split(":");
            int m = Integer.parseInt(arr[0]);
            int c = Integer.parseInt(arr[1]);
            int dir = Integer.parseInt(arr[2]);
            if(dir==0){
                left_m -= m;
                left_c -= c;
                right_m += m;
                right_c += c;
                System.out.println("\n       > > > > > M:" + m + ",C:" + c + " > > > > > \n");
            }
            else{
                left_m += m;
                left_c += c;
                right_m -= m;
                right_c -= c;
                System.out.println("\n       < < < < < M:" + m + ",C:" + c + " < < < < < \n");
            }
            System.out.println("(M:" + left_m + ", C:" + left_c + ") -------------------- (" + "M:" + right_m + ", C:" + right_c + ")");
        }
    }
}
