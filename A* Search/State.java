/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ai_offline2_new;

import java.util.ArrayList;
import java.util.Arrays;

/**
 *
 * @author lelmil
 */
public class State {
    //0 will mean blank space
    int[][] map;
    int x0;
    int y0; //x0, y0 tracks the position of blank tile
    int k;
    int prev_cost;
    State prev_node;
    String edge;
    /*
    State(int sz){
        k = sz;
        map = new int[sz][sz];
    }
    */
    State(int size, int[][] m, State parent, int pathcost, int X0, int Y0){
        this.k = size;
        //this.map = m;
        map = new int[k][k];
        for (int i = 0; i < k; i++) {
            System.arraycopy(m[i], 0, this.map[i], 0, m[i].length);
        }
        prev_node = parent;
        prev_cost = pathcost;
        x0 = X0;
        y0 = Y0;
        edge = "";
    }
    //copy constructor
    State(State origin){
        this.k = origin.k;
        //this.map = m;
        map = new int[k][k];
        for (int i = 0; i < k; i++) {
            System.arraycopy(origin.map[i], 0, this.map[i], 0, k);
        }
        this.prev_node = origin.prev_node;
        this.prev_cost = origin.prev_cost;
        this.x0 = origin.x0;
        this.y0 = origin.y0;
        this.edge = origin.edge;
    }
    
    //count #inversion
    int countInversion(){
        //as array in row major order
        int[] matToArr = new int[this.k * this.k - 1];
        int inv = 0, pos = 0;
        for(int i = 0; i<k; i++){
            for(int j = 0; j<k; j++){
                if(this.map[i][j]!=0){
                    matToArr[pos++] = map[i][j];
                }
            }
        }
        for(int i = 0; i<pos; i++){
            for(int j = i+1; j<pos; j++){
                if(matToArr[i]>matToArr[j]){
                    inv++;
                }
            }
        }
        return inv;
    }
    //check if solvable
    boolean isSolvable(){
        if(this.k%2==1){
            return this.countInversion()%2 != 1;
        }
        else{
            return (this.countInversion()+this.x0)%2 != 0;
        }
    }
    
    ArrayList<State> getChildren(){
        ArrayList<State> children = new ArrayList<>();
        int[][] m = new int[k][k];
        for (int i = 0; i < k; i++) {
            System.arraycopy(this.map[i], 0, m[i], 0, this.map[i].length);
        }
        //left transition of blank tile
        if(y0>0){
            int element = this.map[x0][y0-1];
            m[x0][y0] = element;
            m[x0][y0-1] = 0;
            State tmp = new State(this.k, m, this, this.prev_cost+1, x0, y0-1);
            tmp.edge = "MOVE " + Integer.toString(element) + " RIGHT";
            m[x0][y0] = 0;
            m[x0][y0-1] = element;
            children.add(tmp);
        }
        //right transition of blank tile
        if(y0<k-1){
            int element = this.map[x0][y0+1];
            m[x0][y0] = element;
            m[x0][y0+1] = 0;
            State tmp = new State(this.k, m, this, this.prev_cost+1, x0, y0+1);
            tmp.edge = "MOVE " + Integer.toString(element) + " LEFT";
            m[x0][y0] = 0;
            m[x0][y0+1] = element;
            children.add(tmp);
        }
        //up transition of blank tile
        if(x0>0){
            int element = this.map[x0-1][y0];
            m[x0][y0] = element;
            m[x0-1][y0] = 0;
            State tmp = new State(this.k, m, this, this.prev_cost+1, x0-1, y0);
            tmp.edge = "MOVE " + Integer.toString(element) + " DOWN";
            m[x0][y0] = 0;
            m[x0-1][y0] = element;
            children.add(tmp);
        }
        //down transition of blank tile
        if(x0<k-1){
            int element = this.map[x0+1][y0];
            m[x0][y0] = element;
            m[x0+1][y0] = 0;
            State tmp = new State(this.k, m, this, this.prev_cost+1, x0+1, y0);
            tmp.edge = "MOVE " + Integer.toString(element) + " UP";
            m[x0][y0] = 0;
            m[x0+1][y0] = element;
            children.add(tmp);
        }
        return children;
    }
    //heuristic 1
    int getHammingDistance(){
        int expected, dist = 0;
        for(int i = 0; i<k; i++){
            for(int j = 0; j<k; j++){
                if(i==k-1 && j==k-1) expected = 0;
                else expected = j + 1 + i*k;
                int found = this.map[i][j];
                if(found!=0 && found!=expected){
                    dist++;
                }
            }
        }
        return dist;
    }
    
    //heuristic 2
    int getManhattanDistance(){
        int expectedRow, expectedCol, rowDist, colDist, dist = 0;
        for(int i = 0; i<k; i++){
            for(int j = 0; j<k; j++){
                int num = this.map[i][j];
                if(num!=0){
                    expectedRow = (num-1)/this.k;
                    expectedCol = (num-1)%this.k;
                    rowDist = Math.abs(expectedRow-i);
                    colDist = Math.abs(expectedCol-j);
                    dist += (rowDist+colDist);
                }
            }
        }
        return dist;
    }
    
    //heuristic 3
    int countLinearConflicts(){
        int conflict = 0;
        int[] line = new int[k];
        //row wise iteration
        for(int i = 0; i<this.k; i++){
            //taking row wise - insert into line those who belong to this row
            int pos = 0, element;
            for(int j = 0; j<this.k; j++){
                element = this.map[i][j];
                if(element!=0){
                    int belongsToRow = (element-1)/this.k;
                    if(belongsToRow==i){
                        //belongs to this row
                        line[pos++] = element;
                    }
                }
            }
            
            //counting inversions now
            int inv = 0;
            for(int j = 0; j<pos; j++){
                for(int jj = j+1; jj<pos; jj++){
                    if(line[j]>line[jj]){
                        inv++;
                    }
                }
            }
            
            conflict += inv;
        }
        
        //col wise iteration
        for(int i = 0; i<this.k; i++){
            //taking column wise - insert into line those who belong to this column
            int pos = 0, element;
            for(int j = 0; j<this.k; j++){
                element = this.map[j][i];
                if(element!=0){
                    int belongsToCol = (element-1)%this.k;
                    if(belongsToCol==i){
                        //belongs to this col
                        line[pos++] = element;
                    }
                }
            }
            
            //counting inversions now
            int inv = 0;
            for(int j = 0; j<pos; j++){
                for(int jj = j+1; jj<pos; jj++){
                    if(line[j]>line[jj]){
                        inv++;
                    }
                }
            }
            
            conflict += inv;
        }
        return conflict;
    }
    int getLinearConflictDistance(){
        return this.getManhattanDistance() + 2 * this.countLinearConflicts();
    }
    
    @Override
    public String toString(){
        String s = "";
        for(int i = 0; i<k; i++){
            for(int j = 0; j<k; j++){
                if(map[i][j]==0) s += (" " + " "); 
                else s += (map[i][j] + " ");
            }
            s += "\n";
        }
        //return s+"(" +x0 + "," + y0 + ")" + "\n";
        return s;
    }
    @Override
    public boolean equals(Object o){
        if(o==null) return false;
        else if(!(o instanceof State)) return false;
        State st = (State) o;
        boolean flag = true;
        for(int i = 0; i<k; i++){
            for(int j = 0; j<k; j++){
                if(this.map[i][j] != st.map[i][j]){
                    flag = false;
                    break;
                }
            }
            if(!flag){
                break;
            }
        }
        return flag;
    }

    @Override
    public int hashCode() {
        int hash = 7;
        hash = 41 * hash + Arrays.deepHashCode(this.map);
        hash = 41 * hash + this.k;
        return hash;
    }
    
    
    
    
    /*extra functions*/
    /*
    void setMap(){
        Scanner sc = new Scanner(System.in);
        for(int i = 0; i<k; i++){
            for(int j = 0; j<k; j++){
                int x = sc.nextInt();
                map[i][j] = x;
                if(x==0){
                    x0 = i;
                    y0 = j;
                }
            }
        }
    }
    
    void showMap(){
        for(int i = 0; i<k; i++){
            for(int j = 0; j<k; j++){
                System.out.print(map[i][j] + " ");
            }
            System.out.println("");
        }
    }
    */
}
