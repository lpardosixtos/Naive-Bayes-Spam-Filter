#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <array>
#include <map>
#include <set>
#include <iomanip>

using namespace std;
int cont;

void read(map<string, int> &words, bool ind){
    ifstream in_file;
    if(ind) in_file.open("spam_training.txt");
    else in_file.open("ham_training.txt");
    string aux;
    cont=0;
    while(!in_file.eof()){
        in_file >> aux;
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
    }
    //cout << cont<<"\n";
}

void Test(map<string, pair<int, int> > &inter, set<string> &test, double &probSpam, double &probHam, double &probWords, array<int, 2> &sum){
    ifstream in_file;
    in_file.open("test.txt");
    string aux;
    cont=0;
    while(!in_file.eof()){
        in_file >> aux;
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
                            map<string, pair<int, int> >::iterator itFind=inter.find(aux2);
                            if(itFind!=inter.end()){
                                pair<set<string>::iterator, bool> inSet=test.insert(aux2);
                                if(inSet.second){
                                    double auxD=itFind->second.first;
                                    auxD/=(double) sum[0];
                                    cout << itFind->first << ":\n"<< "\taparece en spam con probabilidad  " << fixed<< setprecision(6) << auxD;
                                    probSpam*=auxD;
                                    auxD=itFind->second.second;
                                    auxD/=(double) sum[1];
                                    probHam*=auxD;
                                    cout << "\n\taparece en no spam con probabilidad "<< fixed << setprecision(6)<< auxD;
                                    auxD=itFind->second.first+itFind->second.second;
                                    double auxSum=sum[0]+sum[1];
                                    auxD/=auxSum;
                                    probWords*=auxD;
                                    cout << "\n\taparece en un texto con probabilidad "<<fixed << setprecision(6)<<auxD<<"\n";
                                    cout << fixed << setprecision(20) << probSpam << " " << probHam << " " << probWords << "\n";
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
                    map<string, pair<int, int> >::iterator itFind=inter.find(aux2);
                    if(itFind!=inter.end()){
                        pair<set<string>::iterator, bool> inSet=test.insert(aux2);
                        if(inSet.second){
                            double auxD=itFind->second.first;
                            auxD/=(double) sum[0];
                            cout << itFind->first << ":\n"<< "\taparece en spam con probabilidad  " << fixed<< setprecision(6) << auxD;
                            probSpam*=auxD;
                            cout << "\n\taparece en no spam con probabilidad "<< fixed << setprecision(6)<< auxD;
                            auxD=itFind->second.second;
                            auxD/=(double) sum[1];
                            probHam*=auxD;
                            auxD=itFind->second.first+itFind->second.second;
                            double auxSum=sum[0]+sum[1];
                            auxD/=auxSum;
                            probWords*=auxD;
                            cout << "\n\taparece en un texto con probabilidad "<<fixed << setprecision(6)<<auxD<<"\n";
                            cout << fixed << setprecision(20) << probSpam << " " << probHam << " " << probWords << "\n";
                        }
                    }
                }
            }
        }
    }
    //cout << cont<<"\n";
}

double spamGivenWords(double &probSpam, double &probHam, double &probWords){
    double aux=probWords-probHam;
    double aux2=probSpam-probHam;
    aux/=aux2;
    aux*=probSpam;
    aux/=probWords;
    return aux;
}

int main(){
    map<string, int> spamWords, hamWords;
    read(spamWords, true);
    read(hamWords, false);
    //cout << spamWords.size()<<"\n";
    //for(map<string, int>::iterator it=spamWords.begin(); it!=spamWords.end(); it++) cout << it->first<<"\n";
    ofstream hist;
    hist.open("Histograma.dat", ofstream::trunc);
    hist << "Palabras Spam No_spam Total\n";
    ofstream inter;
    inter.open("Inter.dat", ofstream::trunc);
    inter << "Palabras Spam No_spam Total\n";
    array<int, 2> sum={0};
    array<int, 2> sumT={0};
    map<string, int>::iterator spamIt=spamWords.begin(), hamIt=hamWords.begin();
    map<string, pair<int, int> > inventario;
    while(spamIt!=spamWords.end() && hamIt!=hamWords.end()){
        while(spamIt!=spamWords.end() && spamIt->first<hamIt->first){
            hist<< spamIt->first << " " << spamIt->second << " " << 0 << " " << spamIt->second<< "\n";
            sumT[0]+=spamIt->second;
            spamIt++;
        }
        while(spamIt!=spamWords.end() && hamIt!=hamWords.end() && spamIt->first > hamIt->first){
            hist<< hamIt->first << " " << 0 << " "  << hamIt->second << " "  <<  hamIt->second << "\n";
            sumT[1]+=spamIt->second;
            hamIt++;
        }
        if(spamIt!=spamWords.end() && hamIt!=hamWords.end() && spamIt->first==hamIt->first){
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

    while(spamIt!=spamWords.end()){
        hist<< spamIt->first << " " << spamIt->second << " " << 0 << " " << spamIt->second<< "\n";
        sumT[0]+=spamIt->second;
        spamIt++;
    }
    while(hamIt!=hamWords.end()){
        hist<< hamIt->first << " " << 0 << " "  << hamIt->second << " "  <<  hamIt->second << "\n";
        sumT[1]+=hamIt->second;
        hamIt++;
    }

    inter << "Total " << sum[0] <<  " " << sum[1] << " "  << sum[0]+sum[1] << "\n";
    hist << "Total " << sumT[0] <<  " " << sumT[1] << " "  << sumT[0]+sumT[1] << "\n";
    hist.close();
    inter.close();
    set<string> testFile;
    double probSpam=1;
    double probHam=1;
    double probWords=1;
    Test(inventario, testFile, probSpam, probHam, probWords, sum);
    //cout << fixed << setprecision(25) << probSpam <<" " << probHam << " " << probWords << "\n";
    double ans=spamGivenWords(probSpam, probHam, probWords);
    cout << "La probabilidad de que el correo de prueba sea spam dado su contenido es:\n\t"<< ans<<"\n";

    return 0;
}
