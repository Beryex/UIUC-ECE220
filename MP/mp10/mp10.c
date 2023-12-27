#include <stdint.h>
#include <stdio.h>

#include "mp10.h"

/*
    This file includes four functions. mark_vertex_minimap can mark each graph
    vertex in g with a minimap bit number based on the pyramid tree p. And 
    build_vertex_set_minimap builds a minimap for a vertex set. build_path_minimap
    builds a minimap for a path. And merge_vertex_sets can find the intersection part
    of two vertex sets into a third vertex set.
*/

int32_t 
mark_vertex_minimap (graph_t* g, pyr_tree_t* p)                         
{
    int32_t index1, index2, leaf_start, leaf_end;
    if (g->n_vertices <= 64){                                           // if has 64 vertices or fewer, use each vertex's index for mm_bit
        for (index1 = 0; index1 <= g->n_vertices - 1; index1++){
            (g->vertex + index1)->mm_bit = index1;
        }
    } else {
        for (index1 = 21; index1 <= 84; index1++){                      // the forth level of the pyr_tree
            index2 = index1;
            while (4*index2 + 1 < p->n_nodes){                          // find the leaf node start index
                index2 = 4 * index2 + 1;       
            }
            leaf_start = index2;
            index2 = index1;
            while (4*index2 + 1 < p->n_nodes){                          // find the leaf node end index
                index2 = 4 * index2 + 4;       
            }
            leaf_end = index2;
            for (index2 = leaf_start; index2 <= leaf_end; index2++){    // each vertex's mm_bit equal to their forth level pry_tree node index
                if (index2 <= p->n_nodes - 1){
                    (g->vertex + (p->node + index2)->id)->mm_bit = index1 - 21;
                }
            }
        }
    }
    return 1;
}


void 
build_vertex_set_minimap (graph_t* g, vertex_set_t* vs)
{
    uint64_t con = 1;
    uint64_t result = 0;
    uint64_t bit;
    int32_t index;
    for (index = 0; index <= vs->count - 1; index++){
        bit = con << ((g->vertex + vs->id[index])->mm_bit);             // compute the bit vector of each vertex in vertex_set
        result = result | bit;                                          // combine of different bit vector
    }
    vs->minimap = result;
}


void 
build_path_minimap (graph_t* g, path_t* p)
{
    uint64_t con = 1;
    uint64_t result = 0;
    uint64_t bit;
    int32_t index;
    for (index = 0; index <= p->n_vertices - 1; index++){
        bit = con << ((g->vertex + p->id[index])->mm_bit);              // compute the bit vector of each vertex in path
        result = result | bit;                                          // combine of different bit vector
    }
    p->minimap = result;
}


int32_t
merge_vertex_sets (const vertex_set_t* v1, const vertex_set_t* v2,
		   vertex_set_t* vint)
{
    int32_t num, index1, index2, index3;
    index1 = index2 = index3 = 0;
    if (v1->count <= v2->count){                                        // the length of intersection is the smaller one
        num = v1->count;
    } else {
        num = v2->count;
    }
    while((index3 <= num - 1) && (index1 <= v1->count - 1) && (index2 <= v2->count - 1)){
        if (v1->id[index1] == v2->id[index2]){                          // as both vertex set sorted in invreasing order, compare them in increasing order
            vint->id[index3] = v1->id[index1];
            index1++;
            index2++;
            index3++;
            vint->count++;
        } else if (v1->id[index1] < v2->id[index2]){
            index1++;
        } else if (v1->id[index1] > v2->id[index2]){
            index2++;
        }
    }
    if (vint->count != 0){                                              // if intersection not empty, return 1, else, return 0
        return 1;
    } else {
        return 0;
    }
}

