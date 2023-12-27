#include <stdint.h>
#include <stdio.h>

#include "mp5.h"
#include "mp9.h"

/*
	This function can find a shortest path starting in the common place
	of two people's start places and ending in the common destination 
	places of them. The index of vertex in the path is stored in path->id[]
	and the minimul distance is stored in path->
*/

int32_t
match_requests (graph_t* g, pyr_tree_t* p, heap_t* h,
		request_t* r1, request_t* r2,
		vertex_set_t* src_vs, vertex_set_t* dst_vs, path_t* path)
{
	locale_t* loc1_start = &(r1->from);
	locale_t* loc1_end = &(r1->to);
	locale_t* loc2_start = &(r2->from);
	locale_t* loc2_end = &(r2->to);
	int32_t result;
	src_vs->count = 0;
	find_nodes(loc1_start, src_vs, p, 0);								// find the common vertex in both requests' start places
	trim_nodes(g, src_vs, loc2_start);
	dst_vs->count = 0;
	find_nodes(loc1_end, dst_vs, p, 0);									// find the common vertex in both requests' destination places
	trim_nodes(g, dst_vs, loc2_end);
	if ((src_vs->count == 0) || (dst_vs->count ==0)){					// if source set = 0 or destination set = 0, return 0
        return 0;
	}
	result = dijkstra(g, h, src_vs, dst_vs, path);
	return result;														// return 1 means find a path successfully
}
