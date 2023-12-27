#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "mp5.h"
#include "mp10.h"

/*
    This project can handle some requests and pair them up depending
    on whether they have a common source places and destination, and 
    then stored them in a singly-linked list, and store unpaired request
    in another singly-linked list. handle_request can handle one request
    at each time.
*/

//
// These variables hold the heads of two singly-linked lists of 
// requests.  
//
// The avaialble list consists of unpaired requests: partner 
// should be NULL, and next is used to form the list.   
// 
// The shared list consists of groups with non-empty start and end 
// vertex intersections.  Only one of the requests in the group is in 
// the list.  The others are linked through the first's partner field 
// and then through the next field of the others in the group.  The 
// next field of the first request in a group is used to form the
// shared list.
//
// Note: for MP2, you should only build groups of two in the shared
// list.
//

static request_t* available = NULL;
static request_t* shared = NULL;


int32_t
handle_request (graph_t* g, pyr_tree_t* p, heap_t* h, request_t* r)
{
    vertex_set_t* src;
    vertex_set_t* dest;
    path_t* path;
    vertex_set_t* test_src;
    vertex_set_t* test_dest;
    vertex_set_t* co_src;
    vertex_set_t* co_dest;
    void* tem_ptr1;
    void* tem_ptr2;
    request_t** test = &available;
    uint64_t result1, result2;
    request_t* partner;
    src = new_vertex_set();
    dest = new_vertex_set();
    path = new_path();
    if ((src == NULL) || (dest == NULL) || (path == NULL)){             // if any allocation fails, clean up and return 0
        if(src != NULL){
            free_vertex_set(src);
        }
        if(dest != NULL){
            free_vertex_set(dest);
        }
        if(path != NULL){
            free_path(path);
        }
        return 0;
    }
    find_nodes(&(r->from), src, p, 0);                                  // fill in the vertex set of source places and destination
    find_nodes(&(r->to), dest, p, 0);
    if ((src->count == 0) || (dest->count == 0)){                       // if either vertex set is empty, claen up and return 0
        free_vertex_set(src);
        free_vertex_set(dest);
        free_path(path);
        return 0;
    }
    mark_vertex_minimap(g, p);
    build_vertex_set_minimap(g, src);                                   // build up minimaps for source places and destination
    build_vertex_set_minimap(g, dest);
    while(*test != NULL){                                               // find a matching request in the available list
        test_src = (*test)->src_vs;
        test_dest = (*test)->dst_vs;
        result1 = test_src->minimap & src-> minimap;
        result2 = test_dest->minimap & dest-> minimap;
        if (result1 == 0 || result2 == 0){                              // if no overlap in both minimaps, skip this request
            test = &((*test)->next);
            continue;
        }
        co_src = new_vertex_set();
        co_dest = new_vertex_set();
        if (co_src == NULL || co_dest == NULL){                         // if allocation fails, clean up and return 0
            if (co_src != NULL){
                free_vertex_set(co_src);
            }
            if (co_dest != NULL){
                free_vertex_set(co_dest);
            }   
            free_vertex_set(src);
            free_vertex_set(dest);
            free_path(path);
            return 0;
        }
        tem_ptr1 = (int32_t*)realloc(co_src->id, src->count * sizeof(*(src->id)));      // create the id array for common source places and destination
        tem_ptr2 = (int32_t*)realloc(co_dest->id, dest->count * sizeof(*(dest->id)));
        if (tem_ptr1 == NULL || tem_ptr2 == NULL){                                      // if allocation fails, clean up and return 0
            if (tem_ptr1 != NULL){
                free(tem_ptr1);
            }
            if (tem_ptr2 != NULL){
                free(tem_ptr2);
            }
            free_vertex_set(co_src);
            free_vertex_set(co_dest);
            free_vertex_set(src);
            free_vertex_set(dest);
            free_path(path);
            return 0;
        }
        co_src->id = tem_ptr1;
        co_dest->id = tem_ptr2;
        if(merge_vertex_sets(src, test_src, co_src) == 0 || merge_vertex_sets(dest, test_dest, co_dest) == 0){
            free_vertex_set(co_src);                                    // if interction is empty, clean up and skip this request
            free_vertex_set(co_dest);
            test = &((*test)->next);
            continue;
        }
        build_vertex_set_minimap(g, co_src);
        build_vertex_set_minimap(g, co_dest);
        if(dijkstra(g, h, co_src, co_dest, path) == 0){                 // if dijkstra fails, clean up and skip this request
            free_vertex_set(co_src);                                                                       
            free_vertex_set(co_dest);
            test = &((*test)->next);
            continue;
        }
        build_path_minimap(g, path);
        break;                                                          // break as having found a matching request
    }
    if (*test == NULL) {                                                // if not find, add this request to available, and free test vertex set
        if (available == NULL){
            available = r;
            r->next = NULL;
        } else {
            r->next = available;
            available = r;
        }                                                      
        r->partner = NULL;                                              // update relevant fields
        r->src_vs = src;                                   
        r->dst_vs = dest;
        r->path = NULL;
        free_path(path);
        return 1;
    } else {
        partner = *test;                                                // if find, remove the matching request from available
        *test = (*test)->next;
        free_vertex_set(partner->src_vs);                               // free both requests vertex sets and other test vertex set
        free_vertex_set(partner->dst_vs);
        free_vertex_set(src);
        free_vertex_set(dest);
        partner->src_vs = r->src_vs = co_src;                           // update relevant fields
        partner->dst_vs = r->dst_vs = co_dest;
        partner->path = r->path = path;
        r->partner = partner;
        partner->partner = NULL;
        partner->next = NULL;
        if (shared == NULL){                                            // add request r to the shared list
            shared = r;
            r->next = NULL;
        } else {
            r->next = shared;
            shared = r;
        }
        return 1;
    }
}


void
print_results ()
{
    request_t* r;
    request_t* prt;

    printf ("Matched requests:\n");
    for (r = shared; NULL != r; r = r->next) {
        printf ("%5d", r->uid);
	for (prt = r->partner; NULL != prt; prt = prt->next) {
	    printf (" %5d", prt->uid);
	}
	printf (" src=%016lX dst=%016lX path=%016lX\n", r->src_vs->minimap,
		r->dst_vs->minimap, r->path->minimap);
    }

    printf ("\nUnmatched requests:\n");
    for (r = available; NULL != r; r = r->next) {
        printf ("%5d src=%016lX dst=%016lX\n", r->uid, r->src_vs->minimap,
		r->dst_vs->minimap);
    }
}


int32_t
show_results_for (graph_t* g, int32_t which)
{
    request_t* r;
    request_t* prt;

    // Can only illustrate one partner.
    for (r = shared; NULL != r; r = r->next) {
	if (which == r->uid) {
	    return show_find_results (g, r, r->partner);
	}
	for (prt = r->partner; NULL != prt; prt = prt->next) {
	    if (which == prt->uid) {
		return show_find_results (g, prt, r);
	    }
	}
    }

    for (r = available; NULL != r; r = r->next) {
        if (which == r->uid) {
	    return show_find_results (g, r, r);
	}
    }
    return 0;
}


static void
free_request (request_t* r)
{
    free_vertex_set (r->src_vs);
    free_vertex_set (r->dst_vs);
    if (NULL != r->path) {
	free_path (r->path);
    }
    free (r);
}

void
free_all_data ()
{
    request_t* r;
    request_t* prt;
    request_t* next;

    // All requests in a group share source and destination vertex sets
    // as well as a path, so we need free those elements only once.
    for (r = shared; NULL != r; r = next) {
	for (prt = r->partner; NULL != prt; prt = next) {
	    next = prt->next;
	    free (prt);
	}
	next = r->next;
	free_request (r);
    }

    for (r = available; NULL != r; r = next) {
	next = r->next;
	free_request (r);
    }
}


