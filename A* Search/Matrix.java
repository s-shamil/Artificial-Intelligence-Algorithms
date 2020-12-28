/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ai_offline2_new;

import java.util.Arrays;

/**
 *
 * @author lelmil
 */
public class Matrix {
    int[][] mat;
    int sz;
    public Matrix(int k, int[][] m){
        mat = new int[k][k];
        this.sz = k;
        for(int i = 0; i<k; i++){
            System.arraycopy(m[i], 0, mat[i], 0, k);
        }
    }

    @Override
    public int hashCode() {
        int hash = 3;
        hash = 53 * hash + Arrays.deepHashCode(this.mat);
        hash = 53 * hash + this.sz;
        return hash;
    }
    @Override
    public boolean equals(Object o){
        if(o==null) return false;
        else if(!(o instanceof Matrix)) return false;
        Matrix st = (Matrix) o;
        boolean flag = true;
        for(int i = 0; i<this.sz; i++){
            for(int j = 0; j<this.sz; j++){
                if(this.mat[i][j] != st.mat[i][j]){
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
}
