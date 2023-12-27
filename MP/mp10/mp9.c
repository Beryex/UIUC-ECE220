#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "mp5.h"
#include "mp10.h"

#define MY_INFINITY   1000000000

/*
    This file includes three main functions. find_nodes can find every vertex
    in the range of locale_t and store them into vertex_set_t in an increasing
    order. trim_nodes can remove any vertex that is not in the range of locale_t 
    from vertex_set_t. dijkstra can find a shortest path that starts in src and
    ends in dest, find the minimum distance and store the path vertex into path->id[]. 
    dijkstra uses a heap to implement dijkstra algorithm, it uses several functions 
    to implement operation to the heap.
*/

// swap two elements in the heap
void swap(int32_t* elt, int32_t i, int32_t smallest);

// let the current node be the elemnt with smallest from_src including its children  
void heaplify(graph_t* g, int32_t* elt, int32_t n, int32_t i);

// after initialization, the top elemnt be the element with smallest from_src
void heap_initialization(graph_t* g, heap_t* h);

// return the top element and remove it and decrement h->n_elts
int32_t remove_closest_unvisited_vertex(graph_t* g, heap_t* h);

void 
swap(int32_t* elt, int32_t i, int32_t smallest){
    int32_t t = *(elt + i);                                 // swap the elements with index i and largest in heap
    *(elt + i) = *(elt + smallest);  
    *(elt + smallest) = t;
}

void 
heaplify(graph_t* g, int32_t* elt, int32_t n, int32_t i)
{
    int32_t smallest = i;                                   // Initialize smallest as root
    int32_t l = 2*i + 1;                                    // left = 2*i + 1
    int32_t r = 2*i + 2;                                    // right = 2*i + 2
    if (l < n && ((g->vertex + *(elt + l)) ->from_src) < ((g->vertex + *(elt + smallest)) ->from_src)){ 
        smallest = l;
    }
    if (r < n && ((g->vertex + *(elt + r)) ->from_src) < ((g->vertex + *(elt + smallest)) ->from_src)){           
        smallest = r;
    }
    if (smallest == i)                                      // If largest is not root, swap them
    {
        return;
    }
    swap(elt, i, smallest);
    heaplify(g, elt, n, smallest);                          // recur to check subtree
}

void
heap_initialization(graph_t* g, heap_t* h){
    int32_t index;
    if (h->n_elts >= 2){
        for (index = (h->n_elts)/2 - 1; index >=0; index--){    
            heaplify(g, h->elt,h->n_elts,index);            // heaplify node that has children
        }
    }
}

int32_t
remove_closest_unvisited_vertex(graph_t* g, heap_t* h){
    int32_t result;                                         // return the top element
    result = *(h->elt);
    swap(h->elt, 0, h->n_elts - 1);                         // swap the top elemnt with the last element to remove it
    (g->vertex + result)->heap_id = -1;                     // reset the removed vertex heap_id
    h->n_elts--;
    return result;
}

void
find_nodes (locale_t* loc, vertex_set_t* vs, pyr_tree_t* p, int32_t nnum)
{
    // Do not modify the following line nor add anything above in the function.
    record_fn_call ();
    int32_t x, y, x_left, x_right, y_up, y_down, quadrant, num, check;
    void* tem_ptr;
    uint32_t size;
    int32_t index1, index2;
    x_left = loc->x - loc->range;                           // used to avoid unnecessary recursion 
    x_right = loc->x + loc->range;
    y_up = loc->y - loc->range;
    y_down = loc->y + loc->range;
    if (4 * nnum + 1 >= p -> n_nodes){                      // if encounter a leaf node, check if this graph vertex in range
        x = (p -> node + nnum) -> x;                        // when leaf nodes, let (x, y_left) represent graph vertix
        y = (p -> node + nnum) -> y_left;
        if (in_range(loc, x, y)){                           // if in the range, fill vs with array indix and increment count
            if (vs->id_array_size < (vs->count + 1) * sizeof(*(vs->id))){
                size = vs->id_array_size+sizeof(*(vs->id)); // if there is not enough memory resize the array
                tem_ptr = realloc(vs->id, size);
                if (tem_ptr == NULL){
                    return;                                 // if realloc fails, skip this vertex
                } else {
                    vs->id = (int32_t*)tem_ptr;
                    vs->id_array_size = size;
                }
            }
            index2 = 0;                                     // used in condition vs->count = 0
            for (index1 = vs->count - 1; index1 >= 0; index1--){            // insert vertex into array, and sort arrat in increasing order
                if (index1 == vs-> count - 1){
                    if (vs->id[index1] < (p -> node + nnum) -> id){         // find the proper place to insert
                        index2 = index1 + 1;
                    }
                } else if (index1 == 0) {
                    if (vs->id[index1] >= (p -> node + nnum) -> id){
                        index2 = index1;
                    }
                } else {
                    if ((vs->id[index1+1] >= (p->node+nnum)->id) && (vs->id[index1] < (p->node+nnum)->id)){
                        index2 = index1 + 1;
                    }
                }
            }
            for (index1 = vs->count; index1 >= index2; index1--){           // then insert this vertex
                if (index1 != index2){
                    vs -> id[index1] = vs -> id[index1 - 1];
                } else {
                    vs -> id[index1] = (p -> node + nnum) -> id;
                }
            }
            vs -> count++;
        }
        return;                                             // return as this node has no children         
    }
    for (quadrant = 1; quadrant <= 4; quadrant++){          // quadrant = 1,2,3,4 represent up_left, up_right, down_left, down_right respectively
        check = 0;
        switch(quadrant){
            case 1:
            if (((p->node + nnum)->x > x_left) && ((p->node + nnum)->y_left > y_up)){
                num = 4*nnum + 1;
                check = 1;
            }
            break;
            case 2:
            if (((p->node + nnum)->x < x_right) && ((p->node + nnum)->y_right > y_up)){
                num = 4*nnum + 2;
                check = 1;
            }
            break;
            case 3:
            if (((p->node + nnum)->x > x_left) && ((p->node + nnum)->y_left < y_down)){
                num = 4*nnum + 3;
                check = 1;
            }
            break;
            case 4:
            if (((p->node + nnum)->x < x_right) && ((p->node + nnum)->y_right < y_down)){
                num = 4*nnum + 4;
                check = 1;
            }
            break;            
        }
        if (4 * num + 1 >= p->n_nodes){                     // used to avoid unnecessary recursion, if next node is leaf node, check if it in range
            check = check && in_range(loc, (p->node + num)->x, (p->node + num)->y_left);
        }
        if (check == 1){
            find_nodes (loc, vs, p, num);                   // if next node has no children and it is in range, or next node has no children, recur
        }
    }
}

void
trim_nodes (graph_t* g, vertex_set_t* vs, locale_t* loc)
{
    int32_t index_set; 
    vertex_t* index_graph;
    int32_t x, y, index;
    for (index_set = vs -> count - 1; index_set >= 0; index_set--){     // check every graph vertex in vertex_set_t
        index_graph = g -> vertex + vs -> id[index_set];
        x = index_graph -> x;
        y = index_graph -> y;
        if (!(in_range(loc, x, y))){                                    // if a graph vertex is out of range of a locale, remove it from vertex set
            for (index = index_set; index <= vs -> count - 2; index++){
                vs -> id[index] = vs -> id[index + 1];
            }
            vs -> count--;                                              // decrement count   
        }
    }
}


int32_t
dijkstra (graph_t* g, heap_t* h, vertex_set_t* src, vertex_set_t* dest,
          path_t* path)
{
    int32_t index, visiting, visit_num = 0;                             // visiting means the vertex index that is visiting, visit_num means the number that has been visited
    int32_t check, min_dit;
    vertex_t* vertex;                                                   // vertex denote the visiting graph vertex, vertex_nb denote the visiting graph neighbor vertex
    vertex_t* vertex_nb;
    int32_t* visit;                                                     // array visit denote the visited condition of each vertex, 0 means unvisited, 1 means visited
    int32_t end_index, start_index;
    void* tem_ptr;
    int32_t num = 1;
    tem_ptr = malloc(g->n_vertices * sizeof(int32_t));
    if (tem_ptr == NULL){
        return 0;
    }
    visit = tem_ptr;
    for (index = 0; index <= g->n_vertices - 1; index++){               // initialize the array visit with all 0
        visit[index] = 0;
    }
    for (index = 0; index <= g->n_vertices - 1; index++){               // initialize every graph vertix in src with from_src = 0 and other with infinity
        (g->vertex + index) -> from_src = MY_INFINITY;
    }
    for (index = 0; index <= src->count - 1; index++){
        (g->vertex + (src->id[index])) -> from_src = 0;
    }
    h -> n_elts = 0;                                                    // clear the heap
    for (index = 0; index <= g->n_vertices - 1; index++){
        (g->vertex + index) -> heap_id = -1;                            // initialize every graph vertix heap_id with -1
    }
    for (index = 0; index <= src->count -1; index++){                   // push all start points' index into heap
        h->n_elts++;
        *(h->elt + index) = src->id[index];
        (g->vertex + src->id[index])->heap_id = 0;                      // 0 means in the heap
    }
    while(visit_num <= g->n_vertices && h->n_elts >= 1){
        heap_initialization(g, h);                                      // let the top element of heap be the vertex with smallest from_src
        visiting = remove_closest_unvisited_vertex(g, h);               // remove the closest unvisited verstex and let it be the visiting point
        visit_num++;
        visit[visiting] = 1;
        for (index = 0; index <= (g->vertex + visiting) -> n_neighbors - 1; index++){           // update each unvisited neighbor's from_src
            if (visit[*((g->vertex + visiting) -> neighbor + index)] == 0){
                vertex = g->vertex + visiting;
                vertex_nb = g->vertex + *((g->vertex + visiting) -> neighbor + index);
                if ((vertex->from_src + *(vertex->distance + index)) < vertex_nb -> from_src){  // if from_src is smaller, update it and set predecessor this vertex
                    vertex_nb -> from_src = vertex->from_src + *(vertex->distance + index);
                    vertex_nb -> pred = visiting;
                    if (vertex_nb->heap_id == -1){                                              // if not in the heap, push the neighbors of this vertex into heap
                        *(h->elt + h->n_elts) = *((g->vertex + visiting) -> neighbor + index);                              
                        vertex_nb->heap_id = 0;
                        h->n_elts++;   
                    }
                }
            }
        }
    }
    min_dit = (g->vertex + dest->id[0])->from_src;                      // let the vertex in dest with smallest from_src be the end point
    end_index = dest->id[0];
    for (index = 0; index <= dest->count - 1; index++){
        if ((g->vertex + dest->id[index])->from_src <= min_dit){        // let the end vertex be the vertex in destination with minimal from_src
            min_dit = (g->vertex + dest->id[index])->from_src;
            end_index = dest->id[index];
        }
    }
    path->tot_dist = min_dit;                                           // total distance is min_dit
    if (min_dit == MY_INFINITY){
        return 0;                                                       // if not reaching the destination, return 0
    }
    check = 0;
    start_index = end_index;
    while (!check){                                                     // find the number of length of the path
        start_index = (g->vertex + start_index)->pred;                 
        num++;
        for (index = 0; index <= src->count - 1; index++){
            check = check || (start_index == src->id[index]);
        }
    }
    tem_ptr = realloc(path->id, num * (sizeof(*(path->id))));
    if (tem_ptr == NULL){
        return 0;                                                       // if malloc fails, return 0
    }
    path->id = tem_ptr;
    path->id[0] = end_index;
    path->n_vertices = 1;
    check = 0;
    start_index = end_index;
    while (1){
        for (index = 0; index <= src->count - 1; index++){              // check if the start point in the src
            check = check || (start_index == src->id[index]);
        }
        if(check == 1){
            break;
        }
        start_index = (g->vertex + start_index)->pred;                  // add the pred of current vertex to the start of path
        path->n_vertices++;
        for(index = path->n_vertices - 1; index >= 1; index--){
            path->id[index] = path->id[index - 1];
        }
        path->id[0] = start_index;
    }
    return 1;                                                           // return 1 means find a path successfully
}