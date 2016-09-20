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

FILE *fp, *out;

void bfs(int, int);
void dfs(int, int);
void ucs(int, int);
void a_star(int, int);

int main(int argc, char const *argv[])
{
    //FILE *fp, *out;
    fp = fopen("input1.txt", "r");
    out= fopen("output.txt", "w");
    char search[5], from[100], to[100], place[100];
    //string from, to;
    int time, location_count = 0;
    //map<pair<string, string>, int> maps;

    fscanf(fp, "%s", search);
    fscanf(fp, "%s", start_state);
    fscanf(fp, "%s", goal_state);
    fscanf(fp, "%d", &live_traffic_lines);
    //printf("%s %s %s %d ", search, start_state, goal_state, live_traffic_lines);
    //fprintf(out, "%s %s %s %d ", search, start_state, goal_state, live_traffic_lines);
    graph.resize(live_traffic_lines);

    for(int i = 0; i < live_traffic_lines; i++)
    {
        fscanf(fp, "%s %s %d", from, to, &time);
        printf("%s %s %d \n", from, to, time);
        //fprintf(out, "%s %s %d \n", from, to, time);
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
    fscanf(fp, "%d", &sunday_traffic_lines);
    //fprintf(out, "%d ", sunday_traffic_lines);
    sunday_traffic_time = (int *)malloc(sunday_traffic_lines * sizeof(int));
    for(int i = 0; i < sunday_traffic_lines; i++)
    {
        fscanf(fp, "%s %d", place, &time);
        //fprintf(out, "%s %d ", place, time);
        sunday_traffic_time[location[place]] = time;
    }/*
    cout<<"\n\n ----- \n";
    for(int i = 0; i < sunday_traffic_lines; i++)
    {
        cout<<sunday_traffic_time[i]<<endl;
    }
    cout<<"\n ----- \n\n";*/

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
    fclose(fp);
    fclose(out);
    return 0;
}

void bfs(int source, int destination)
{
    cout<<"BFS : "<<source<<"->"<<destination<<endl;
    vector <int> path;
    queue <vector <int> > q;
    int path_last_node;

    path.push_back(source);
    q.push(path);
    while(!q.empty())
    {
        path = q.front();
        q.pop();

        path_last_node = path[path.size() - 1];
        if(path_last_node == destination)
        {
            cout<<"The Required path is:: \n";
            //print_path(path);
            for (int i = 0; i < path.size(); i++)
            {
                printf("%s %d\n", (char *)location_inv[path[i]].c_str(), i);
                fprintf(out, "%s %d\n", (char *)location_inv[path[i]].c_str(), i);
            }
            return;
        }

        for(int i = 0; i < graph[path_last_node].size(); i++)
        {
            if(find(path.begin(), path.end(), graph[path_last_node][i]) == path.end())
            {
                vector <int> new_path(path.begin(), path.end());
                new_path.push_back(graph[path_last_node][i]);
                q.push(new_path);
            }
        }
    }
}

void dfs(int source, int destination)
{
    cout<<"DFS : "<<source<<"->"<<destination<<endl;
    vector <int> path;
    deque <vector <int> > q;
    int visited[sunday_traffic_lines], path_last_node;

    for(int k = 0; k < sunday_traffic_lines; k++)
    {
        visited[k] = 0;
    }
    visited[source] = 1;
    path.push_back(source);
    q.push_back(path);
    while(!q.empty())
    {
        path = q.front();
        q.pop_front();

        path_last_node = path[path.size() - 1];
        if(path_last_node == destination)
        {
            cout<<"The Required path is:: \n";
            //print_path(path);
            for (int i = 0; i < path.size(); i++)
            {
                printf("%s %d\n", (char *)location_inv[path[i]].c_str(), i);
                fprintf(out, "%s %d\n", (char *)location_inv[path[i]].c_str(), i);
            }
            return;
        }

        deque <vector <int> >::iterator it = q.begin();
        for(int i = 0; i < graph[path_last_node].size(); i++)
        {
            if(!visited[graph[path_last_node][i]] && find(path.begin(), path.end(), graph[path_last_node][i]) == path.end())
            {
                vector <int> new_path(path.begin(), path.end());
                new_path.push_back(graph[path_last_node][i]);
                it = q.insert(it, new_path);
                //q.push(new_path);
                it++;
                visited[graph[path_last_node][i]] = 1;
            }
        }
    }
}

void ucs(int source, int destination)
{
    cout<<"UCS : "<<source<<"->"<<destination<<endl;
    vector <int> path;
    vector <int> cost;
    deque <vector <int> > q;
    deque <vector <int> > q1;
    int visited[sunday_traffic_lines], path_last_node;

    for(int k = 0; k < sunday_traffic_lines; k++)
    {
        visited[k] = 0;
    }
    visited[source] = 1;
    path.push_back(source);
    cost.push_back(0);
    q.push_back(path);
    q1.push_back(cost);
    while(!q.empty())
    {
        path = q.front();
        q.pop_front();
        cost = q1.front();
        q1.pop_front();

        path_last_node = path[path.size() - 1];
        if(path_last_node == destination)
        {
            cout<<"The Required path is:: \n";
            //print_path(path);
            for (int i = 0; i < path.size(); i++)
            {
                printf("%s %d\n", (char *)location_inv[path[i]].c_str(), cost[i]);
                fprintf(out, "%s %d\n", (char *)location_inv[path[i]].c_str(), cost[i]);
            }
            return;
        }/*
        else
        {
            cout<<"[ ";
            for(int i=0;i<path.size();++i)
            {
                cout<<path[i]<<" ";
            }
            cout<<"]"<<endl;
        }*/

        //deque <vector <int> >::iterator it = q.begin();
        //deque <vector <int> >::iterator it1 = q1.begin();
        for(int i = 0; i < graph[path_last_node].size(); i++)
        {
            deque <vector <int> >::iterator it = q.begin();
            deque <vector <int> >::iterator it1 = q1.begin();
            vector <int> new_path(path.begin(), path.end());
            new_path.push_back(graph[path_last_node][i]);
            vector <int> new_cost(cost.begin(), cost.end());
            int cost_sum = cost[cost.size() - 1] + travel_time[make_pair(path_last_node, graph[path_last_node][i])];
            new_cost.push_back(cost_sum);
            //cout<<">>> "<<path_last_node<<" - "<<graph[path_last_node][i]<<endl;
            //cout<<cost_sum<<endl;
            if(!visited[graph[path_last_node][i]])// && find(path.begin(), path.end(), graph[path_last_node][i]) == path.end())
            {
                //cout<<"hi "<<cost_sum<<endl;
                //vec.begin()+(vec.size()-1)
                //cout<<"visited : "<<graph[path_last_node][i]<<endl;
                visited[graph[path_last_node][i]] = 1;
                if(it1 == q1.end())
                {
                    q.push_back(new_path);
                    q1.push_back(new_cost);
                    continue;
                }
                while(it1 != q1.end() && *((*it1).begin()+(*it1).size() - 1) <= cost_sum)
                {
                    it1++;
                    it++;
                }
                /*if(it1 == q1.end())// q1.size() == 1)
                {
                    it1 = q1.begin() + (q1.size() - 1);
                    it = q.begin() + (q.size() - 1);
                }*/
                it = q.insert(it, new_path);
                it1 = q1.insert(it1, new_cost);
                //q.push(new_path);
                //it++;
                //it1++;
            }
            else
            {
                //cout<<"hi123 "<<cost_sum<<endl;
                while(it != q.end() && graph[path_last_node][i] != *((*it).begin()+(*it).size() - 1))
                {
                    it++;
                    it1++;
                }
                if(it != q.end() && cost_sum < *((*it1).begin()+(*it1).size() - 1))
                {
                    it = q.erase(it);
                    it1 = q1.erase(it1);
                    it = q.insert(it, new_path);
                    it1 = q1.insert(it1, new_cost);
                }
            }
        }
    }
}

void a_star(int source, int destination)
{
    cout<<"A* : "<<source<<"->"<<destination<<endl;
    vector <int> path;
    vector <int> cost;
    deque <vector <int> > q;
    deque <vector <int> > q1;
    int visited[sunday_traffic_lines], path_last_node;

    for(int k = 0; k < sunday_traffic_lines; k++)
    {
        visited[k] = 0;
    }
    visited[source] = 1;
    path.push_back(source);
    cost.push_back(0);
    q.push_back(path);
    q1.push_back(cost);
    while(!q.empty())
    {
        path = q.front();
        q.pop_front();
        cost = q1.front();
        q1.pop_front();

        path_last_node = path[path.size() - 1];
        if(path_last_node == destination)
        {
            cout<<"The Required path is:: \n";
            //print_path(path);
            for (int i = 0; i < path.size(); i++)
            {
                printf("%s %d\n", (char *)location_inv[path[i]].c_str(), cost[i]);
                fprintf(out, "%s %d\n", (char *)location_inv[path[i]].c_str(), cost[i]);
            }
            return;
        }
        else
        {
            cout<<"[ ";
            for(int i=0;i<path.size();++i)
            {
                cout<<path[i]<<" ";
            }
            cout<<"]"<<endl;
        }

        vector <int> next_node_list = graph[path_last_node];
        //cout<<"hi "<<next_node_list[0]<<endl;
        int i;
        for(i = 0; i < next_node_list.size(); i++)
        {
            if(!visited[next_node_list[i]])
            {
                break;
            }
        }
        int next_cost_min_value = sunday_traffic_time[next_node_list[i]], next_cost_min_index = next_node_list[i];
        for(i = i + 1; i < next_node_list.size(); i++)
        {
            //printf("in loop : node = %d h = %d \n", next_node_list[i], sunday_traffic_time[next_node_list[i]]);
            if(!visited[next_node_list[i]] && next_cost_min_value > sunday_traffic_time[next_node_list[i]])
            {
                next_cost_min_value = sunday_traffic_time[next_node_list[i]];
                next_cost_min_index = next_node_list[i];
            }
        }
        //cout<<"hi123 "<<next_cost_min_value<<" "<<next_cost_min_index<<endl;
        vector <int> new_path(path.begin(), path.end());
        new_path.push_back(next_cost_min_index);
        vector <int> new_cost(cost.begin(), cost.end());
        new_cost.push_back(cost[cost.size() - 1] + travel_time[make_pair(path_last_node, next_cost_min_index)]);
        q.push_back(new_path);
        q1.push_back(new_cost);
        visited[next_cost_min_index] = 1;
    }
}
