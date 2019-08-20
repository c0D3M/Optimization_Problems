#include <iostream>
#include <vector>
#include <sys/resource.h>
#include <algorithm>
/*
Independent Set problem with weighted Nodes
Output: Max Size Independent Set node.
Special Case Algorithm: Works for all cases, in optimal time but only for special case.
*/
struct Vertex {
    int weight;
    long long  best;
    std::vector <int> children;
};
typedef std::vector<Vertex> Graph;
typedef std::vector<int> Sum;

Graph ReadTree() {
    int vertices_count;
    std::cin >> vertices_count;

    Graph tree(vertices_count);

    for (int i = 0; i < vertices_count; ++i)
        std::cin >> tree[i].weight;

    tree[0].best = -1;
    for (int i = 1; i < vertices_count; ++i) {

        int from, to, weight;
        std::cin >> from >> to;
        tree[from - 1].children.push_back(to - 1);

        tree[to - 1].children.push_back(from - 1);
        tree[from - 1].best = -1;
        tree[to - 1].best = -1;
    }

    return tree;
}

long long dfs(Graph &tree, int vertex, int parent) {
    if(tree[vertex].best!=-1)
        return tree[vertex].best;
    if(!tree[vertex].children.size()) //Leaf node
    {
        tree[vertex].best = tree[vertex].weight;
        return tree[vertex].best;
    }

    //M0 =  Take Node & Grand-Children
    //M1 = Take Children
    unsigned long long m0 =tree[vertex].weight;
    for (auto &i: tree[vertex].children)
        if(i!=vertex && (i!=parent)) // Ignore child if it is same as parent
        for (auto&j:tree[i].children)
            //Ignore grand-child if its part of any of parents sibling ?
            if((j!=vertex) && (find(tree[vertex].children.begin(), tree[vertex].children.end(), j) ==tree[vertex].children.end()))
            	m0 += dfs(tree, j, i);
    unsigned long long m1 = 0;
    for (auto &i: tree[vertex].children)
        if(i!=parent) // Ignore child if it is same as parent
        	m1 += dfs(tree, i, vertex);
    tree[vertex].best =std::max(m0, m1);
    return tree[vertex].best;
}

int MaxWeightIndependentTreeSubset(Graph &tree) {
    size_t size = tree.size();
    if (size == 0)
        return 0;
    return dfs(tree, 0, -1);
}

int main() {
    // This code is here to increase the stack size to avoid stack overflow
    // in depth-first search.
    const rlim_t kStackSize = 64L * 1024L * 1024L;  // min stack size = 64 Mb
    struct rlimit rl;
    int result;
    result = getrlimit(RLIMIT_STACK, &rl);
    if (result == 0)
    {
        if (rl.rlim_cur < kStackSize)
        {
            rl.rlim_cur = kStackSize;
            result = setrlimit(RLIMIT_STACK, &rl);
            if (result != 0)
            {
                //fprintf(stderr, "setrlimit returned result = %d\n", result);
            }
        }
    }

    Graph tree = ReadTree();
    int weight = MaxWeightIndependentTreeSubset(tree);
    std::cout << weight << std::endl;
    return 0;
}
