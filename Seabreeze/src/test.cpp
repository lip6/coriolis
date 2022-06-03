#include <iostream>
#include <cassert>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <map>
#include <vector>

#include "Seabreeze/Node.h"
#include "Seabreeze/Tree.h"

using namespace std;

void build_tree(std::ifstream &file, Tree &t){
  if(file.is_open()){
    string line;
    getline(file, line);
    char RoC;
    int lb1, lb2, vl;           // possible labels of current node (can be parrent - current or current - child), value of R(or C)

    vector<string> infos;	// memoriser of infos taken from line
    // Read file, line by line, get infomations
    while(getline(file, line)){
      stringstream tmp(line);
      string word;
      while(tmp >> word){
        infos.push_back(word);
      }

      // Format is : 
      // infos[0] = RoC
      // infos[1] = lb1 = label of current node
      // infos[2] = lb2 = label of child node
      // infos[3] = vl = value of RoC
			
      char RoC = infos[0].at(0);

      // infos[i>0] has type of string, so we need to turn it into integer using strtol
      lb1 = strtol(infos[1].c_str(), NULL, 10);
      lb2 = strtol(infos[2].c_str(), NULL, 10);
      vl = strtol(infos[3].c_str(), NULL, 10);

      infos.clear();
 
      if(lb1 < 0 || lb2 == 0 || lb2 < -1){
        // No node can have the root as a child
        // A parent node cannot have label < 0 (<0 means -1, which is the ground)
        cout << "Please check your configuration !" << endl;
        cout << RoC << "\t" << lb1 << "\t" << lb2 << "\t" << vl << endl;
        return;
      }
      else if(lb1 >= 0 && lb2 == -1){
        // In this case, the branch is connected to "the ground"
        // lb1 is the label of current node

        // Check if current node is already created
        if(RoC == 'R'){
          cout << "Please check your configuration !" << endl;
          cout << RoC << "\t" << lb1 << "\t" << lb2 << "\t" << vl << endl;
          return;
        }

//----------------------------------------------------------------------------------
//        cout << "Node label : " << lb1 << " " << lb2 << " " << RoC << " " << vl << endl;
//----------------------------------------------------------------------------------

        if(t.get_node(lb1)->label == -1)
          t.get_node(lb1)->label = lb1;

        if(RoC == 'C')
          t.get_node(lb1)->C = vl;
//---------------------------------------------------------------------------------- 
//        cout << "Node " << t.get_node(lb1).label << " -> " << lb2 << " C = " << t.get_node(lb1).C << endl << endl;
//----------------------------------------------------------------------------------
      }
      else if(lb1 >= 0 && lb2 > 0){
        // lb1 is the label of parent node
        // lb2 is the label of current node
//---------------------------------------------------------------------------------- 
//        cout << "Node label : " << lb1 << " " << lb2 << " " << RoC << " " << vl << endl;       
//        cout << "Exist ? " << " n1 : " << t.get_node(lb1).label+1 << " n2 : " << t.get_node(lb2).label+1 << endl; 
//----------------------------------------------------------------------------------

        if(t.get_node(lb1)->label == -1)
          t.get_node(lb1)->label = lb1;

        if(t.get_node(lb2)->label == -1)
          t.get_node(lb2)->label = lb2;

        // Get the nodes 
        if(RoC == 'R')
          t.get_node(lb2)->R = vl;
        else if(RoC == 'C')
          t.get_node(lb2)->C = vl;
//-----------------------------------------------------------------------------------
//        cout << "Node : " << t.get_node(lb1).label << " -> " << t.get_node(lb2).label << " R = " << t.get_node(lb2).R << " C = " << t.get_node(lb2).C << endl;
//-----------------------------------------------------------------------------------
        ((t.get_node(lb1))->Ne).push_back(t.get_node(lb2));
        ((t.get_node(lb2))->Np) = t.get_node(lb1);
//-----------------------------------------------------------------------------------
//        cout << "Check : " << t.get_node(lb2).Np->label << " -> " << t.get_node(lb2).label << " R = " << t.get_node(lb2).R << " C = " << t.get_node(lb2).C << endl;
//-----------------------------------------------------------------------------------
      }
    }
//-----------------------------------------------------------------------------------
//    cout << "Double check !" << endl;
//    for(int i = 1; i < t.get_N(); i++){
//      cout << t.get_node(i).Np->label << " -> " << t.get_node(i).label << " R = " << t.get_node(i).R << " C = " << t.get_node(i).C << endl;
//    }
//-----------------------------------------------------------------------------------
  }
}


int main(int argc, char *argv[]){
  if(argc < 4){
    cout << "Usage   : ./test <file name> <nodes number> <point index>" << endl
         << "Example : " << argv[0] << " data.txt 10 3" << endl;
    return 0;
  }
  cout << "Testing with data from : " << argv[1] << endl;
  ifstream f(argv[1]);
  assert(f.is_open());

  int num = strtol(argv[2], NULL, 10);
  Tree t;
  for(int i = 0; i < num; i++){
    t.new_node();
  }
  build_tree(f, t);

  filebuf fb;
  fb.open ( "output.txt", ios::out );
  ostream fout(&fb);
  t.print(fout); 

  int idx = strtol(argv[3], NULL, 10);
  int time = t.Delay_Elmore(idx);
  cout << "Elmore delay time : " << time << endl;
  fb.close();
  return 0;
}
