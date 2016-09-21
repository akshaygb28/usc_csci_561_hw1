#include <cstdio>
#include <iostream>
#include <string>
#include <cstring>
#include <map>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

char start_state[100];
char goal_state[100];

int live_traffic_lines;
int sunday_traffic_lines;
int *sunday_traffic_time;

vector<vector<int> > graph;
map <string, int> location;
map <int, string> location_inv;
map <pair<int, int>, int> travel_time;

FILE *in, *out;

void bfs(int, int);
void dfs(int, int);
void ucs(int, int);
void a_star(int, int);

int main(int argc, char const *argv[])
{
    in = fopen("input.txt", "r");
    out= fopen("output.txt", "w");
    if(!in || !out)
    {
        printf("File open failed!! \n");
        return 0;
    }
    char search[5], from[100], to[100], place[100];
    int time, location_count = 0;

    fscanf(in, "%s", search);
    fscanf(in, "%s", start_state);
    fscanf(in, "%s", goal_state);
    fscanf(in, "%d", &live_traffic_lines);

    graph.resize(live_traffic_lines);

    for(int i = 0; i < live_traffic_lines; i++)
    {
        fscanf(in, "%s %s %d", from, to, &time);
        //printf("%s %s %d \n", from, to, time);
        if(location.count(from) == 0)
        {
            location.insert(make_pair(string(from), location_count));
            location_inv.insert(make_pair(location_count, string(from)));
            location_count++;
        }
        if(location.count(to) == 0)
        {
            location.insert(make_pair(string(to), location_count));
            location_inv.insert(make_pair(location_count, string(to)));
            location_count++;
        }
        //cout<<"-->"<<location[from]<<" "<<location[to]<<endl;
        graph[location[from]].push_back(location[to]);
        travel_time.insert(make_pair(make_pair(location[from], location[to]), time));
    }
    fscanf(in, "%d", &sunday_traffic_lines);
    sunday_traffic_time = (int *)malloc(sunday_traffic_lines * sizeof(int));
    for(int i = 0; i < sunday_traffic_lines; i++)
    {
        fscanf(in, "%s %d", place, &time);
        sunday_traffic_time[location[place]] = time;
    }

    if(!strcmp(search, "BFS"))
    {
        bfs(location[string(start_state)], location[string(goal_state)]);
    }
    else if(!strcmp(search, "DFS"))
    {
        dfs(location[string(start_state)], location[string(goal_state)]);
    }
    else if(!strcmp(search, "UCS"))
    {
        ucs(location[string(start_state)], location[string(goal_state)]);
    }
    else if(!strcmp(search, "A*"))
    {
        a_star(location[string(start_state)], location[string(goal_state)]);
    }

    fclose(in);
    fclose(out);
    return 0;
}

void bfs(int source, int destination)
{
    cout<<endl<<"BFS : "<<location_inv[source]<<" -> "<<location_inv[destination]<<" ["<<source<<" -> "<<destination<<"]"<<endl;
    vector <int> tpath;
    queue <vector <int> > tq;
    int visited[sunday_traffic_lines], path_last_node;

    for(int k = 0; k < sunday_traffic_lines; k++)
    {
        visited[k] = 0;
    }
    visited[source] = 1;
    tpath.push_back(source);
    tq.push(tpath);
    while(!tq.empty())
    {
        tpath = tq.front();
        tq.pop();

        path_last_node = tpath[tpath.size() - 1];
        if(path_last_node == destination)
        {
            cout<<"Required path : \n";
            for (int i = 0; i < tpath.size(); i++)
            {
                printf("%s %d\n", (char *)location_inv[tpath[i]].c_str(), i);
                fprintf(out, "%s %d\n", (char *)location_inv[tpath[i]].c_str(), i);
            }
            return;
        }

        for(int i = 0; i < graph[path_last_node].size(); i++)
        {
            //cout<<">>> "<<path_last_node<<" - "<<graph[path_last_node][i]<<" "<<visited[graph[path_last_node][i]]<<endl;
            if(!visited[graph[path_last_node][i]] && find(tpath.begin(), tpath.end(), graph[path_last_node][i]) == tpath.end())
            {
                //cout<<"visited : "<<graph[path_last_node][i]<<endl;
                vector <int> new_path(tpath.begin(), tpath.end());
                new_path.push_back(graph[path_last_node][i]);
                tq.push(new_path);
                visited[graph[path_last_node][i]] = 1;
            }
        }
    }
}

void dfs(int source, int destination)
{
    cout<<endl<<"DFS : "<<location_inv[source]<<" -> "<<location_inv[destination]<<" ["<<source<<" -> "<<destination<<"]"<<endl;
    vector <int> tpath;
    deque <vector <int> > tq;
    int visited[sunday_traffic_lines], path_last_node;

    for(int k = 0; k < sunday_traffic_lines; k++)
    {
        visited[k] = 0;
    }
    visited[source] = 1;
    tpath.push_back(source);
    tq.push_back(tpath);
    while(!tq.empty())
    {
        tpath = tq.front();
        tq.pop_front();

        path_last_node = tpath[tpath.size() - 1];
        if(path_last_node == destination)
        {
            cout<<"Required path : \n";
            for (int i = 0; i < tpath.size(); i++)
            {
                printf("%s %d\n", (char *)location_inv[tpath[i]].c_str(), i);
                fprintf(out, "%s %d\n", (char *)location_inv[tpath[i]].c_str(), i);
            }
            return;
        }

        deque <vector <int> >::iterator it = tq.begin();
        for(int i = 0; i < graph[path_last_node].size(); i++)
        {
            if(!visited[graph[path_last_node][i]] && find(tpath.begin(), tpath.end(), graph[path_last_node][i]) == tpath.end())
            {
                //cout<<"visited : "<<graph[path_last_node][i]<<endl;
                vector <int> new_path(tpath.begin(), tpath.end());
                new_path.push_back(graph[path_last_node][i]);
                it = tq.insert(it, new_path);
                it++;
                visited[graph[path_last_node][i]] = 1;
            }
        }
    }
}

void ucs(int source, int destination)
{
    cout<<endl<<"UCS : "<<location_inv[source]<<" -> "<<location_inv[destination]<<" ["<<source<<" -> "<<destination<<"]"<<endl;
    vector <int> tpath;
    vector <int> cost;
    deque <vector <int> > tq;
    deque <vector <int> > tq1;
    int visited[sunday_traffic_lines], path_last_node;

    for(int k = 0; k < sunday_traffic_lines; k++)
    {
        visited[k] = 0;
    }
    visited[source] = 1;
    tpath.push_back(source);
    cost.push_back(0);
    tq.push_back(tpath);
    tq1.push_back(cost);
    while(!tq.empty())
    {
        tpath = tq.front();
        tq.pop_front();
        cost = tq1.front();
        tq1.pop_front();

        path_last_node = tpath[tpath.size() - 1];
        if(path_last_node == destination)
        {
            cout<<"Required path : \n";
            for (int i = 0; i < tpath.size(); i++)
            {
                printf("%s %d\n", (char *)location_inv[tpath[i]].c_str(), cost[i]);
                fprintf(out, "%s %d\n", (char *)location_inv[tpath[i]].c_str(), cost[i]);
            }
            return;
        }/*
        else
        {
            cout<<"[ ";
            for(int i=0;i<tpath.size();++i)
            {
                cout<<tpath[i]<<" ";
            }
            cout<<"]"<<endl;
        }*/

        for(int i = 0; i < graph[path_last_node].size(); i++)
        {
            deque <vector <int> >::iterator it = tq.begin();
            deque <vector <int> >::iterator it1 = tq1.begin();
            vector <int> new_path(tpath.begin(), tpath.end());
            new_path.push_back(graph[path_last_node][i]);
            vector <int> new_cost(cost.begin(), cost.end());
            int cost_sum = cost[cost.size() - 1] + travel_time[make_pair(path_last_node, graph[path_last_node][i])];
            new_cost.push_back(cost_sum);
            //cout<<">>> "<<path_last_node<<" - "<<graph[path_last_node][i]<<endl;
            //cout<<cost_sum<<endl;
            if(!visited[graph[path_last_node][i]])// && find(tpath.begin(), tpath.end(), graph[path_last_node][i]) == tpath.end())
            {
                //cout<<"hi "<<cost_sum<<endl;
                //cout<<"visited : "<<graph[path_last_node][i]<<endl;
                visited[graph[path_last_node][i]] = 1;
                if(it1 == tq1.end())
                {
                    tq.push_back(new_path);
                    tq1.push_back(new_cost);
                    continue;
                }
                while(it1 != tq1.end() && *((*it1).begin()+(*it1).size() - 1) <= cost_sum)
                {
                    it1++;
                    it++;
                }
                it = tq.insert(it, new_path);
                it1 = tq1.insert(it1, new_cost);
            }
            else
            {
                //cout<<"hi123 "<<cost_sum<<endl;
                while(it != tq.end() && graph[path_last_node][i] != *((*it).begin()+(*it).size() - 1))
                {
                    it++;
                    it1++;
                }
                if(it != tq.end() && cost_sum < *((*it1).begin()+(*it1).size() - 1))
                {
                    it = tq.erase(it);
                    it1 = tq1.erase(it1);
                    it = tq.insert(it, new_path);
                    it1 = tq1.insert(it1, new_cost);
                }
            }
        }
    }
}

void a_star(int source, int destination)
{
    cout<<endl<<"A* : "<<location_inv[source]<<" -> "<<location_inv[destination]<<" ["<<source<<" -> "<<destination<<"]"<<endl;
    deque <vector <int> > tq;
    deque <vector <int> > tq1;
    deque <int> tq2;
    vector <int> tpath;
    vector <int> cost;
    int hcost;
    int visited[sunday_traffic_lines], path_last_node;

    for(int k = 0; k < sunday_traffic_lines; k++)
    {
        visited[k] = 0;
    }
    visited[source] = 1;
    tpath.push_back(source);
    cost.push_back(0);
    tq.push_back(tpath);
    tq1.push_back(cost);
    tq2.push_back(sunday_traffic_time[source]);
    while(!tq.empty())
    {
        tpath = tq.front();
        tq.pop_front();
        cost = tq1.front();
        tq1.pop_front();
        hcost = tq2.front();
        tq2.pop_front();

        path_last_node = tpath[tpath.size() - 1];
        if(path_last_node == destination)
        {
            cout<<"Required path : \n";
            for (int i = 0; i < tpath.size(); i++)
            {
                printf("%s %d\n", (char *)location_inv[tpath[i]].c_str(), cost[i]);
                fprintf(out, "%s %d\n", (char *)location_inv[tpath[i]].c_str(), cost[i]);
            }
            return;
        }/*
        else
        {
            cout<<"[ ";
            for(int i=0;i<tpath.size();++i)
            {
                cout<<tpath[i]<<" ";
            }
            //cout<<"]"<<endl;
            cout<<"] "<<cost[cost.size() - 1]<<" "<<hcost<<endl;
        }*/

        for(int i = 0; i < graph[path_last_node].size(); i++)
        {
            deque <vector <int> >::iterator it = tq.begin();
            deque <vector <int> >::iterator it1 = tq1.begin();
            deque <int>::iterator it2 = tq2.begin();
            vector <int> new_path(tpath.begin(), tpath.end());
            new_path.push_back(graph[path_last_node][i]);
            vector <int> new_cost(cost.begin(), cost.end());
            int cost_sum = cost[cost.size() - 1] + travel_time[make_pair(path_last_node, graph[path_last_node][i])];
            new_cost.push_back(cost_sum);
            int new_hcost = cost_sum + sunday_traffic_time[graph[path_last_node][i]];
            //cout<<">>> "<<path_last_node<<" - "<<graph[path_last_node][i]<<endl;
            //cout<<cost_sum<<" "<<new_hcost<<endl;
            if(!visited[graph[path_last_node][i]])// && find(tpath.begin(), tpath.end(), graph[path_last_node][i]) == tpath.end())
            {
                //cout<<"hi "<<cost_sum<<endl;
                //cout<<"visited : "<<graph[path_last_node][i]<<endl;
                visited[graph[path_last_node][i]] = 1;
                if(it2 == tq2.end())
                {
                    tq.push_back(new_path);
                    tq1.push_back(new_cost);
                    tq2.push_back(new_hcost);
                    continue;
                }
                while(it2 != tq2.end() && (*it2) <= new_hcost)
                {
                    it++;
                    it1++;
                    it2++;
                }
                it = tq.insert(it, new_path);
                it1 = tq1.insert(it1, new_cost);
                it2 = tq2.insert(it2, new_hcost);
            }
            else
            {
                //cout<<"hi123 "<<cost_sum<<endl;
                while(it != tq.end() && graph[path_last_node][i] != *((*it).begin()+(*it).size() - 1))
                {
                    it++;
                    it1++;
                    it2++;
                }
                if(it != tq.end() && cost_sum < *((*it1).begin()+(*it1).size() - 1))
                {
                    it = tq.erase(it);
                    it1 = tq1.erase(it1);
                    it2 = tq2.erase(it2);
                    it = tq.insert(it, new_path);
                    it1 = tq1.insert(it1, new_cost);
                    it2 = tq2.insert(it2, new_hcost);
                }
            }
        }
    }
}
