/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ai_offline2_new;

import java.io.FileNotFoundException;
import java.io.PrintStream;
import java.util.ArrayList;
import java.util.Scanner;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author lelmil
 */
public class Main {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // TODO code application logic here
        
        ArrayList<State> col = new ArrayList<>();
        Scanner sc = new Scanner(System.in);

        int matrix_size, x00 = 0, y00 = 0;
        System.out.print("Board Size: ");
        matrix_size = sc.nextInt();
        System.out.println("Board:");
        int[][] mat = new int[matrix_size][matrix_size];
        for (int i = 0; i < matrix_size; i++) {
            for (int j = 0; j < matrix_size; j++) {
                int x = sc.nextInt();
                mat[i][j] = x;
                if (x == 0) {
                    x00 = i;
                    y00 = j;
                }
            }
        }
        State s = new State(matrix_size, mat, null, 0, x00, y00);
        //System.out.println(s.getHammingDistance());
        //System.out.println(s.getManhattanDistance());
        //System.out.println(s.countLinearConflicts());
        //System.out.println(s.getLinearConflictDistance());
        //System.out.println(s.isSolvable());
        boolean solvable = s.isSolvable();
        if (solvable) {
            System.out.println("Choose heuristic: 1. Hamming  2. Manhattan  3. Linear Conflict  4. All");
            int op = sc.nextInt();

            Solver solver = new Solver(s);
            
            //comment out this block to get console output
            
            PrintStream fileStream;
            try {
                fileStream = new PrintStream("src/ai_offline2_new/output.txt");
                System.setOut(fileStream);
            } catch (FileNotFoundException ex) {
                Logger.getLogger(AI_offline2_new.class.getName()).log(Level.SEVERE, null, ex);
            }
            
            
            
            if (op == 1 || op == 4) {
                long startTime = System.currentTimeMillis();
                State goal1 = solver.solveUsingHamming();
                System.out.println("\n::::::::::::Using Hamming::::::::::::");
                solver.printSolution(goal1);
                long endTime = System.currentTimeMillis();
                System.out.println("Time Elapsed: " + (endTime - startTime) + " ms");
                System.out.println("Expanded Nodes: " + solver.closedList.size());
                System.out.println("Explored Nodes: " + (solver.closedList.size() + solver.pq1.size()));
                System.out.println(":::::::::::::::::::::::::::::::::::::\n");
            }

            if (op == 2 || op == 4) {
                long startTime = System.currentTimeMillis();
                State goal2 = solver.solveUsingManhattan();
                System.out.println("\n:::::::::::Using Manhattan:::::::::::");
                solver.printSolution(goal2);
                long endTime = System.currentTimeMillis();
                System.out.println("Time Elapsed: " + (endTime - startTime) + " ms");
                System.out.println("Expanded Nodes: " + solver.closedList.size());
                System.out.println("Explored Nodes: " + (solver.closedList.size() + solver.pq2.size()));
                System.out.println(":::::::::::::::::::::::::::::::::::::\n");
            }

            if (op == 3 || op == 4) {
                long startTime = System.currentTimeMillis();
                State goal3 = solver.solveUsingLinearConflict();
                System.out.println("\n::::::::Using Linear Conflict::::::::");
                solver.printSolution(goal3);
                long endTime = System.currentTimeMillis();
                System.out.println("Time Elapsed: " + (endTime - startTime) + " ms");
                System.out.println("Expanded Nodes: " + solver.closedList.size());
                System.out.println("Explored Nodes: " + (solver.closedList.size() + solver.pq3.size()));
                System.out.println(":::::::::::::::::::::::::::::::::::::\n");
            }
            
        } else {
            System.out.println("\nYour board:\n" + s);
            System.out.println("Unsolvable");
        }


        /*
        System.out.println(s);
        if(goal!=null){
            String output = "";
            int steps = 0;
            while(goal.prev_node!=null){
                steps++;
                output = goal.toString() + output;
                goal = goal.prev_node;
            }
            System.out.println(output);
        }
         */
    }

}


/*
test cases

(1) 
1 3 11 14
9 10 15 4
0 5 12 8
6 13 2 7  [44]

(2) 
14 8 12 1
9 6 4 3
7 2 5 11
10 0 13 15 [48]

(3) 
0 8 14 11
9 5 6 15
12 7 2 13
1 3 10 4  [60]

(4) 
12 14 9 2
4 0 15 3
1 10 11 8
13 6 7 5  [48]




(1) 
8 2 0 
5 4 7 
3 6 1 [26]
(2) 
8 2 7 
5 0 6 
4 3 1 [24]
(3) 
1 5 0 
7 6 4 
2 3 8 [22]
(4) 
0 4 1 
5 3 8 
2 7 6 [22]
(5) 
5 6 0 
7 1 4 
3 8 2 [22]




1 0 15 4
5 2 3 8
9 13 14 7
11 6 12 10 [33]
*/
