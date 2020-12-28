#include <bits/stdc++.h>
using namespace std;

#define INF 1e9
#define MAX_NODES 100000
#define pii pair<int,int>
#define PRINTALL 0

int number_of_cities;
struct Node{
  double x;
  double y;
  Node(double xx, double yy){
    this->x = xx;
    this->y = yy;
  }
  Node(){}
} typedef node;

double getDistance(node a, node b){
  return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

vector<node> nodes;
vector<int> visited;
vector<int> solution; //keps indices in order of solution
vector<int> bestSolution;

void printNodes();
double printSolution();
void printBestSolution();
void initAll();
double calcSolnDistance(vector<int> v);

int getNearestUnvisited(int curr);

//return the distance
double SolveUsingNearestNeighbor(int start);
double SolveUsingNearestNeighbor_randomized(int start, int options_no);

int selectStartNode(int random);

//return the distance
double SolveUsingSavings(int randomStart);
double SolveUsingSavings_randomized(int startNode, int k);

//return the distance
double SolveUsingTwoOpt(vector<int> tempSoln);
double SolveUsingTwoOpt_bestImprov(vector<int> tempSoln);

int main(int argc, char const *argv[]) {
  cout<<setprecision(20);
  freopen("input.txt", "r", stdin);
  //printf("Number of cities(n): ");
  scanf("%d", &number_of_cities);
  double xx, yy;
  int dummy;
  for(int i = 0; i<number_of_cities; i++){
    scanf("%d %lf %lf", &dummy , &xx, &yy);
    node nd(xx, yy);
    nodes.push_back(nd);
    visited.push_back(0);
  }
  /* //test functions - ok
  //for(int i = 0; i<number_of_cities; i++){
    SolveUsingNearestNeighbor(rand()%number_of_cities);  //starting with a random city. or can iterate over all
  //}
  SolveUsingSavings(1); //selects random start
  cout<<"not rand"<<endl;
  SolveUsingSavings(0); // selects better start

  //cout<<endl<<"---2-opt Heuristic---"<<endl;
  //for(int i = 0; i<number_of_cities; i++){
    SolveUsingTwoOpt(rand()%number_of_cities);
  //}

    */

  int k_times = 5;
  //task 1 - run each of NNH and SH k times with random start 

  //NNH
  double worstDist = -INF, bestDist = +INF, avgDist = 0.0;
  for(int i = 0; i<k_times; i++){
    if(PRINTALL) cout<<"NNH round "<<i+1<<endl;
    double distance = SolveUsingNearestNeighbor(rand()%number_of_cities);
    if(distance>worstDist) worstDist = distance;
    if(distance<bestDist) {
      bestDist = distance;
      bestSolution = solution;
    }
    avgDist += distance;
  }
  avgDist /= k_times;
  cout<<"Greedy NNH:"<<endl;
  cout<<"Best distance: "<< bestDist<<endl;
  cout<<"Avg distance: "<< avgDist<<endl;
  cout<<"Worst distance: "<< worstDist<<endl;
  cout<<"Best Solution by Greedy NNH:"<<endl;
  printBestSolution();
  vector<int> bestSolutionNNH = bestSolution;
  int bestStart_for_NNH_random = bestSolution[0];

  //SH
  worstDist = -INF, bestDist = +INF, avgDist = 0.0;
  for(int i = 0; i<k_times; i++){
    if(PRINTALL) cout<<"SH round "<<i+1<<endl;
    double distance = SolveUsingSavings(1); //randomStart = 1
    if(distance>worstDist) worstDist = distance;
    if(distance<bestDist) {
      bestDist = distance;
      bestSolution = solution;
    }
    avgDist += distance;
  }
  avgDist /= k_times;
  cout<<"Greedy SH:"<<endl;
  cout<<"Best distance: "<< bestDist<<endl;
  cout<<"Avg distance: "<< avgDist<<endl;
  cout<<"Worst distance: "<< worstDist<<endl;
  cout<<"Best Solution by Greedy SH:"<<endl;
  printBestSolution();
  vector<int> bestSolutionSH = bestSolution;
  int bestStart_for_SH_random = bestSolution[0];
  



  //task 2 - randomized
  int run_for_n_times = 10, options_no = 5;
  //NNH RANDOMIZED
  pair<double , vector<int> > bestThree_NNH[3];
  bestThree_NNH[0].first = bestThree_NNH[1].first = bestThree_NNH[2].first = INF;

  worstDist = -INF, bestDist = +INF, avgDist = 0.0;
  for(int i = 0; i<run_for_n_times; i++){
    if(PRINTALL) cout<<"NNH round "<<i+1<<endl;
    double distance = SolveUsingNearestNeighbor_randomized(bestStart_for_NNH_random, options_no);
    if(distance>worstDist) worstDist = distance;
    if(distance<bestDist) {
      bestDist = distance;
      bestSolution = solution;
    }
    //find worst in best three
    int may_remove = -1;
    double distFound = 0.0;
    for(int j = 0; j<3; j++){
      if(bestThree_NNH[j].first > distFound){
        may_remove = j;
        distFound = bestThree_NNH[j].first;
      }
    }
    //if new one is better replace the worst
    if(distance < distFound){
      bestThree_NNH[may_remove].first = distance;
      bestThree_NNH[may_remove].second = solution;
    }
    avgDist += distance;
  }
  avgDist /= run_for_n_times;
  cout<<"Greedy Randomized NNH:"<<endl;
  cout<<"Best distance: "<< bestDist<<endl;
  cout<<"Avg distance: "<< avgDist<<endl;
  cout<<"Worst distance: "<< worstDist<<endl;
  cout<<"\nBest Solution by Greedy Randomized NNH:"<<endl;
  printBestSolution();

  //print best 3 random NNH 
  /*
  cout<<"\nBEST 3 randomized NNH path:"<<endl;
  for(int i = 0; i<3; i++){
    bestSolution = bestThree_NNH[i].second;
    printBestSolution();
    cout<<"------------------"<<endl;
  }
  */

  
  run_for_n_times = 10, options_no = 5;
  //SH RANDOMIZED
  pair<double , vector<int> > bestThree_SH[3];
  bestThree_SH[0].first = bestThree_SH[1].first = bestThree_SH[2].first = INF;

  //SH
  worstDist = -INF, bestDist = +INF, avgDist = 0.0;
  for(int i = 0; i<run_for_n_times; i++){
    if(PRINTALL) cout<<"SH round "<<i+1<<endl;
    double distance = SolveUsingSavings_randomized(bestStart_for_SH_random, options_no);
    if(distance>worstDist) worstDist = distance;
    if(distance<bestDist) {
      bestDist = distance;
      bestSolution = solution;
    }
    //find worst in best three
    int may_remove = -1;
    double distFound = 0.0;
    for(int j = 0; j<3; j++){
      if(bestThree_SH[j].first > distFound){
        may_remove = j;
        distFound = bestThree_SH[j].first;
      }
    }
    //if new one is better replace the worst
    if(distance < distFound){
      bestThree_SH[may_remove].first = distance;
      bestThree_SH[may_remove].second = solution;
    }
    avgDist += distance;
  }
  avgDist /= run_for_n_times;
  cout<<"Greedy Randomized SH:"<<endl;
  cout<<"Best distance: "<< bestDist<<endl;
  cout<<"Avg distance: "<< avgDist<<endl;
  cout<<"Worst distance: "<< worstDist<<endl;
  cout<<"\nBest Solution by Greedy Randomized SH:"<<endl;
  printBestSolution();

  //print best 3 random SH 
  /*
  cout<<"\nBEST 3 randomized SH path:"<<endl;
  for(int i = 0; i<3; i++){
    bestSolution = bestThree_SH[i].second;
    printBestSolution();
    cout<<"------------------"<<endl;
  }
  */

  //2 opt first improv
  //NNH 2-opt first improv on bestSolutionNNH, bestThree_NNH[3]
  worstDist = -INF, bestDist = +INF, avgDist = 0.0;
  worstDist = bestDist = SolveUsingTwoOpt(bestSolutionNNH);
  avgDist += worstDist;
  for(int i = 0; i<3; i++){
    double distance = SolveUsingTwoOpt(bestThree_NNH[i].second);
    if(distance > worstDist) worstDist = distance;
    if(distance < bestDist) bestDist = distance;
    avgDist += distance;
  }
  avgDist /= 4;
  cout<<"\nNNH 2-opt First improvement:"<<endl;
  cout<<"Best distance: "<< bestDist<<endl;
  cout<<"Avg distance: "<< avgDist<<endl;
  cout<<"Worst distance: "<< worstDist<<endl;

  //SH 2-opt first improv on bestSolutionSH, bestThree_SH[3]
  worstDist = -INF, bestDist = +INF, avgDist = 0.0;
  worstDist = bestDist = SolveUsingTwoOpt(bestSolutionSH);
  avgDist += worstDist;
  for(int i = 0; i<3; i++){
    double distance = SolveUsingTwoOpt(bestThree_SH[i].second);
    if(distance > worstDist) worstDist = distance;
    if(distance < bestDist) bestDist = distance;
    avgDist += distance;
  }
  avgDist /= 4;
  cout<<"\nSH 2-opt First improvement:"<<endl;
  cout<<"Best distance: "<< bestDist<<endl;
  cout<<"Avg distance: "<< avgDist<<endl;
  cout<<"Worst distance: "<< worstDist<<endl;


  //2 opt best improv
  //NNH 2-opt best improv on bestSolutionNNH, bestThree_NNH[3]
  worstDist = -INF, bestDist = +INF, avgDist = 0.0;
  worstDist = bestDist = SolveUsingTwoOpt_bestImprov(bestSolutionNNH);
  avgDist += worstDist;
  for(int i = 0; i<3; i++){
    double distance = SolveUsingTwoOpt_bestImprov(bestThree_NNH[i].second);
    if(distance > worstDist) worstDist = distance;
    if(distance < bestDist) bestDist = distance;
    avgDist += distance;
  }
  avgDist /= 4;
  cout<<"\nNNH 2-opt Best improvement:"<<endl;
  cout<<"Best distance: "<< bestDist<<endl;
  cout<<"Avg distance: "<< avgDist<<endl;
  cout<<"Worst distance: "<< worstDist<<endl;

  //SH 2-opt best improv on bestSolutionSH, bestThree_SH[3]
  worstDist = -INF, bestDist = +INF, avgDist = 0.0;
  worstDist = bestDist = SolveUsingTwoOpt_bestImprov(bestSolutionSH);
  avgDist += worstDist;
  for(int i = 0; i<3; i++){
    double distance = SolveUsingTwoOpt_bestImprov(bestThree_SH[i].second);
    if(distance > worstDist) worstDist = distance;
    if(distance < bestDist) bestDist = distance;
    avgDist += distance;
  }
  avgDist /= 4;
  cout<<"\nSH 2-opt Best improvement:"<<endl;
  cout<<"Best distance: "<< bestDist<<endl;
  cout<<"Avg distance: "<< avgDist<<endl;
  cout<<"Worst distance: "<< worstDist<<endl;

  return 0;
}
/*
10
1 2
5 3
4 5
2 7
3 9
8 3
1 9
5 8
6 7
9 2
*/

int getNearestUnvisited(int curr){
  node at = nodes[curr];
  int idx = -1; double dist = INF;
  for(int i = 0; i<number_of_cities; i++){
    if(i!=curr && !visited[i] && dist>getDistance(nodes[curr], nodes[i])){
      idx = i; dist = getDistance(nodes[curr], nodes[i]);
    }
  }
  return idx;
}

double SolveUsingNearestNeighbor(int start){
  int cnt = number_of_cities, curr = start;
  initAll();
  solution.push_back(curr);
  visited[curr] = 1;
  while(--cnt){
    curr = getNearestUnvisited(curr);
    solution.push_back(curr);
    visited[curr] = 1;
  }
  solution.push_back(solution[0]); //closing the loop
  if(PRINTALL) cout<<"Solution using Nearest Neighbor Heuristic: "<<endl;
  return printSolution();
}

int getNearestUnvisited_random(int curr, int k){
  node at = nodes[curr];
  double dist;
  vector< pair<double, int> > temp; //vector of (distance , node no) 
  for(int i = 0; i<number_of_cities; i++){
    if(i!=curr && !visited[i]){
      dist = getDistance(nodes[curr], nodes[i]);
      temp.push_back(make_pair(dist, i));
    }
  }
  sort(temp.begin(), temp.end());
  int sz = temp.size();
  int options = min(sz, k);
  int selected = rand()%options;
  return temp[selected].second;
}

double SolveUsingNearestNeighbor_randomized(int start, int k){
  int cnt = number_of_cities, curr = start;
  initAll();
  solution.push_back(curr);
  visited[curr] = 1;
  while(--cnt){
    curr = getNearestUnvisited_random(curr, k);
    solution.push_back(curr);
    visited[curr] = 1;
  }
  solution.push_back(solution[0]); //closing the loop
  if(PRINTALL) cout<<"Solution using Nearest Neighbor Heuristic RANDOMIZED: "<<endl;
  return printSolution();
}


int selectStartNode(int random){
  if(random==1){
    return rand()%number_of_cities;
  }
  else{
    double d = INF;
    int ret = -1;
    for(int centre = 0; centre<number_of_cities; centre++){
      double dcurr = 0;
      for(int i = 0; i<number_of_cities; i++){
        dcurr += getDistance(nodes[i], nodes[centre]);
      }
      if(dcurr<d){
        ret = centre;
        d = dcurr;
      }
    }
    return ret;
  }
}

//D S U - DSU - needed only for savings heuristics
int p[MAX_NODES];
int Find(int x){
  if(p[x]==x) return x;
  return p[x] = Find(p[x]);
}
void Union(int a, int b){
  p[Find(b)] = Find(a);
}

double SolveUsingSavings(int randomStart){
  initAll();
  int start = selectStartNode(randomStart);
  int n = number_of_cities - 1; //exclude start node
  int cnt[number_of_cities];
  for(int i = 0; i<number_of_cities; i++) cnt[i] = 2;
  cnt[start] = 0;
  int remaining = number_of_cities - 2; //remaining number of external dsu union
  if(PRINTALL) cout<<start<<endl;

  //savings contains all pairs in n
  int savings_sz = (n*(n-1))/2;
  //saving - (node, node)
  vector< pair< double, pii > > savings;
  for(int i = 0; i<number_of_cities; i++){
    if(i!=start){
      for(int j = i+1; j<number_of_cities; j++){
        if(j!=start){
          double sav = getDistance(nodes[i], nodes[start]);
          sav += getDistance(nodes[j], nodes[start]);
          sav -= getDistance(nodes[i], nodes[j]);
          savings.push_back(make_pair(sav, make_pair(i,j)));
        }
      }
    }
  }
  sort(savings.begin(), savings.end());
  reverse(savings.begin(), savings.end());
/*
  cout<<endl<<savings_sz<<endl<<savings.size()<<endl;
  for(int i = 0; i<savings_sz; i++) cout<<savings[i].first<<' ' <<savings[i].second.first<<' '<<savings[i].second.second<< '\n' ;
  cout<<endl;
*/
  for(int i = 0; i<number_of_cities; i++){
    p[i] = i;
  }
  vector< pii > soln_edges;
  for(int i = 0 ; i<savings_sz && remaining>0; i++){
    pair< double , pii > entry = savings[i];
    pii edge = entry.second;
    int u = edge.first, v = edge.second;
    if(cnt[u]>0 && cnt[v]>0){
      //dsu not same set test
      if(Find(u)!=Find(v)){      
        //union
        Union(u,v);
        remaining--;
        cnt[u]--; cnt[v]--;
        soln_edges.push_back(edge);
      }
    }
  }
  int u, found1 = 0;
  for(int i = 0; i<number_of_cities; i++){
    if(cnt[i]==1 && found1==0){
      u = i;
      found1 = 1;
    }
    else if(cnt[i]==1 && found1==1){
      soln_edges.push_back(make_pair(start, i));
      soln_edges.push_back(make_pair(start, u));
      break;
    }
  }
/*
  for(int i = 0; i<soln_edges.size(); i++){
    cout<<soln_edges[i].first<<' '<<soln_edges[i].second<<endl;
  }
*/
  //edge list of solution graph which is a cycle
  int adj[number_of_cities][2], soln_edges_sz = soln_edges.size();
  for(int i = 0; i<number_of_cities; i++){adj[i][0] = adj[i][1] = -1; } // initialize
  for(int i = 0; i<soln_edges_sz; i++){
    int uu = soln_edges[i].first;
    int vv = soln_edges[i].second;
    //cout<<uu<< "    "<< vv<<endl; 
    if(adj[uu][0]==-1){
      adj[uu][0] = vv;
    }
    else{
      adj[uu][1] = vv;
    }
    if(adj[vv][0]==-1){
      adj[vv][0] = uu;
    }
    else{
      adj[vv][1] = uu;
    }
    /*
    cout<<"__________________________________\n";
    for(int kp = 0; kp<number_of_cities ; kp++){
      cout<<adj[kp][0]<< " "<< adj[kp][1]<<endl;
    }
    cout<<"__________________________________";
    */
  }
  int parent = -1, here = start, facing;

  while(true){
    solution.push_back(here);
    if(adj[here][0]==parent) facing = adj[here][1];
    else facing = adj[here][0];
    //std::cout << "hell " <<here <<" "<<facing << '\n';
    if(facing==start) break;
    parent = here;
    here = facing;
  }
  solution.push_back(start);
  if(PRINTALL) cout<<"Solution using Savings Heuristic: "<<endl;
  return printSolution();
}

double SolveUsingSavings_randomized(int startNode, int k){
  initAll();
  int start = startNode;
  int n = number_of_cities - 1; //exclude start node
  int cnt[number_of_cities];
  for(int i = 0; i<number_of_cities; i++) cnt[i] = 2;
  cnt[start] = 0;
  int remaining = number_of_cities - 2; //remaining number of external dsu union
  if(PRINTALL) cout<<start<<endl;

  //savings contains all pairs in n
  int savings_sz = (n*(n-1))/2;
  //saving - (node, node)
  vector< pair< double, pii > > savings;
  for(int i = 0; i<number_of_cities; i++){
    if(i!=start){
      for(int j = i+1; j<number_of_cities; j++){
        if(j!=start){
          double sav = getDistance(nodes[i], nodes[start]);
          sav += getDistance(nodes[j], nodes[start]);
          sav -= getDistance(nodes[i], nodes[j]);
          savings.push_back(make_pair(sav, make_pair(i,j)));
        }
      }
    }
  }
  sort(savings.begin(), savings.end());
  reverse(savings.begin(), savings.end());
/*
  cout<<endl<<savings_sz<<endl<<savings.size()<<endl;
  for(int i = 0; i<savings_sz; i++) cout<<savings[i].first<<' ' <<savings[i].second.first<<' '<<savings[i].second.second<< '\n' ;
  cout<<endl;
*/
  for(int i = 0; i<number_of_cities; i++){
    p[i] = i;
  }
  vector< pii > soln_edges;
  for(int i = 0 ; i<savings_sz && remaining>0; i++){
    //for RANDOMIZATION - select and bring it to the front
    int base_idx = i;
    int end_idx = min(savings_sz-1, base_idx + k - 1);
    int range = end_idx - base_idx + 1;
    int opt = (rand() % range) + base_idx;
    swap(savings[base_idx], savings[opt]);

    pair< double , pii > entry = savings[i];
    pii edge = entry.second;
    int u = edge.first, v = edge.second;
    if(cnt[u]>0 && cnt[v]>0){
      //dsu not same set test
      if(Find(u)!=Find(v)){      
        //union
        Union(u,v);
        remaining--;
        cnt[u]--; cnt[v]--;
        soln_edges.push_back(edge);
      }
    }
  }
  int u, found1 = 0;
  for(int i = 0; i<number_of_cities; i++){
    if(cnt[i]==1 && found1==0){
      u = i;
      found1 = 1;
    }
    else if(cnt[i]==1 && found1==1){
      soln_edges.push_back(make_pair(start, i));
      soln_edges.push_back(make_pair(start, u));
      break;
    }
  }
/*
  for(int i = 0; i<soln_edges.size(); i++){
    cout<<soln_edges[i].first<<' '<<soln_edges[i].second<<endl;
  }
*/
  //edge list of solution graph which is a cycle
  int adj[number_of_cities][2], soln_edges_sz = soln_edges.size();
  for(int i = 0; i<number_of_cities; i++){adj[i][0] = adj[i][1] = -1; } // initialize
  for(int i = 0; i<soln_edges_sz; i++){
    int uu = soln_edges[i].first;
    int vv = soln_edges[i].second;
    //cout<<uu<< "    "<< vv<<endl; 
    if(adj[uu][0]==-1){
      adj[uu][0] = vv;
    }
    else{
      adj[uu][1] = vv;
    }
    if(adj[vv][0]==-1){
      adj[vv][0] = uu;
    }
    else{
      adj[vv][1] = uu;
    }
    /*
    cout<<"__________________________________\n";
    for(int kp = 0; kp<number_of_cities ; kp++){
      cout<<adj[kp][0]<< " "<< adj[kp][1]<<endl;
    }
    cout<<"__________________________________";
    */
  }
  int parent = -1, here = start, facing;

  while(true){
    solution.push_back(here);
    if(adj[here][0]==parent) facing = adj[here][1];
    else facing = adj[here][0];
    //std::cout << "hell " <<here <<" "<<facing << '\n';
    if(facing==start) break;
    parent = here;
    here = facing;
  }
  solution.push_back(start);
  if(PRINTALL) cout<<"Solution using Savings Heuristic RANDOMIZED: "<<endl;
  return printSolution();
}
double SolveUsingTwoOpt(vector<int> tempSoln){
  if(PRINTALL) cout<<"Improving the solution using 2-opt Heuristic..."<<endl;
  solution = tempSoln;
  double currDist = calcSolnDistance(solution);
  bool run_again = true;
  while(run_again){
    run_again = false;
    for(int i = 1; i<(number_of_cities-1); i++){
      for(int j = i+1; j<number_of_cities; j++){
        vector<int> testForBetter = solution;
        //reversing the path
        reverse(testForBetter.begin()+i, testForBetter.begin()+j+1);
        double newdist = calcSolnDistance(testForBetter);
        if(newdist<currDist){
          run_again = true;
          currDist = newdist;
          solution = testForBetter;
          //cout<<i+1<<' '<<j+1<<endl;
          //printSolution();
          testForBetter.clear();
          break;
        }
      }
      if(run_again) break;
    }
  }
  if(PRINTALL) cout<<"Improved solution using 2-opt Heuristic: "<<endl;
  return printSolution();
}

double SolveUsingTwoOpt_bestImprov(vector<int> tempSoln){
  if(PRINTALL) cout<<"Improving the solution using 2-opt Heuristic..."<<endl;
  solution = tempSoln;
  double currDist = calcSolnDistance(solution);
  bool run_again = true;
  while(run_again){
    run_again = false;
    
    //best is now current 
    double bestDist = currDist;
    vector<int> bestSoln = solution;

    for(int i = 1; i<(number_of_cities-1); i++){
      for(int j = i+1; j<number_of_cities; j++){
        vector<int> testForBetter = solution;
        //reversing the path
        reverse(testForBetter.begin()+i, testForBetter.begin()+j+1);
        double newdist = calcSolnDistance(testForBetter);
        //replace best if possible
        if(newdist<bestDist){
          run_again = true;
          bestDist = newdist;
          bestSoln = testForBetter;
          //cout<<i+1<<' '<<j+1<<endl;
          //printSolution();
          testForBetter.clear();
        }
      }
    }
    if(!run_again) break;
    else{
      //replace with the best
      currDist = bestDist;
      solution = bestSoln;  
    }
  }
  if(PRINTALL) cout<<"Improved solution using 2-opt Heuristic: "<<endl;
  return printSolution();
}




//utility
void initAll(){
  solution.clear();
  for(int i = 0; i<number_of_cities; i++) visited[i] = 0;
}
void printNodes(){
  for(int i = 0; i<number_of_cities; i++){
    printf("%d : %lf %lf\n", i+1, nodes[i].x, nodes[i].y);
  }
}
double calcSolnDistance(vector<int> v){
  double dist = 0;
  for(int i = 1; i<=number_of_cities; i++){
    dist += getDistance(nodes[v[i-1]], nodes[v[i]]);
  }
  return dist;
}
double printSolution(){
  double dist = calcSolnDistance(solution);
  if(PRINTALL){
    cout<<"Distance: "<<dist<<endl;
    cout<<"Order:"<<endl;
    for(int i = 0; i<=number_of_cities; i++){
      cout<<solution[i]+1<<" : ("<<nodes[solution[i]].x<<", "<< nodes[solution[i]].y<<")"<<endl;
    }
  }
  return dist;
}

void printBestSolution(){
  double dist = calcSolnDistance(bestSolution);
  cout<<"Distance: "<<dist<<endl;
  cout<<"Order:"<<endl;
  for(int i = 0; i<=number_of_cities; i++){
    cout<<bestSolution[i]+1<<" : ("<<nodes[bestSolution[i]].x<<", "<< nodes[bestSolution[i]].y<<")"<<endl;
  }
}
