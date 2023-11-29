#include <bits/stdc++.h>
using namespace std;

class Graph
{
    map<int, list<int>> adjacency_list;
    vector<int> vertices;
    bool undirected = false;
    map<int, list<int>> reve_adja_list;

public:
    Graph(vector<vector<int>> d, vector<int> vertices, bool undirected = true)
    {
        this->vertices = vertices;
        this->undirected = undirected;
        for (auto v : this->vertices)
        {
            for (auto edge : d)
            {
                if (edge[0] == v)
                {
                    auto keyIt = adjacency_list.find(edge[0]);
                    if (keyIt == adjacency_list.end())
                    {
                        adjacency_list.insert(keyIt, std::make_pair(edge[0], std::list<int>{edge[1]}));
                    }
                    else
                    {
                        keyIt->second.push_back(edge[1]);
                    }
                }
                else if (edge[1] == v && undirected)
                {
                    auto keyIt = adjacency_list.find(edge[1]);
                    if (keyIt == adjacency_list.end())
                    {
                        adjacency_list.insert(keyIt, std::make_pair(edge[1], std::list<int>{edge[0]}));
                    }
                    else
                    {
                        keyIt->second.push_back(edge[1]);
                    }
                }
            }
        }
        if (!undirected)
        {
            for (auto v : this->vertices)
            {
                for (auto edge : d)
                {
                    if (edge[1] == v)
                    {
                        auto keyIt = reve_adja_list.find(edge[1]);
                        if (keyIt == reve_adja_list.end())
                        {
                            reve_adja_list.insert(keyIt, std::make_pair(edge[1], std::list<int>{edge[0]}));
                        }
                        else
                        {
                            auto listIt = std::find(keyIt->second.begin(), keyIt->second.end(), edge[0]);
                            if (listIt == keyIt->second.end())
                            {
                                keyIt->second.push_back(edge[0]);
                            }
                        }
                    }
                }
            }
        }
    }

    void print(bool rev = false)
    {
        auto ad_list = this->adjacency_list;
        if (rev)
        {
            ad_list = this->reve_adja_list;
        }
        for (auto &&[key, val] : ad_list)
        {
            cout << "key = " << key << endl;
            for (auto v : val)
            {
                cout << v << " ";
            }
            cout << endl;
        }
    }

    void DFS(int s)
    {
        stack<int> st;
        st.push(s);
        map<int, bool> marked;
        for (auto v : vertices)
        {
            marked.at(v) = false;
        }
        while (st.size() != 0)
        {
            auto v = st.top();
            st.pop();
            if (!marked.at(v))
            {
                marked.at(v) = true;
                for (auto v_ : adjacency_list[v])
                {
                    st.push(v_);
                }
                cout << "traverse " << v << endl;
            }
        }
    }
    void DFS_recurese(int s)
    {
        std::function<void(int, map<int, bool> &)> helper = [&](int s_, map<int, bool> &marked)
        {
            marked.at(s_) = true;
            cout << "visited to " << s_ << endl;
            for (auto v : adjacency_list[s_])
            {
                if (!marked.at(v))
                {
                    helper(v, marked);
                }
            }
        };
        map<int, bool> marked;
        for (auto v : vertices)
        {
            marked.insert(marked.begin(), std::make_pair(v, false));
        }
        helper(s, marked);
    }

    void topoSort()
    {
        /*
            This algorithm is applied for a directed acyclic graph.
            Theorem 1. Every directed acyclic graph has at least one topo ordering.
            Theorem 2. A directed cycle graph doesn't have any topo ordering.
            Theorem 3. Every directed acyclic graphs has a least one source vertex.
            source vertex: là đỉnh mà không có bất kỳ cạnh đi vào nào.
        */
        int curLabel = vertices.size();

        map<int, int> f;
        for (auto v : vertices)
        {
            f.insert(f.begin(), std::make_pair(v, 9999999));
        }

        function<void(int, map<int, bool> &)> DFS_topo = [&](int s, map<int, bool> &m)
        {
            m.at(s) = true;
            for (auto v : adjacency_list[s])
            {
                if (!m.at(v))
                {
                    DFS_topo(v, m);
                }
            }
            f.at(s) = curLabel;
            curLabel -= 1;
        };

        map<int, bool> marked;
        for (auto v : vertices)
        {
            marked.insert(marked.begin(), std::make_pair(v, false));
        }

        for (auto v : vertices)
        {

            if (!marked.at(v))
            {
                DFS_topo(v, marked);
            }
        }

        for (auto &&[key, val] : f)
        {
            cout << key << " " << val << endl;
        }
    }

    void korasaju()
    {

        int curLabel = vertices.size();

        map<int, int> f;
        for (auto v : vertices)
        {
            f.insert(f.begin(), std::make_pair(v, 9999999));
        }

        function<void(int, map<int, list<int>>, map<int, bool> &)> DFS_topo = [&](int s, map<int, list<int>> g, map<int, bool> &m)
        {
            m[s] = true;
            for (auto v : g[s])
            {
                if (!m[v])
                {
                    DFS_topo(v, g, m);
                }
            }
            f[s] = curLabel;
            curLabel--;
        };

        map<int, bool> marked;
        for (auto v : vertices)
        {
            marked.insert(marked.begin(), std::make_pair(v, false));
        }

        for (auto v : vertices)
        {
            if (!marked[v])
            {
                DFS_topo(v, this->reve_adja_list, marked);
            }
        }

        for (auto &&[key, val] : f)
        {
            cout << key << " " << val << endl;
        }

        map<int, int> scc;
        for (auto v : vertices)
        {
            f.insert(f.begin(), std::make_pair(v, 0));
        }
        int numSCC = 0;
        function<void(int, map<int, bool> &)> DFS_SCC = [&](int s, map<int, bool> &m)
        {
            m[s] = true;
            scc[s] = numSCC;
            for (auto v : this->adjacency_list[s])
            {
                if (!m.at(v))
                {
                    DFS_SCC(v, m);
                }
            }
        };

        function<bool(pair<int, int> &, pair<int, int> &)> cmp = [](pair<int, int> &p1, pair<int, int> &p2)
        {
            return p1.second < p2.second;
        };

        // reset marked list and sort  map using a vector
        vector<pair<int, int>> v_m;
        map<int, int>::iterator it_ = f.begin();
        for (auto v : vertices)
        {
            marked.at(v) = false;
            v_m.push_back(*it_);
            it_++;
        }
        // main kosaraju
        sort(v_m.begin(), v_m.end(), cmp);

        for (auto &&[key, val] : v_m)
        {
            if (!marked[key])
            {
                numSCC++;
                DFS_SCC(key, marked);
            }
        }
        cout << "print the scc value" << endl;
        for (int i = 1; i <= numSCC; i++)
        {
            for (auto &&[key, val] : scc)
            {
                if (val == i)
                {
                    cout << key << " " << val << endl;
                }
            }
        }
    }
};

int main()
{
    // vector<vector<int>> edge = {{1, 2}, {1, 3}, {1, 4}, {2, 5}, {3, 6}, {6, 7}, {4, 5}, {3, 4}, {8, 9}, {9, 10}, {11, 12}};
    // vector<vector<int>> edge = {{0,1}, {0,2}, {1,2}, {2,0}, {2,3}, {3,3}};
    // vector<int> vertices = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    // auto g = Graph(edge, vertices);
    // g.print();
    // g.DFS(1);
    // g.DFS(8);
    // g.DFS(11);
    // g.DFS_recurese(1);
    // g.DFS_recurese(8);
    // g.DFS_recurese(11);
    // vector<vector<int>> edge = {{1,2}, {1,3}, {3,4}, {2,4}};
    // vector<int> vertices = {1, 2, 3, 4};

    // auto dag = Graph(edge, vertices, false);
    // // dag.print();
    // dag.topoSort();

    // kosaraju algorithm

    // vector<vector<int>> edge = {{1, 3}, {3, 11}, {3, 5}, {11, 6}, {11, 8}, {6, 10}, {10, 8}, {8, 6}, {2, 10}, {2, 4}, {9, 8}, {9, 2}, {9, 4}, {4, 7}, {7, 9}, {5, 9}, {5, 7}, {5, 1}};
    // vector<int> vertices = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    // auto dag = Graph(edge, vertices, false);
    // dag.print(true);
    // // dag.topoSort();
    // dag.korasaju();
    return 0;
}