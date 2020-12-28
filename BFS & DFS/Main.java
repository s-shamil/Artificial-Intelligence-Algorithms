/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package aioffline_1;

import java.util.Scanner;


/**
 *
 * @author lelmil
 */
public class Main {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        System.out.print("Number of Missionaries: ");
        int m = sc.nextInt();
        System.out.print("Number of Cannibals: ");
        int c = sc.nextInt();
        System.out.print("Boat Capacity: ");
        int cap = sc.nextInt();
        //System.out.println(m + "  " + c + "  " + cap );
        Graph g = new Graph(m, c, cap);
        long startTime, elapsedTime;
        
        System.out.println("BFS Solution:");
        startTime = System.currentTimeMillis();
        State ansBfs = g.bfs();
        elapsedTime = System.currentTimeMillis() - startTime;
        if(null!=ansBfs){
            //System.out.println(ansBfs);
            ansBfs.printSolution();
            System.out.println("\nSolution Steps: " + ansBfs.path.size());
        }
        System.out.println("Expanded Nodes: " + g.closedList.size());
        int exp = (g.closedList.size() + g.stateQueue.size());
        System.out.println("Explored Nodes: " + exp);
        System.out.println("Time taken: " + elapsedTime + "ms");
        
        System.out.println("\n\nDFS Solution:");
        startTime = System.currentTimeMillis();
        State ansDfs = g.dfs();
        elapsedTime = System.currentTimeMillis() - startTime;
        if(ansDfs!=null){
            //System.out.println(ansDfs);
            ansDfs.printSolution();
            System.out.println("\nSolution Steps: " + ansDfs.path.size());
        }
        System.out.println("Expanded Nodes: " + g.closedList.size());
        exp = (g.closedList.size() + g.stateStack.size());
        System.out.println("Explored Nodes: " + exp);
        System.out.println("Time taken: " + elapsedTime + "ms");
    }
}
