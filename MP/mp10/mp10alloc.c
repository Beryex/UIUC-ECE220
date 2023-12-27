#include <stdint.h>
#include <stdlib.h>

#include "mp10.h"

/*
    This files contain four functions. new_vertex_set can allocate
    a new vertex set. free_vertex_set can free a vertex set. new_path
    can allocate a new path. free_path can free a path and its id
    array.
*/

vertex_set_t*
new_vertex_set ()
{
    vertex_set_t* tem_ptr1;
    int32_t* tem_ptr2;
    tem_ptr1 = (vertex_set_t*)malloc(sizeof(vertex_set_t));             // create a vertex set
    if (tem_ptr1 == NULL){
        return NULL;
    } else {
        tem_ptr2 = (int32_t*)malloc(sizeof(*(tem_ptr1->id)));           // create the id array inside it
        if (tem_ptr2 == NULL){
            free(tem_ptr1);
            return NULL;
        } else {
            tem_ptr1->id = tem_ptr2;                                    // initialize some fields in vertex_set_t
            tem_ptr1->id_array_size = sizeof(*(tem_ptr1->id));
            tem_ptr1->count = 0;
            return tem_ptr1;
        }
    }
}


void
free_vertex_set (vertex_set_t* vs)
{
    if (vs->id != NULL) {                                               // if the id not NULL, free it
        free(vs->id);
    }
    free(vs);
}


path_t*
new_path ()
{
    path_t* tem_ptr1;
    int32_t* tem_ptr2;
    tem_ptr1 = (path_t*)malloc(sizeof(path_t));                         // create a path
    if (tem_ptr1 == NULL){
        return NULL;
    } else {
        tem_ptr2 = (int32_t*)malloc(sizeof(*(tem_ptr1->id)));           // create the id array inside it
        if (tem_ptr2 == NULL){
            free(tem_ptr1);
            return NULL;
        } else {
            tem_ptr1->id = tem_ptr2;                                    // initialize some fields in path
            tem_ptr1->n_vertices = 0;
            tem_ptr1->tot_dist = 0;
            return tem_ptr1;
        }
    }
}


void
free_path (path_t* path)
{
    if (path->id != NULL) {                                             // if the id not NULL, free it
        free(path->id);
    }
    free(path);
}

