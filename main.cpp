#include <iostream>
#include <string>
#include <fstream>
#include <map>

using namespace std;
int cont;

void read(map<string, int> &words, bool ind){
    ifstream in_file;
    if(ind) in_file.open("spam_training.txt");
    else in_file.open("ham_training.txt");
    string aux;
    cont=0;
    do{
        in_file >> aux;
        //cout << aux <<"\n";
        cont++;
        if(aux.size()>=3){
            string::iterator it=aux.begin();
            string::iterator lastIt=aux.end();
            while(it!=aux.end()){
                if(isalpha(*it)){
                    //cout << *it << "\n";
                    *it=tolower(*it);
                    if(lastIt==aux.end()) lastIt=it;
                }
                else{
                    if(lastIt!=aux.end()){
                        int sz=it-lastIt;
                        if(sz>=3){
                            int pos=lastIt-aux.begin();
                            string aux2=aux.substr(pos, sz);
                            pair<map<string, int>::iterator, bool> ins;
                            ins=words.insert(pair<string, int>(aux2, 1));
                            if(!ins.second) ins.first->second++;
                        }
                        lastIt=aux.end();
                    }
                }
                it++;
            }
            if(lastIt!=aux.end()){
                int sz=it-lastIt;
                if(sz>=3){
                    int pos=lastIt-aux.begin();
                    string aux2=aux.substr(pos, sz);
                    pair<map<string, int>::iterator, bool> ins;
                    ins=words.insert(pair<string, int>(aux2, 1));
                    if(!ins.second) ins.first->second++;
                    lastIt=aux.end();
                }
            }
        }
    } while(!in_file.eof());
    //cout << cont<<"\n";
}

int main(){
    map<string, int> spamWords, hamWords;
    read(spamWords, true);
    read(hamWords, false);
    cout << spamWords.size()<<"\n";
    for(map<string, int>::iterator it=spamWords.begin(); it!=spamWords.end(); it++) cout << it->first<<"\n";

    return 0;
}
