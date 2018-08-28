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
    //cout << cont<<"\n";
}

double spamGivenWords(double &probSpamWords, double &probWords, double &probSpam){
    double aux=probSpamWords;
    aux*=probSpam;
    aux/=probWords;
    return aux;
}

void test(map<string, pair<int, int> > &inventario, array<int, 2> &sum, double &probSpam, int &numSpamTest, int &numHamTest, int &numSpamFinal, int &numHamFinal){
    ifstream in_file;
    in_file.open("prueba.txt");
    string aux;
    while(!in_file.eof()){
        set<string> thisSMS;
        int ind=0;
        getline(in_file, aux);
        if(aux[0]=='h') numHamTest++;
        else numSpamTest++;
        double probSpamWords=1, probWords=1;
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
                            cout << aux2 <<"\n";
                            pair<set<string>::iterator, bool> inSet=thisSMS.insert(aux2);
                            if(inSet.second){
                                map<string, pair<int, int> >::iterator itFind=inventario.find(aux2);
                                if(itFind!=inventario.end()){
                                    double auxD=itFind->second.first;
                                    cout << auxD << " "  << sum[0] << " spam\n";
                                    auxD/=(double) sum[0];
                                    probSpamWords*=auxD;
                                    auxD=itFind->second.second+itFind->second.first;
                                    cout << auxD << " " << sum[0]+sum[1] << " total\n";
                                    double auxSum=sum[0]+sum[1];
                                    auxD/=auxSum;
                                    probWords*=auxD;
                                }
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
                    cout << aux2 <<"\n";
                    pair<set<string>::iterator, bool> inSet=thisSMS.insert(aux2);
                    if(inSet.second){
                        map<string, pair<int, int> >::iterator itFind=inventario.find(aux2);
                        if(itFind!=inventario.end()){
                            double auxD=itFind->second.first;
                            cout << auxD << " "  << cout << sum[0] << " spam\n";
                            auxD/=(double) sum[0];
                            probSpamWords*=auxD;
                            auxD=itFind->second.second+itFind->second.first;
                            cout << auxD << " " << cout << sum[0]+sum[1] << " total\n";
                            double auxSum=sum[0]+sum[1];
                            auxD/=auxSum;
                            probWords*=auxD;
                        }
                    }
                }
                lastIt=aux.end();
            }
        }
        cout << fixed << setprecision(40) << probSpamWords << " "  << probWords<< " "  << probSpam << "\n";
        double ans=spamGivenWords(probSpamWords, probWords, probSpam);
        cout << "La probabilidad de que este mensaje sea spam es :\t"<< fixed << setprecision(20) << ans <<"\n";
        if(ans>0.5 && aux[0]=='s') numSpamFinal++;
        if(ans<=0.5 && aux[0]=='h') numHamFinal++;
    }
    in_file.close();
    //cout << cont<<"\n";
}

int main(){
    array<map<string, int>, 2> dicts;
    int numSpam=0, numHam=0, numTot;
    read(dicts, numSpam, numHam);
    numTot=numSpam+numHam;
    cout <<numSpam << " " << numHam << "\n";
    //cout << dicts[0].size()<<"\n";
    //for(map<string, int>::iterator it=dicts[0].begin(); it!=dicts[0].end(); it++) cout << it->first<<" " << it->second << "\n";
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

    test(inventario, sum, probSpam, numSpamTest, numHamTest, numSpamFinal, numHamFinal);
    ///cout << fixed << setprecision(25) << probSpamWords <<" " << probHamWords << " " << probWords << "\n";
    //double ans=spamGivenWords(probSpam, probHam, probWords);
    //cout << "La probabilidad de que el correo de prueba sea spam dado su contenido es:\n\t"<< ans<<"\n";

    double spamAc=(double)numSpamFinal/(double)numSpamTest;
    double hamAc=(double)numSpamFinal/(double)numSpamTest;
    cout << "La precision para spam es "<< spamAc<<"\n";
    cout << "La precision para no spam es "<< hamAc<<"\n";

    return 0;
}
