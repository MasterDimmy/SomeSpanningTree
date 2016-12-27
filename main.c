#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sets.h"

#define VERTEX_MAX 	1000

int VERTEX_COUNT = 0;

/* print edga data to screen */
void print_basic_edge(int tree_edges[VERTEX_MAX][2]) {
	int i=0;
	/* while tree_edges has any not empty vertex nums */
	while (tree_edges[i][0]!=0 || tree_edges[i][1]!=0) {
		printf("%d -> %d\n", tree_edges[i][0] + 1, tree_edges[i][1] + 1); /*fix numeration*/
		i++;
	}
}

/*
	This function finds some spanning tree of max. depth N if it is exists,
	Returns 1 on success, 0 if tree doesn't exist.
	Upon success, tree_edges contains VERTEX_COUNT-1 edges of the spanning tree.
	and *root_vertex contains zero-based tree root vertex number.
*/
int find_spanning_tree(char adj_matrix[VERTEX_MAX][VERTEX_MAX], int best_edges[VERTEX_MAX][2], int max_depth, int* root_vertex) {
	Sets dset; 
	int i = 0, max_depth_here, en, v1, v2, cur_vert, best_root = 0, check_vertexes = 0;
	int min_depth = VERTEX_MAX * VERTEX_MAX, next_vcnt = 0, depth = 0;
	int tree_edges[VERTEX_MAX][2];
	int vertex_depth[VERTEX_MAX];
	int check_vertex[VERTEX_MAX*2];

	memset(best_edges, 0, VERTEX_MAX*2*sizeof(int));
	      
	/* lets check depth from every vertex as starting node, then we will choose the best */
	for (cur_vert=0; cur_vert<VERTEX_COUNT; cur_vert++) {
		set_init(&dset, VERTEX_COUNT);		
		next_vcnt = 1;
		depth = 0;
		check_vertexes = 0;
		memset(check_vertex, -1, VERTEX_MAX*2*sizeof(int));
		memset(vertex_depth, 0, VERTEX_MAX*sizeof(int));
		vertex_depth[cur_vert] = 1;  /* last vertex is starting vertex (root) */
		check_vertex[0] = cur_vert;
		memset(tree_edges, 0, VERTEX_MAX*2*sizeof(int));
		en = 0;
		max_depth_here = 0;
		
		/*printf("\nroot vert: %d\n", cur_vert+1); */
		 
		/* do we have any vertexes, recently connected to last edge? */
		while (next_vcnt > 0) {
			next_vcnt = 0;
			/*will take all vertex, edged with current one by one*/
			for (i=0; i<VERTEX_COUNT; i++)
				if (check_vertex[i] != -1) {
					v1 = check_vertex[i];
					depth = vertex_depth[v1]; /*current depth of the vertex*/
					check_vertex[i] = -1; /*we will use current vertex, so it no more needed*/
					next_vcnt = 1;
					break;
				}
			/*printf("[%d] ",v1+1);*/
			if (next_vcnt==0) break;
			/* searching for any edge from recently added vertex v1 */
			for (v2=0; v2<VERTEX_COUNT; v2++) {
				if (adj_matrix[v1][v2]!=0) {
					int v1_s = set_find(&dset, v1);
					int v2_s = set_find(&dset, v2);
					
					/* is vertex v2 is new to this graph? */
					if (v1_s != v2_s) {
						/*printf("%d(%d) ",v2+1,depth+1); */
						vertex_depth[v2] = depth+1;
						check_vertexes++;
						check_vertex[check_vertexes] = v2;
						if ((depth+1)>max_depth_here) /* what is maximum current depth? */
							max_depth_here = depth+1;
						set_union(&dset, v1_s, v2_s); 
						tree_edges[en][0] = v1;
						tree_edges[en][1] = v2;
						en++;
					}	
				}					
			}
		}
		/*printf("cur depth: %d \n", depth);*/
		/* if depth satisfyes us and we connects all vertexes, then we choose if this solution is best */
		if ((max_depth_here<min_depth) && (en==(VERTEX_COUNT-1))) {
			/*printf("\nbest depth: %d root %d en %d \n", max_depth_here, cur_vert+1, en); */
			min_depth = max_depth_here;
			memcpy(best_edges, tree_edges, VERTEX_MAX*2*sizeof(int));
			best_root = cur_vert;
		}
		
		set_clear(&dset);
	}
	
	printf("min_depth found: %d\n", min_depth - 1);
		   
	/* test max_depth condition */
	if (min_depth>max_depth)
		return 0;

	*root_vertex = best_root + 1;		
	return 1;
}

/* check for the end of the input file */
void check_feof(FILE *f) {
	if (feof(f))  { 
		fprintf(stderr, "got end of input. need data to be set");
        exit(EXIT_FAILURE); 
    }
}

int main(int argc, char* argv[])
{
	int i, vertexes, edges, v1, v2, max_depth, root_vertex, func_ret; 
	char skip[100];
	char adj_matrix[VERTEX_MAX][VERTEX_MAX];
	int tree_edges[VERTEX_MAX][2]; 
	FILE * fh;

	/* check input arguments count */
    if(argc!=3) { 
		fprintf(stderr, "usage: %s input.txt max_depth", argv[0]);
        exit(EXIT_FAILURE); 
    }
        
	fh = fopen (argv[1], "r");
    if (0 == fh) {
        perror("open input file error");
        exit(EXIT_FAILURE); 
    }
	
	max_depth = atoi(argv[2]);
	printf("max_depth = %d\n", max_depth);

	func_ret = fscanf(fh, "%d %d ", &vertexes, &edges);
	if (func_ret!=2) {
			fprintf(stderr, "input file read error: need vertex and edge data");
			exit(EXIT_FAILURE); 			
	}	
	
	/* skip vertex names */
	for (i=0; i<vertexes; i++) {
		check_feof(fh);
		fgets(skip, 100, fh);
	}

	/* read edges data */
	for (i=0; i<edges; i++) {
		check_feof(fh);
		func_ret = fscanf(fh,"%d %d ", &v1, &v2);
		if (func_ret!=2) {
			fprintf(stderr, "input file read error: need edge data: Vertex1 Vertex2");
			exit(EXIT_FAILURE); 			
		}		
		v1--;
		v2--;
		adj_matrix[v1][v2] = 1;
		adj_matrix[v2][v1] = 1;
	}
		
    fclose(fh);

	VERTEX_COUNT = vertexes;	 	

	/* max_depth + 1 cause depth started from 1, not 0 */	
	func_ret = find_spanning_tree(adj_matrix, tree_edges, max_depth + 1, &root_vertex); 
	printf("func ret = %d\n", func_ret);
	printf("root = %d\n", root_vertex);
	
	/* if search got success, we will output edge data*/
	if (func_ret)
		print_basic_edge(tree_edges);
	
    return 0;
}
