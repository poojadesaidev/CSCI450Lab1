#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <queue>
#include <cmath>
#include <string>

using namespace std;

//Read the scores file and create two hashmaps, one to map NodeName to index and other to Map NodeName to cost
int createHashMaps(string fileName, unordered_map<string, int> *indexMap, unordered_map<int, string> *nameMap, unordered_map<string, int> *costMap)
{
  ifstream file(fileName);
  if (!file.is_open())
  {
    cout << "Sorry! No File with name " << fileName << "found";
    return 0;
  }

  string line;
  int idx = 0;
  while (getline(file, line))
  {
    istringstream sStm(line);
    string nodeName;
    int cost;
    sStm >> nodeName;
    sStm >> cost;

    try
    {
      (*indexMap).at(nodeName);
    }
    catch (exception e)
    {
      (*indexMap)[nodeName] = idx;
      (*nameMap)[idx] = nodeName;
      idx++;
    }

    try
    {
      (*costMap).at(nodeName);
    }
    catch (exception e)
    {
      (*costMap)[nodeName] = cost;
    }
  }
  file.close();
  return idx;
}

/////////////////////////GEEKS For GEEKS Start ////////////////////////////////////////////////////////
#define INF 0x3f3f3f3f

// iPair ==> Integer Pair
typedef pair<int, float> iPair;

struct ToFrom
{
  int to;
  int from;
};

typedef pair<int, struct ToFrom> jPair;

// To add an edge
void addEdge(vector<pair<int, float> > adj[], int u,
             int v, float wt)
{

  adj[u].push_back(make_pair(v, wt));
  adj[v].push_back(make_pair(u, wt));
}

// Prints shortest paths from src to all other vertices
void shortestPath(vector<pair<int, float> > adj[], int V, int src, int dest, unordered_map<int, string> nameMap, int k)
{
  // Create a priority queue to store vertices that
  // are being preprocessed. This is weird syntax in C++.
  // Refer below link for details of this syntax
  // http://geeksquiz.com/implement-min-heap-using-stl/
  priority_queue<iPair, vector<iPair>, greater<iPair> > pq;
  priority_queue<iPair, vector<iPair>, greater<iPair> > pq2;

  // Create a vector for distances and initialize all
  // distances as infinite (INF)
  vector<float> dist(V, INF);
  vector<int> parent(V, -1);

  // Insert source itself in priority queue and initialize
  // its distance as 0.
  pq.push(make_pair(0.0f, src));
  pq2.push(make_pair(0.0f, src));
  dist[src] = 0.0f;
  parent[src] = src;

  /* Looping till priority queue becomes empty (or all
	distances are not finalized) */
  while (!pq.empty())
  {
    // The first vertex in pair is the minimum distance
    // vertex, extract it from priority queue.
    // vertex label is stored in second of pair (it
    // has to be done this way to keep the vertices
    // sorted distance (distance must be first item
    // in pair)
    int u = pq.top().second;
    int uParent = pq2.top().second;
    pq.pop();
    pq2.pop();

    if (u == dest && dist[u] != INF && parent[u] == uParent)
    {
      //cout << "Dest is Reached !" << endl;
      int nhops = 1;
      vector<string> fpath;
      fpath.push_back(nameMap.at(u));
      //vector<int> fpath;
      //fpath.push_back(u);
      //cout << " " << u;
      int pt = uParent;
      while (pt != src)
      {
        nhops++;
        fpath.push_back(nameMap.at(pt));
        //fpath.push_back(pt);
        //cout << " " << pt;
        pt = parent[pt];
      }
      fpath.push_back(nameMap.at(pt));
      // cout << " " << pt;
      // cout << "Dest is Reached !" << endl;
      //fpath.push_back(pt);

      if (nhops <= k)
      {
        //cout << "Path exists between " << nameMap.at(src) << " and " << nameMap.at(dest) << " within " << k << " hops" << endl;
        for (int i = nhops; i >= 0; i--)
        {
          cout << fpath[i] << " ";
        }
        cout << endl
             << fixed << setprecision(2) << dist[u] << endl;
        //      << "Number of Hops " << nhops;
        // cout << "xxxxxxxxxxxxxxxxxxxx " << endl;
        // for (int y = 0; y < dist.size(); y++)
        // {
        //   cout << y << " parent = " << parent[y] << " and dist= " << dist[y] << endl;
        // }
        // cout << "xxxxxxxxxxxxxxxxxxxx " << endl;
        return;
      }
      else
      {
        //cout << "here";
        dist[u] = INF;
        parent[u] = -1;
        continue;
      }
    }

    // Get all adjacent of u.
    //for (auto x : adj[u])
    // cout << "---------------------------- " << endl;
    // cout << "Printing ADJ of " << u << endl;
    for (int k = 0; k < adj[u].size(); k++)
    {
      // Get vertex label and weight of current adjacent
      // of u.
      int v = adj[u][k].first;
      float weight = adj[u][k].second;

      //cout << v << " at wt " << weight << endl;

      // If there is shorted path to v through u.
      if (dist[v] > dist[u] + weight)
      {
        // Updating distance of v
        dist[v] = dist[u] + weight;
        parent[v] = u;
        pq.push(make_pair(dist[v], v));
        pq2.push(make_pair(dist[v], u));
      }
    }
    //cout << "---------------------------- " << endl;
  }

  cout << "No path exists between " << nameMap.at(src) << " and " << nameMap.at(dest) << " within " << k << " hops" << endl;
}

/////////////////////////GEEKS For GEEKS End ////////////////////////////////////////////////////////

//Read the graph and scores file and create the adjacency list to find shortest path
void createAdjacencyList(string graphfileName, vector<iPair> adj[], unordered_map<string, int> indexMap, unordered_map<string, int> costMap)
{

  ifstream gfile(graphfileName);
  if (!gfile.is_open())
  {
    cout << "Sorry! No File with name " << graphfileName << "found";
    return;
  }

  string gline;
  while (getline(gfile, gline))
  {
    istringstream sStm(gline);
    string sourceNName;
    string destNName;

    int sourceIdx;
    int destIdx;

    float sourceCost;
    float destCost;

    sStm >> sourceNName;
    sStm >> destNName;

    //Get Source Node IDX
    try
    {
      sourceIdx = indexMap.at(sourceNName);
    }
    catch (exception e)
    {
      cout << "Sorry! Node " << sourceIdx << "not found in scores.txt";
    }

    //Get Dest Node IDX
    try
    {
      destIdx = indexMap.at(destNName);
    }
    catch (exception e)
    {
      cout << "Sorry! Node " << destIdx << "not found in scores.txt";
    }

    //Get Source Node Cost
    try
    {
      sourceCost = (float)costMap.at(sourceNName);
    }
    catch (exception e)
    {
      cout << "Sorry! Node " << sourceCost << "not found in scores.txt";
    }

    //Get Dest Node Cost
    try
    {
      destCost = (float)costMap.at(destNName);
    }
    catch (exception e)
    {
      cout << "Sorry! Node " << destCost << "not found in scores.txt";
    }

    float totalCost = (float)abs(sourceCost - destCost) / (sourceCost + destCost);
    addEdge(adj, sourceIdx, destIdx, totalCost);
  }
  gfile.close();
}

int main(int argc, char **argv)
{

  unordered_map<string, int> indexMap;
  unordered_map<int, string> nameMap;
  unordered_map<string, int> costMap;
  int numNodes;
  string scoresFName = "scores.txt";
  string graphFName = "graph.txt";
  numNodes = createHashMaps(scoresFName, &indexMap, &nameMap, &costMap);
  vector<iPair> adj[numNodes];
  createAdjacencyList(graphFName, adj, indexMap, costMap);
  // for (int l = 0; l < numNodes; l++)
  // {
  //   cout << "=================================" << endl;
  //   cout << l << endl;
  //   for (int k = 0; k < adj[l].size(); k++)
  //   {
  //     cout << adj[l][k].first << " " << adj[l][k].second << endl;
  //   }
  //   cout << endl;
  //   cout << "=================================" << endl;
  // }

  string src = argv[1];
  string dest = argv[2];
  int nHops;
  stringstream sStm(argv[3]);
  if (!(sStm >> nHops))
  {
    cout << "No path exists between " << src << " and " << dest << " within " << argv[3] << " hops" << endl;
    return EXIT_FAILURE;
  }

  int srcIdx;
  int destIdx;
  try
  {
    srcIdx = indexMap.at(src);
  }
  catch (exception e)
  {
    cout << "No path exists between " << src << " and " << dest << " within " << nHops << " hops" << endl;
    return EXIT_FAILURE;
  }

  try
  {
    destIdx = indexMap.at(dest);
  }
  catch (exception e)
  {
    cout << "No path exists between " << src << " and " << dest << " within " << nHops << " hops" << endl;
    return EXIT_FAILURE;
  }

  if(srcIdx == destIdx){
    cout << "No path exists between " << src << " and " << dest << " within " << nHops << " hops" << endl;
    return EXIT_FAILURE;
  }

  shortestPath(adj, numNodes, srcIdx, destIdx, nameMap, nHops);

  return 0;
}