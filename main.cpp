#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <array>
#include <map>
#include <set>
#include <iomanip>

using namespace std;

void read(array<map<string, int>, 2> &dicts, int &numSpam, int &numHam){
    ifstream in_file;
    in_file.open("training_set.txt");
    string aux;
    while(!in_file.eof()){
        set<string> thisSMS;
        int ind=0;
        getline(in_file, aux);
        if(aux[0]=='h'){
            ind=1;
            numHam++;
        }
        else numSpam++;
        if(aux.size()>=5){
            string::iterator it=aux.begin();
            it++;
            it++;
            string::iterator lastIt=aux.end();
            while(it!=aux.end()){
                if(isalpha(*it)){
                    *it=tolower(*it);
                    if(lastIt==aux.end()) lastIt=it;
                }
                else{
                    if(lastIt!=aux.end()){
                        int sz=it-lastIt;
                        if(sz>=3){
                            int pos=lastIt-aux.begin();
                            string aux2=aux.substr(pos, sz);
                            pair<set<string>::iterator, bool> inSet=thisSMS.insert(aux2);
                            if(inSet.second){
                                pair<map<string, int>::iterator, bool> ins;
                                ins=dicts[ind].insert(pair<string, int>(aux2, 1));
                                if(!ins.second) ins.first->second++;
                            }
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
                    pair<set<string>::iterator, bool> inSet=thisSMS.insert(aux2);
                    if(inSet.second){
                        pair<map<string, int>::iterator, bool> ins;
                        ins=dicts[ind].insert(pair<string, int>(aux2, 1));
                        if(!ins.second) ins.first->second++;
                    }
                }
                lastIt=aux.end();
            }
        }
    }
    in_file.close();
}

double spamGivenWords(double &probSpamWords, double &probWords, double &probSpam){
    double aux=probSpamWords;
    aux*=probSpam;
    aux/=probWords;
    return aux;
}

void test(map<string, pair<int, int> > &inventario, int &numSpam, int &numHam, int &numTot, double &probSpam, int &numSpamTest, int &numHamTest, int &numSpamFinal, int &numHamFinal){
    ifstream in_file;
    in_file.open("test_set.txt");
    string aux;
    int cont=0;
    while(!in_file.eof()){
        cont++;
        set<string> thisSMS;
        int ind=0;
        getline(in_file, aux);
        if(aux[0]=='h') numHamTest++;
        else numSpamTest++;
        if(aux.size()>=5){
            string::iterator it=aux.begin();
            it++;
            it++;
            string::iterator lastIt=aux.end();
            while(it!=aux.end()){
                if(isalpha(*it)){
                    *it=tolower(*it);
                    if(lastIt==aux.end()) lastIt=it;
                }
                else{
                    if(lastIt!=aux.end()){
                        int sz=it-lastIt;
                        if(sz>=3){
                            int pos=lastIt-aux.begin();
                            string aux2=aux.substr(pos, sz);
                            thisSMS.insert(aux2);
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
                    map<string, pair<int, int> >::iterator itFind=inventario.find(aux2);
                    thisSMS.insert(aux2);
                }
                lastIt=aux.end();
            }
        }
        double probSpamWords=1, probHamWords=1, probWords=1;
        map<string, pair<int, int> >::iterator itDict=inventario.begin();
        set<string>::iterator itSet=thisSMS.begin();
        while(itDict!=inventario.end() && itSet!=thisSMS.end()){
            while(itDict!=inventario.end() && itSet!=thisSMS.end() && itDict->first<*itSet){
                double auxD=itDict->second.first;
                auxD/=(double)numSpam;
                auxD=1-auxD;
                probSpamWords*=auxD;
                auxD=itDict->second.second;
                auxD/=(double)numHam;
                auxD=1-auxD;
                probHamWords*=auxD;
                itDict++;
            }
            while(itDict!=inventario.end() && itSet!=thisSMS.end() && itDict->first>*itSet)itSet++;
            while(itDict!=inventario.end() && itSet!=thisSMS.end() && itDict->first==*itSet){
                double auxD=itDict->second.first;
                auxD/=(double)numSpam;
                probSpamWords*=auxD;
                auxD=itDict->second.second;
                auxD/=(double)numHam;
                probHamWords*=auxD;
                itSet++;
                itDict++;
            }
        }
        while(itDict!=inventario.end()){
            double auxD=itDict->second.first;
            auxD/=(double)numSpam;
            auxD=1-auxD;
            probSpamWords*=auxD;
            auxD=itDict->second.second;
            auxD/=(double)numHam;
            auxD=1-auxD;
            probHamWords*=auxD;
            itDict++;
        }
        probWords=probSpamWords*probSpam;
        double auxD=probHamWords*(1.0-probSpam);
        probWords+=auxD;
        double ans=spamGivenWords(probSpamWords, probWords, probSpam);
        cout << "La probabilidad de que el mensaje " << cont << " sea spam, dado su contenido, es :\n\t"<< fixed << setprecision(20) << ans;
        if(ans>0.5) cout << ". Es spam.\n";
        else cout << ". No es spam\n";
        if(ans>0.5 && aux[0]=='s') numSpamFinal++;
        if(ans<=0.5 && aux[0]=='h') numHamFinal++;
    }
    in_file.close();
}

int main(){
    array<map<string, int>, 2> dicts;
    int numSpam=0, numHam=0, numTot;
    read(dicts, numSpam, numHam);
    numTot=numSpam+numHam;
    ofstream hist;
    hist.open("Histograma.dat", ofstream::trunc);
    hist << "Palabras Spam No_spam Total\n";
    ofstream inter;
    inter.open("Inter.dat", ofstream::trunc);
    inter << "Palabras Spam No_spam Total\n";
    array<int, 2> sum={0};
    array<int, 2> sumT={0};
    map<string, int>::iterator spamIt=dicts[0].begin(), hamIt=dicts[1].begin();
    map<string, pair<int, int> > inventario;
    while(spamIt!=dicts[0].end() && hamIt!=dicts[1].end()){
        while(spamIt!=dicts[0].end() && spamIt->first<hamIt->first){
            hist<< spamIt->first << " " << spamIt->second << " " << 0 << " " << spamIt->second<< "\n";
            sumT[0]+=spamIt->second;
            spamIt++;
        }
        while(spamIt!=dicts[0].end() && hamIt!=dicts[1].end() && spamIt->first > hamIt->first){
            hist<< hamIt->first << " " << 0 << " "  << hamIt->second << " "  <<  hamIt->second << "\n";
            sumT[1]+=spamIt->second;
            hamIt++;
        }
        if(spamIt!=dicts[0].end() && hamIt!=dicts[1].end() && spamIt->first==hamIt->first){
            pair<string, pair<int, int> > auxp;
            auxp.first=spamIt->first;
            auxp.second.first=spamIt->second;
            auxp.second.second=hamIt->second;
            inventario.insert(auxp);
            sum[0]+=auxp.second.first;
            sumT[0]+=auxp.second.first;
            sum[1]+=auxp.second.second;
            sumT[1]+=auxp.second.second;
            hist<< auxp.first << " " << auxp.second.first << " " << auxp.second.second << " " << auxp.second.first+auxp.second.second << "\n";
            inter << auxp.first << " " << auxp.second.first << " " << auxp.second.second << " " << auxp.second.first+auxp.second.second << "\n";
            spamIt++;
            hamIt++;
        }
    }

    while(spamIt!=dicts[0].end()){
        hist<< spamIt->first << " " << spamIt->second << " " << 0 << " " << spamIt->second<< "\n";
        sumT[0]+=spamIt->second;
        spamIt++;
    }
    while(hamIt!=dicts[1].end()){
        hist<< hamIt->first << " " << 0 << " "  << hamIt->second << " "  <<  hamIt->second << "\n";
        sumT[1]+=hamIt->second;
        hamIt++;
    }

    inter << "Total " << sum[0] <<  " " << sum[1] << " "  << sum[0]+sum[1] << "\n";
    hist << "Total " << sumT[0] <<  " " << sumT[1] << " "  << sumT[0]+sumT[1] << "\n";
    hist.close();
    inter.close();

    double probSpam=numSpam;
    probSpam/=(double) numTot;
    int numSpamTest=0, numHamTest=0, numSpamFinal=0, numHamFinal=0;

    test(inventario, numSpam, numHam, numTot, probSpam, numSpamTest, numHamTest, numSpamFinal, numHamFinal);

    double spamAc=(double)numSpamFinal/(double)numSpamTest;
    double hamAc=(double)numHamFinal/(double)numHamTest;
    cout << "La precision para decir que un mensaje es spam es de "<< spamAc<<"\n";
    cout << "La precision para decir que un mensaje no spam no es spam es de "<< hamAc<<"\n";

    return 0;
}
