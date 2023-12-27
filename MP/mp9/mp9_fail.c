#include <stdint.h>
#include <stdio.h>

#include "mp5.h"
#include "mp9.h"

#define MY_INFINITY   1000000000

void 
swap(int32_t* elt, int32_t i, int32_t largest){
int32_t t = *(elt + i);                                     // swap the elements with index i and largest in heap
*(elt + i) = *(elt + largest);  
*(elt + largest) = t; 
}

void 
heaplify(int32_t* elt, int32_t n, int32_t i)
{
    int32_t largest = i;                                    // Initialize largest as root
    int32_t l = 2*i + 1;                                    // left = 2*i + 1
    int32_t r = 2*i + 2;                                    // right = 2*i + 2
    if (l < n && *(elt + l) > *(elt + largest)){            // let largest = the offset of largest one
        largest = l;
    }
    if (r < n && *(elt + r) > *(elt + largest)){           
        largest = r;
    }
    if (largest != i)                                       // If largest is not root, swap them
    {
        swap(elt, i, largest);
        heaplify(elt, n, largest);                          // recur to check subtree
    }
}

void
heap_initialization(heap_t* h){
    int32_t index;
    for (index = (h->n_elts)/2 - 1; index >=0; index--){    // heaplify node that has children
        heaplify(h->elt,h->n_elts,index);
    }
}

int32_t
remove_closest_unvisited_vertex(heap_t* h){
    int32_t cls_vertex = *(h->elt);                         // replace the top elements in heap with the last element in heap
    *(h->elt) = *(h->elt + h->n_elts - 1);
    h->n_elts--;
    return cls_vertex;
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
            find_nodes (loc, vs, p, num);                 // if next node has no children and it is in range, or next node has no children, recur
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
    int32_t result = 0;
    for (index = 0; index <= g->n_vertices - 1; index++){               // initialize the array visit with all 0
        visit[index] = 0;
    }
    for (index = 0; index <= g->n_vertices - 1; index++){               // initialize every graph vertix in src with from_src = 0 and other with infinity
        (g->vertex + index) -> from_src = MY_INFINITY;
    }
    for (index = 0; index <= src->count - 1; index++){
        (g->vertex + (src->id[index])) -> from_src = 0;
    }
    visiting = src->id[0];                                              // use src->id[0] as a visited start point
    printf("%d\n", visiting);  
    visit[visiting] = 1;
    visit_num++;
    while(visit_num <= g->n_vertices){                          
        check = 1;                                                      // check if the visiting vertex has unvisited neighbor
        for (index = 0; index <= (g->vertex + visiting) -> n_neighbors - 1; index++){
            check = check & visit[*((g->vertex + visiting) -> neighbor + index)];
        }
        if (check == 1 || visit[visiting] == 0){                                                 // if yes, mark this vertex visited, increment visit_num
            visit[visiting] = 1;
            visit_num++;
            min_dit = (g->vertex + visiting) -> from_src;
            for (index = 0; index <= g->n_vertices - 1; index++){       // then let visiting denote the unvisited vertex with least from_src
                if (((g->vertex + index)->from_src < min_dit) && (visit[index] == 0)){
                    visiting = index;
                    min_dit = (g->vertex + index)->from_src;
                } 
            }
            continue;
        }
        h -> n_elts = 0;                                                // clear the heap
        for (index = 0; index <= g->n_vertices - 1; index++){
            (g->vertex + index) -> heap_id = -1;                            // initialize every graph vertix heap_id with -1
        }
        for (index = 0; index <= (g->vertex + visiting) -> n_neighbors - 1; index++){           // update each unvisited neighbor's from_src
            if (visit[*((g->vertex + visiting) -> neighbor + index)] == 0){
                vertex = g->vertex + visiting;
                vertex_nb = g->vertex + *((g->vertex + visiting) -> neighbor + index);
                if ((vertex->from_src + *(vertex->distance + index)) < vertex_nb -> from_src){  // if from_src is smaller, update it and set predecessor this vertex
                    vertex_nb -> from_src = vertex->from_src + *(vertex->distance + index);
                    vertex_nb -> pred = visiting;
                }
                *(h->elt + h->n_elts) = vertex_nb -> from_src;                                  // push the neighbors of this vertex into heap and mark their heap_id
                h->n_elts++;
            }
        }
        heap_initialization(h);                                         // after initialization, the top vertex in the heap has the smallest from_src
        min_dit = remove_closest_unvisited_vertex(h);                   // remove the closest unvisited verstex and record the minimal distance
        visit[visiting] = 1;                                            
        visit_num++;
        for (index = 0; index <= (g->vertex + visiting) -> n_neighbors - 1; index++){
            vertex_nb = g->vertex + *((g->vertex + visiting) -> neighbor + index);
            if (vertex_nb -> from_src == min_dit){                      // let the neighbor with minimal distance be the next visiting
                visiting = *((g->vertex + visiting) -> neighbor + index);
            }
        }
    }
    for (index = 0; index <= g->n_vertices - 1; index++){
        if (visit[index] == 1){
          printf("%d\n", index);
          printf("%d\n", (g->vertex + index)->from_src);
        }
    }
    min_dit = (g->vertex + dest->id[0])->from_src;                      // let the vertex in dest with smallest from_src be the end point
    end_index = dest->id[0];
    for (index = 0; index <= dest->count - 1; index++){
        if ((g->vertex + dest->id[index])->from_src != MY_INFINITY){    // if all vertex in dest has from_src = infinity, fail to find path and return 0
            result = result || 1;
        } else {
            result = result || 0;
        }
        if ((g->vertex + dest->id[index])->from_src < min_dit){         // let the end vertex be the vertex in destination with minimal from_src
            min_dit = (g->vertex + dest->id[index])->from_src;
            end_index = dest->id[index];
        }
    }
    if (result == 0){
        return 0;
    }
    path->tot_dist = min_dit;                                           // total distance is min_dit
    //printf("%d", path->tot_dist);
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

