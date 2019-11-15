#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <set>
#include <tuple>
#include <bits/stdc++.h>
using namespace std;


//method to get vertices from a sentence
vector<string> split(string s, char delim){
	vector<string> result;
	stringstream ss (s);
	string item;
	while(getline(ss,item,delim)){
		result.push_back(item);
	}
	return result;
}

//node for the linked list to contain vertices of a graph
struct vertex{
	string buildingNum;//name of the node , which is the building number provided in NEU map
	float distFromSource; // distance of the vertex from the considered source vertex
	string prevVertex;
	vertex *prev; //pointer that points to the previous element in the shortest path
	vertex *next;//pointer that points to the next element in the Vertex[G] list of the graph
};

//node that contains all the edges of a graph
struct edge{
	string vertex1;
	string vertex2;
	float weight;
};
//node that contains the details of nodes in the edges
struct adjVertexList{
	string name;//name of the node , which is the building number provided in NEU map
	string sourcename;//name of the source vertex to which is adjacent to
	float distFromVertex;//distance of the vertex from its considered adjacent vertex
	adjVertexList *next;//pointer to the next element in the adjacent vertices linked list
	adjVertexList *prev;//pointer to the considered adjacent vertex
};

//struct to provide comparison
struct dist_from_sources_key{
	inline bool operator()(const vertex& vertex1,const vertex& vertex2)
	{
		return (vertex1.distFromSource<vertex2.distFromSource);
	}
};
//method to fetch the adjacency list of a provided vector
vector<adjVertexList> getadjacencylist(vector<pair<string,vector<adjVertexList>>> adjacencyList,string toFetch){
	auto itr = adjacencyList.begin();
	for(itr;itr!=adjacencyList.end();itr++){
		if(!((*itr).first.compare(toFetch))){
			return (*itr).second;
		}
	}
}
//method to get a vertex node from a vector of vertices
vertex* getvertex(vector<vertex>* vertices,string vertex){
	for(auto itr = (*vertices).begin();itr!=(*vertices).end();itr++){
		if(!(*itr).buildingNum.compare(vertex)){
			return &(*itr);
		}
	}
}
//method to check if a vertex is present in the provided vector of vertex
bool checkvertex(vector<vertex>* vertices,string vertex){
	for(auto itr = (*vertices).begin();itr!=(*vertices).end();itr++){
		if(!(*itr).buildingNum.compare(vertex)){
			return true;
		}
	}
	return false;
}

//method to relax
void relax(vertex& b,vertex& v,adjVertexList& l){
if(((v).distFromSource)>((b).distFromSource+(l).distFromVertex)){
	(v).distFromSource = ((b).distFromSource+(l).distFromVertex);
	(v).prevVertex= b.buildingNum;
}
}
//method to print
void print_therest(vertex v,vector<vertex>* vertices){
	while(v.prevVertex!=""){
		cout<<v.prevVertex<<endl;
		v = *(getvertex(vertices,v.prevVertex));
	}
}

vector<vertex> path;
//Dijkstra's Algorithm //check this and print path.
void dijkstras(vector<vertex> vertices,vector<edge> edges,vector<pair<string,vector<adjVertexList>>> adjacencyList,string source,string destination){
//initialize-single source
	float infinity = 10000;
	for(auto i= vertices.begin();i!=vertices.end();i++){
			if(!(source.compare((*i).buildingNum))){
				(*i).distFromSource = 0;
				(*i).prev = NULL;
				(*i).prevVertex = "";
			}else{
				(*i).distFromSource = ++infinity;
				(*i).prev = NULL;
				(*i).prevVertex = "";
			}
		}

	vector<adjVertexList> adjList;
	sort(vertices.begin(),vertices.end(),dist_from_sources_key());
	vector<vertex> queue  = vertices;
	vector<vertex>* track = &queue;
	int count = 0;
	int size = queue.size();
	while(count<size){
		sort(queue.begin(),queue.end(),dist_from_sources_key());
		vertex begin = queue.front();
		queue.erase(queue.begin());
		path.insert(path.end(),begin);
		vector<adjVertexList> adjL = getadjacencylist(adjacencyList, begin.buildingNum);
		for(adjVertexList l:adjL){
			if(!(checkvertex(&path,l.name))){
				vertex* v = getvertex(&queue, l.name);
				relax(begin,*v,l);

			}
			}
		count++;
		for(auto v:path){
				cout<<v.buildingNum<<" "<<v.distFromSource<<endl;
		}
   //print destination path
		for(auto v:path){
		if(!(destination.compare(v.buildingNum))){
      cout<<"The path vertices in the order from DESTINATION<-SOURCE:"<<endl;
			cout<<v.buildingNum<<endl;
			print_therest(v,&path);
		}

		}

	}
}





int main(){

//Build a vector of vertices for the array Vertex[G] of the graph
vector<vertex> verticesList;


//read from the text file and extract the vertices and populate the vertex[G] of the graph
//populate both the vectors of verticesList and adjVerticesList.
//Note: implementing a unordered/ ordered set with user defined data types in C++ requires the use of spl methods in addn to the data
//set used to store the vertices(building names)
set<string> vertices;
//vector containing the edges along with their weights
vector<edge> edges_w_weights;
//input filestream to create vertices
ifstream istream;
//input filestream to create edges
ifstream istream1;

vector<vertex> path;

//string to store local variable from input stream - istream and istream1
string sentence;

//create a stream from the input file containing vertices
istream.open("graph-load.txt");
//exit if there is an error opening the file
if(istream.fail()){
	cerr<<"Error opening file \n";
	exit(1);
}
//handle exceptions is the stream is malformed
istream.exceptions(ifstream::failbit|ifstream::badbit);
//read till the end of the stream
while(!istream.eof()){
//read from the stream to extract vertices and add them to a set so that there are
	// no duplicates
try{
	istream>>sentence;
	vector<string> values = split(sentence,',');
	values.pop_back();
	for(auto s:values){
		vertices.insert(s);
	}
//if there is a failure break from the loop;
}catch (ifstream::failure e) {
	break;
}
}
//from the 'vertices' which is a set of vertices for the graph - move it to the set
//of structs of vertices - 'verticesList'
for(auto var:vertices){
	vertex v = {var};
	verticesList.insert(verticesList.end(),v);
}
//printing out to check
//for(auto val:verticesList){
//	cout<<val.buildingNum<<endl;
//}
//total number of vertices in the graph
//cout<<verticesList.size()<<endl;
//create a stream from the input file containing edges
istream1.open("graph-load.txt");
//exit if there is an error opening the file
if(istream1.fail()){
	cerr<<"Error opening file \n";
	exit(1);
}
//handle exceptions is the stream is malformed
istream1.exceptions(ifstream::failbit|ifstream::badbit);
while(!istream1.eof()){
//extract edges from istream and their corresponding vertices
	//keep them in edge structure and add them to edge_w_weight vector
	//which contains  edge type elements.
try{
	istream1>>sentence;
	vector<string> values = split(sentence,',');
	edge edge_c = {values.at(0),values.at(1),stof(values.at(2))};
	edges_w_weights.insert(edges_w_weights.end(),edge_c);
}catch (ifstream::failure e) {
	break;
}
}
//printing out to check
/*for(auto val:edges_w_weights){
	cout<<val.vertex1<<" "<<val.vertex2<<" "<<val.weight<<endl;
}*/
//total number of edges in the graph
//cout<<edges_w_weights.size()<<endl;

int count = 0;

//Build a vector of adjacent vertices and their distance from the adjacent vertex
vector<vector<adjVertexList>> adjVerticesList;

//build the adjacentVertices declared above to contain a list of vectors each containing the type structure adjVertexList
for(vertex v:verticesList){
	adjVertexList u;
	vector<adjVertexList> u_list;
	u.sourcename=v.buildingNum;
	for(edge e:edges_w_weights){
		if(!(v.buildingNum.compare(e.vertex1))){
			u.name=e.vertex2;
			u.distFromVertex =e.weight;
			u_list.insert(u_list.end(),u);
			//cout<<++count<<endl;
			//cout<<e.vertex1<<endl;
		}
	}
	adjVerticesList.insert(adjVerticesList.end(),u_list);
}

//printing out to check
/*cout<<adjVerticesList.size()<<endl;
for(auto val:adjVerticesList){
	for(auto val1: val){
		cout<<val1.sourcename<<" "<<val1.name<<" "<<val1.distFromVertex<<endl;
		cout<<++count<<endl;
	}
}*/

string source;
string destination;
cout<<"Please enter the source vertex";
cin>>source;
cout<<"Please enter the destination vertex";
cin>>destination;

//create adjacencyList
vector<pair<string,vector<adjVertexList>>> adjacencyList;
for(vertex x:verticesList){
	pair<string,vector<adjVertexList>> pair1;
	vector<adjVertexList> adjvertexlist;
	for(auto v:adjVerticesList){
		for(auto v1:v){
		if(!(x.buildingNum).compare(v1.sourcename)){
			adjvertexlist.insert(adjvertexlist.end(), v1);
		}
		}
	}
	pair1.first = x.buildingNum;
	pair1.second = adjvertexlist;
	adjacencyList.insert(adjacencyList.end(), pair1);
}

//test for get adjacencylist method
/*
auto result = getadjacencylist(adjacencyList, "2");
for(auto v:result){
	cout<<v.name<<" "<<v.sourcename<<endl;
}
*/

dijkstras(verticesList, edges_w_weights, adjacencyList, source, destination);
//Implement Dijkstra's Algorithm

}








