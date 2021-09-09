#include <bits/stdc++.h>
#include "graph.hpp"

using namespace std; 

typedef struct inforId_t
{
    string url; 
    double curRank;
    double newRank; 
    int numIn; 
    int numOut; 
} inforId;

typedef map<string, inforId> IdMap; 

typedef pair <double, string> rankPair; 

list <string> splitString(string s, string del) { 
    list <string> res;
    int pos = 0; 
    string token; 
    while ((pos = s.find(del)) != string::npos) { 
        token = s.substr(0,pos); 
        res.push_back(token); 
        s.erase(0, pos + del.length()); 
    }
    res.push_back(s); 
    return res; 
}

void read_file(Graph& g, IdMap& ID) { 
    fstream fi("webpages.txt", ios::in); 
    if (fi.fail()) { 
        cout<<"Cannot open file !!!"<<endl; 
        return; 
    }
    int m;
    fi>>m; 
    for(int i=0; i<m; i++) { 
        string url, id; 
        fi>>url>>id; 
        insert_vertex(g,id); 
        ID[id].url = url ; 
        ID[id].curRank = 1; 
        ID[id].newRank = 0; 
        ID[id].numIn = 0; 
        ID[id].numOut = 0; 
    }
    fi.close(); 

    fstream f2("pageConnections.txt", ios::in); 
    if (f2.fail()) { 
        cout<<"Cannot open file !!!!!"<<endl;
        return;  
    }
    int n; 
    f2>>n; 
    for(int i=0; i<=n; i++) {
        string line;  
        getline(f2,line);
        if (line != "") { 
            list <string> splittedId; 
            splittedId = splitString(line," "); 
            string u = splittedId.front(); 
            for(auto x:splittedId) { 
                if (x != u) { 
                    insert_edge(g, u, x, 1); 
                    ID[u].numOut ++; 
                    ID[x].numIn ++; 
                }
            }
        }
    }
    f2.close(); 
    return; 
}

void page_rank(Graph g, IdMap& ID) { 
    for(auto x:g) { 
        for(auto y:g[x.first]) { 
            ID[y.first].newRank += 1.0*ID[x.first].curRank/ID[x.first].numOut; 
        }
    }
    for(auto x:ID) { 
        ID[x.first].curRank = ID[x.first].newRank; 
        ID[x.first].newRank = 0; 
    }
    return; 
}

void find_distance(Graph g, IdMap ID) { 
    string id1, id2; 
    cout<<"Please input 2 id : "; 
    cin>>id1>>id2; 
    cout<<Dijkstra(g,id1,id2); 
    return; 
}

int main() {
    Graph g;
    IdMap ID; 
    int minIn = 1000000, maxOut = 0; 
    int m = 0; 
    int numBot = 0; 
    int numSpamBot = 0; 
    set <rankPair, greater<rankPair>> rankSet; 
    set <rankPair, greater<rankPair>> :: iterator it; 
    int func; 
    do { 
        cout<<"\tMENU"<<endl; 
        cout<<"1. READ FILE AND PRINT DATA"<<endl; 
        cout<<"2. PAGE RANK 1 TIMES"<<endl; 
        cout<<"3. PAGE RANK n TIMES"<<endl; 
        cout<<"4. PRINT SPAM BOTS"<<endl; 
        cout<<"5. FIND DISTANCE BETWEEN 2 WEBPAGE"<<endl; 
        cout<<"6. EXIT"<<endl; 
        cout<<"YOU CHOOSE : "; cin>>func; 
        switch (func)
        {
        case 1:
            read_file(g, ID); 
            for(auto x:ID) { 
                cout<<x.first<<" "<<x.second.url<<endl; 
                minIn = min(minIn, x.second.numIn); 
                maxOut = max(maxOut, x.second.numOut); 
            }
            cout<<"Webs with least InConnections : "<<endl; 
            for(auto x:ID) {     
                if (x.second.numIn == minIn) { 
                    cout<<x.second.url<<endl;
                }
            }
            cout<<"Webs with most OutConnections : "<<endl; 
            for(auto x:ID) { 
                if (x.second.numOut == maxOut) { 
                    cout<<x.second.url<<endl;
                }
            }
            break; 
        case 2: 
            page_rank(g, ID);
            rankSet.clear(); 
            for(auto x:ID) { 
                rankSet.insert(make_pair(x.second.curRank, x.first)); 
            } 
            it = rankSet.begin(); 
            cout<<"Page with best rank is : "<<ID[(*it).second].url<<endl;
            it = rankSet.end(); 
            it--; 
            cout<<"Page with worst rank is : "<<ID[(*it).second].url<<endl; 
            break; 
        case 3: 
            do { 
                cout<<"Please input times(10 <= x <= 50) u want do Page Rank algo : "; 
                cin>>m;
            } while (m<10 || m>50); 
            for(int i=0; i<m; i++) { 
                page_rank(g,ID); 
            } 
            rankSet.clear(); 
            for(auto x:ID) { 
                rankSet.insert(make_pair(x.second.curRank, x.first)); 
            } 
            cout<<"Top 3 page : "<<endl; 
            it = rankSet.begin(); 
            for(int i=0; i<3; i++) { 
                cout<<ID[(*it).second].url<<endl; 
                it++; 
            }
            break; 
        case 4: 
            for(auto x:ID) { 
                if(x.second.numIn > 0 && x.second.numOut == 0) 
                    numBot++; 
                if(x.second.numOut > 0 && x.second.numIn == 0)
                    numSpamBot++; 
            }
            cout<<"Numbers of bot is : "<<numBot<<endl;
            cout<<"Numbers of Spambot is : "<<numSpamBot<<endl;
            break; 
        case 5: 
            find_distance(g, ID); 
            break; 
        default:
            break;
        }
    } while (func != 6);
    return 0;  
}