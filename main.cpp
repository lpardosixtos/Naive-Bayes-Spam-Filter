#include <iostream>
#include <string>
#include <fstream>
#include <map>

using namespace std;

void read(map<string, int> &words, bool ind){
    ifstream in_file;
    if(ind) in_file.open(spam_training);
    else in_file.open(ham_training)
    string aux;
    while(!in_file.eof()){
        in_file >> aux;
        if(aux.size()>=3){
            string::iterator it=aux.begin();
            string::iterator lastIt=aux.end();
            while(it!=aux.end()){
                
                if(!isalpha(*it)){
                    tolower(*it);
                    if(lastIt==aux.end()) lastIt=it;
                }
                else{
                    if(lastIt!=aux.end()){
                        int sz=it-lastIt;
                        int pos=lastIt-aux.begin();
                        string aux2=aux.substr(pos, sz);
                        pair<map<string, int>::iterator, bool> ins=words.insert(pair<string, int>(aux2, 1));
                        if(!ins.second) ins.first->second++;

                    }
                }
                it++;
            }
        }
    }





}










int main(){






    return 0;
}
