// Graph.c ... implementation of Graph ADT
// Written by John Shepherd, May 2013

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Graph.h"
#include "Queue.h"

// graph representation (adjacency matrix)
typedef struct GraphRep {
	int nV;		 // #vertices
	int nE;		 // #edges
	int **edges; // matrix of weights (0 == no edge)
} GraphRep;

// check validity of Vertex
int validV (Graph g, Vertex v)
{
	return (g != NULL && v >= 0 && v < g->nV);
}

// make an edge
static Edge mkEdge (Graph g, Vertex v, Vertex w)
{
	assert (g != NULL && validV (g, v) && validV (g, w));
	Edge new = {v, w}; // struct assignment
	return new;
}

// insert an Edge
// - sets (v,w) and (w,v)
void insertEdge (Graph g, Vertex v, Vertex w, int wt)
{
	assert (g != NULL && validV (g, v) && validV (g, w));

	if (g->edges[v][w] != 0 && g->edges[w][v] != 0)
		// an edge already exists; do nothing.
		return;

	g->edges[v][w] = wt;
	g->edges[w][v] = wt;
	g->nE++;
}

// remove an Edge
// - unsets (v,w) and (w,v)
void removeEdge (Graph g, Vertex v, Vertex w)
{
	assert (g != NULL && validV (g, v) && validV (g, w));
	if (g->edges[v][w] == 0 && g->edges[w][v] == 0)
		// an edge doesn't exist; do nothing.
		return;

	g->edges[v][w] = 0;
	g->edges[w][v] = 0;
	g->nE--;
}

// create an empty graph
Graph newGraph (int nV)
{
	assert (nV > 0);

	GraphRep *new = malloc (sizeof *new);
	assert (new != NULL);
	*new = (GraphRep){ .nV = nV, .nE = 0 };

	new->edges = calloc ((size_t) nV, sizeof (int *));
	assert (new->edges != NULL);
	for (int v = 0; v < nV; v++) {
		new->edges[v] = calloc ((size_t) nV, sizeof (int));
		assert (new->edges[v] != 0);
	}

	return new;
}

// free memory associated with graph
void dropGraph (Graph g)
{
	assert (g != NULL);
	for (int v = 0; v < g->nV; v++)
		free (g->edges[v]);
	free (g->edges);
	free (g);
}

// display graph, using names for vertices
void showGraph (Graph g, char **names)
{
	assert (g != NULL);
	printf ("#vertices=%d, #edges=%d\n\n", g->nV, g->nE);
	int v, w;
	for (v = 0; v < g->nV; v++) {
		printf ("%d %s\n", v, names[v]);
		for (w = 0; w < g->nV; w++) {
			if (g->edges[v][w]) {
				printf ("\t%s (%d)\n", names[w], g->edges[v][w]);
			}
		}
		printf ("\n");
	}
}

// find a path between two vertices using breadth-first traversal
// only allow edges whose weight is less than "max"
int findPath (Graph g, Vertex src, Vertex dest, int max, int *path)
{
	assert (g != NULL);
	Queue newQ = newQueue();
	QueueJoin(newQ, src);
	path[0] = src;
	int pathcounter = 0;
	int returnvalue = 0;
	int founddest = 0;
	int visited[30] = {0};
	visited[src] = 1;
	int distance[1000] = {0};
	distance[src] = 0;

	//Adding all connections from source to queue
	for (int x=0; x< g->nV; x++){
		if (g->edges[x][src] != 0 && g->edges[x][src] <= max){
			QueueJoin(newQ, x);
		}
	}
	
	if (g->edges[dest][src] != 0 && g->edges[dest][src] <= max){
		printf("They are connected\n");
		returnvalue = 2;
		path[1] = dest;
	}else{
		while(!QueueIsEmpty(newQ)){
			Vertex temp = QueueLeave(newQ);
			
			for (int i = 0; i < g->nV; i++){
				if(visited[g->edges[temp][i] == 1]){
					continue;
				}
				if(g->edges[i][temp] != 0 && g->edges[i][temp] <= max){
					distance[g->edges[temp][i]] = distance[temp]+1;
					QueueJoin(newQ, i);
					visited[i] = 1;
				}

			
			}

			
		}
	}
	//if (founddest == 0){
	//	returnvalue = 0;
	//}
	print(distance[dest]);
	dropQueue(newQ);
	return returnvalue; // never find a path ... you need to fix this
}
