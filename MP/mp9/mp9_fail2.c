#include <stdint.h>
#include <stdio.h>

#include "mp5.h"
#include "mp9.h"

#define MY_INFINITY   1000000000

void 
swap(graph_t* g, int32_t* elt, int32_t i, int32_t largest){
    int32_t t = *(elt + i);                                     // swap the elements with index i and largest in heap
    int32_t index, index1, index2;
    *(elt + i) = *(elt + largest);  
    *(elt + largest) = t;
    for (index = 0; index <= g->n_vertices - 1; index++){       // then swap the two vertex's heap_id
        if ((g->vertex + index)->heap_id == i){
            index1 = index;
            break;
        }
    }
    for (index = 0; index <= g->n_vertices - 1; index++){
        if ((g->vertex + index)->heap_id == largest){
            index2 = index;
            break;
        }
    }
    (g ->vertex + index1) ->heap_id = largest;
    (g ->vertex + index2) ->heap_id = i;
}

void 
heaplify(graph_t* g, int32_t* elt, int32_t n, int32_t i)
{
    int32_t smallest = i;                                   // Initialize smallest as root
    int32_t l = 2*i + 1;                                    // left = 2*i + 1
    int32_t r = 2*i + 2;                                    // right = 2*i + 2
    if (l < n && *(elt + l) < *(elt + smallest)){           // let largest = the offset of largest one
        smallest = l;
    }
    if (r < n && *(elt + r) < *(elt + smallest)){           
        smallest = r;
    }
    if (smallest == i)                                      // If largest is not root, swap them
    {
        return;
    }
    swap(g, elt, i, smallest);
    heaplify(g, elt, n, smallest);                          // recur to check subtree
}

void
heap_initialization(graph_t* g, heap_t* h){
    int32_t index;
    for (index = (h->n_elts)/2 - 1; index >=0; index--){    // heaplify node that has children
        heaplify(g, h->elt,h->n_elts,index);
    }
}

int32_t
remove_closest_unvisited_vertex(graph_t* g, heap_t* h){
    int32_t result, index;                                  // return the top element's index
    for (index = 0; index <= g->n_vertices - 1; index++){
        if ((g->vertex + index)->heap_id == 0){
            result = index;
        }
    }
    swap(g, h->elt, 0, h->n_elts - 1);                      // swap the top elemnt with the last element to remove it
    (g->vertex + result)->heap_id = -1;                     // reset the removed vertex heap_id
    h->n_elts--;
    return result;
}

void
find_nodes (locale_t* loc, vertex_set_t* vs, pyr_tree_t* p, int32_t nnum)
{
    // Do not modify the following line nor add anything above in the function.
    record_fn_call ();
    int32_t x, y, quadrant, num;
    if (vs -> count > MAX_IN_VERTEX_SET - 1){               // if having identify MAX_IN_VERTEX_SET vertices, stop
        return;
    }
    if (4 * nnum + 1 >= p -> n_nodes){                      // if encounter a leaf node, check if this graph vertex in range
        x = (p -> node + nnum) -> x;                        // let (x, y_left) represent graph vertix
        y = (p -> node + nnum) -> y_left;
        if (in_range(loc, x, y)){                           // if in the range, fill vs with array indix and increment count
            vs -> id[vs -> count] = (p -> node + nnum) -> id;
            vs -> count++;
        }
        return;                                             // return as this node has no children         
    }
    for (quadrant = 1; quadrant <= 4; quadrant++){          // quadrant = 1,2,3,4 represent up_left, up_right, down_left, down_right respectively
        switch(quadrant){
            case 1:
            num = 4*nnum + 1;
            break;
            case 2:
            num = 4*nnum + 2;
            break;
            case 3:
            num = 4*nnum + 3;
            break;
            case 4:
            num = 4*nnum + 4;
            break;            
        }
        if (((4*num +1) < p -> n_nodes) || (((4*num +1) >= p -> n_nodes) && in_range(loc, (p -> node + num) -> x, (p -> node + num) -> y_left))){
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
    int32_t visit[g->n_vertices];                                       // array visit denote the visited condition of each vertex, 0 means unvisited, 1 means visited
    int32_t end_index, start_index;
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
    for (index = 0; index <= src->count -1; index++){                   // push all start points into heap
        h->n_elts++;
        *(h->elt + index) = (g->vertex + src->id[index])->from_src;
        (g->vertex + src->id[index])->heap_id = index;
    }
    while(visit_num <= g->n_vertices){
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
                        *(h->elt + h->n_elts) = vertex_nb -> from_src;                              
                        vertex_nb->heap_id = h->n_elts;
                        h->n_elts++;   
                    } else {
                        *(h->elt + vertex_nb->heap_id) = vertex_nb -> from_src;                 // if in the heap, update it
                    }
                }
            }
        }
    }
    min_dit = (g->vertex + dest->id[0])->from_src;                      // let the vertex in dest with smallest from_src be the end point
    printf("%d", min_dit);
    end_index = dest->id[0];
    for (index = 0; index <= dest->count - 1; index++){
        if ((g->vertex + dest->id[index])->from_src < min_dit){         // let the end vertex be the vertex in destination with minimal from_src
            min_dit = (g->vertex + dest->id[index])->from_src;
            end_index = dest->id[index];
        }
    }
    path->tot_dist = min_dit;                                           // total distance is min_dit
    path->id[0] = end_index;
    path->n_vertices = 1;
    check = 0;
    start_index = end_index;
    while (!check){
        start_index = (g->vertex + start_index)->pred;                  // add the pred of current vertex to the start of path
        path->n_vertices++;
        if (path->n_vertices > MAX_PATH_LENGTH){                        // if path requires more than MAX_PATH_LENGTH nodes, return 0
            return 0;
        }
        for(index = path->n_vertices - 1; index >= 1; index--){
            path->id[index] = path->id[index - 1];
        }
        path->id[0] = start_index;
        for (index = 0; index <= src->count - 1; index++){              // check if the start point in the src
            check = check || (start_index == src->id[index]);
        }
    }
    return 1;                                                           // return 1 means find a path successfully
}

