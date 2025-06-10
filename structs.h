#ifndef STRUCTS_H

#define STRUCTS_H
//////////////////////////////

typedef struct {
	int computer;
	int min;
} Candidate;

typedef struct candidate_node {
	Candidate c;
	struct candidate_node* next;
	struct candidate_node* prev;
} CandidateNode;

typedef struct {
	CandidateNode* head;
	CandidateNode* tail;
} CandidateList;


//////////////////////////////////
typedef struct {
	int neighbor;
	int cost;
} Edge;

typedef struct edge_node {
	Edge e;
	struct edge_node* next;
} EdgeNode;

typedef struct {
	EdgeNode* head;
	EdgeNode* tail;
} EdgeList;

//////////////////////////////////

#endif
