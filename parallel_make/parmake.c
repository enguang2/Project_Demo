/**
 * parallel_make
 * CS 241 - Fall 2021
 */

#include "format.h"
#include "graph.h"
#include "parmake.h"
#include "parser.h"
#include "assert.h"
#include "set.h"
#include "queue.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <pthread.h>

#define cyclic_node 2;
#define good_node 1;
#define uninitialized_node 0;
#define error_node -1;
//#define zero = 0;
//#define one = 1;
// -1: error found, should not run, and need to be marked as fail
//  0: dependencies not finished yet, check later
//  1: all dependecies are satisfied, should run immediately
//  2: should not run, and need to be marked as satisfied
//  3: rule is already done, don't do anything

//indicator in dict d
static int one = 1;
static int zero = 0;
static size_t thread_count = 0;
static queue *q = NULL;
static dictionary *d = NULL;
static graph* g;
//set will be deprecated for part2
static set* visited = NULL;
void print_all_vertex(graph* g);
bool isCyclic(char* target);
bool vertex_is_descendent(void* vertex_son, void* vertex_parent, graph* g);
void mark_vertex_as(int node_type);
void reset_dict();
void *execute(void *target);
void add_task(char* target);


//cond variable and mutex
pthread_cond_t cv = PTHREAD_COND_INITIALIZER;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

void print_set(set* s) {
    if (s == NULL) {
        printf("Null set\n");
        return;
    }
    printf("The number of elements are %zu\n", set_cardinality(s));
    vector* vec = set_elements(s);
    for (size_t i = 0; i < vector_size(vec); i++) {
        printf("element is: %s\n", (char*)vector_get(vec, i));
    }
}

void mark_vertex_as(int node_type) {
    vector* vertex = graph_vertices(g);
    size_t total = graph_vertex_count(g);
    for (size_t i = 0; i < total; i++) {
        rule_t* curr_rule = graph_get_vertex_value(g, vector_get(vertex, i));
        curr_rule->state = node_type;
    }
    return;
}

//check if target itself is in a loop
//mark self as visited, then travsere all children
bool inCyclic(void* target) {
    //printf("cyclic check starts");
    //print_set(visited);
    //printf("Current vertex is %s\n", (char*)target);
    if (NULL == visited) {
        visited = shallow_set_create();
    }
    if (set_contains(visited, target)) {
        //two-times access
        
        visited = NULL;
        return true;
    } else {
        //normal 
        set_add(visited, target);
        //print_set(visited);
        vector* neighbors = graph_neighbors(g, target);
        size_t neighbor_size = vector_size(neighbors);
        for (size_t i = 0; i < neighbor_size; i++) {
            if (true == inCyclic(vector_get(neighbors, i))) {
                //printf("get a match\n");
                //print_set(visited);
                visited = NULL;
                return true;
            }
        }
        vector_destroy(neighbors);
        //no match for all children, pop this node out
        set_remove(visited, target);
    }
    //visited = NULL;
    //printf("never here");
    return false;
}

// void print_all_vertex(graph* g) {
//     //printf("now all vertex\n");
//     vector* vec = graph_vertices(g);
//     for (size_t i = 0; i < vector_size(vec); i++) {
//         //printf("The node is %s, and ptr is %p \n", (char*)vector_get(vec, i), vector_get(vec, i));
//     }
// }

// bool vertex_is_descendent(void* vertex_son, void* vertex_parent, graph* g) {
//     //start from parent and do DFS
//     //printf("The son we pass is %p, and parent is %p \n", vertex_son, vertex_parent);
//     //printf("The son we pass is %s, and parent is %s \n", (char*)vertex_son, (char*)vertex_parent);
//     vector* parent_neighbors = graph_neighbors(g, vertex_parent);

//    if (strcmp((char*)vertex_parent, (char*)vertex_son) == 0) {
//        //printf("direct same");
//        return true;
//    }

//     //recursion check
//     for (size_t i = 0; i < vector_size(parent_neighbors); i++) {
//         //direct son
        
//         if (true == vertex_is_descendent(vertex_son, vector_get(parent_neighbors, i), g)) {
//             //printf("yse get here!");
//             return true;
//         }
//     }
//     return false;
// }

// //if target is a noncyclic goal vertex, mark all its descendents as good
// void mark_as_good_vertex(void* target) {
//     rule_t* curr_rule = graph_get_vertex_value(g, target);
//     curr_rule->state = good_node;
//     vector* children = graph_neighbors(g, target);
//     size_t children_size = vector_size(children);

//     for (size_t i = 0; i < children_size; i++) {
//         mark_as_good_vertex(vector_get(children, i));
//     }
//     return;
// }

//compare if same vertex cause we use shallow copy
// bool same_vertex(void* a, void* b) {
//     if (strcmp((char*)a, (char*)b) == 0) {
//         return true;
//     }
//     return false;
// }

// -1: error found, should not run, and need to be marked as fail
//  0: dependencies not finished yet, check later
//  1: all dependecies are satisfied, should run immediately
//  2: should not run, and need to be marked as satisfied
//  3: rule is already done, don't do anything

//recursively call nodes, return 0 if success, 1 if fail

/*
//deprecated version of execute_rule in a recursive way
int execute_rule(void* target) {
    //check timestamp and stuff
    //TODO, if files arenot updated, then don't run
    //set the state to make sure only run once 
    
    //if already satisfied, then skip running
    //printf("Current target is %s\n", (char*)target);
    rule_t* rule = graph_get_vertex_value(g, target);
    if (rule->state == 3) {
        return 3;
    }
    if (rule->state == -1) {
        //printf("This target cannot be run %s", (char*)target);
        return -1;
    }


    if (graph_vertex_degree(g, target) != 0) {
        vector* curr_neighbors = graph_neighbors(g, target);
        bool some_child_fail = false;
        for (size_t i = 0; i < vector_size(curr_neighbors); i++) {
            if (execute_rule(vector_get(curr_neighbors, i)) == -1) {
                //one of dependent rule fails
                some_child_fail = true;
                rule_t* rule = graph_get_vertex_value(g, target);
                rule->state = -1;
            }
        }
        vector_destroy(curr_neighbors);
        if (some_child_fail == true) {
            return -1;
        }
    }   
        //NO NO

        //if the rule a name of disk?
        if (access(target, F_OK) == 0) {
            //rule is name of a file on disk
            //all dependencies are file on disk?
            bool all_file = true;
            vector* curr_neighbors = graph_neighbors(g, target);
            for (size_t i = 0; i < vector_size(curr_neighbors); i++) {
                void* child = vector_get(curr_neighbors, i);
                if (access(child, F_OK) == -1) {
                    //depend on another rule that is not a file
                    all_file = false;
                    break;
                }

            }
            
            if (all_file == true) {
                //file depends on all files, check timestamp
                bool sub_is_older = true;
                for (size_t i = 0; i < vector_size(curr_neighbors); i++) {
                    struct stat stat_0, stat_1;
                    stat((char *)target, &stat_0);
                    stat(vector_get(curr_neighbors, i), &stat_1);
                    if (difftime(stat_0.st_mtime, stat_1.st_mtime) < 0) {
                        //any sub is modeified after target
                        sub_is_older = false;
                        break;
                    }
                }
                if (sub_is_older == true) {
                    rule_t* rule = graph_get_vertex_value(g, target);
                    rule->state = 3;
                    vector_destroy(curr_neighbors);
                    //printf("The target %s is skipped cause no updated children\n", (char*)target);
                    return 3;
                }
            }
            
        }

        rule_t* curr_rule = graph_get_vertex_value(g, target);
        vector* vec_cmd = curr_rule->commands;
        for (size_t i = 0; i < vector_size(vec_cmd); i++) {
            if (0 != system(vector_get(vec_cmd, i))) {
                //some commands itself fail
                rule_t* rule = graph_get_vertex_value(g, target);
                rule->state = -1;
                return -1;
            }
            
        }

        //mark the rule as satistied
        rule = graph_get_vertex_value(g, target);
        rule->state = 3;
    

    return 3;
}
*/

//input nothing, return nothing
void *execute(void *ptr) {
    while (true) {
        char *target = queue_pull(q);
        //check if we reach end
        if (!target) {
            break;
        }
        rule_t *rule = graph_get_vertex_value(g, target);
        struct stat time_stamp;
        int flag_stat = 0;
        int result = stat(rule->target, &time_stamp);
        if (result == -1) flag_stat = 1;
        //if all  stat
        if (flag_stat == 0) {
            pthread_mutex_lock(&m);
            vector *dependencies = graph_neighbors(g, target);

            pthread_mutex_unlock(&m);

            VECTOR_FOR_EACH (dependencies, vertex, {
                rule_t *temp_rule = (rule_t*)graph_get_vertex_value(g, vertex);
                struct stat tempstat;
                //compare edit time
                if (stat(temp_rule->target, &tempstat) == -1 || tempstat.st_mtime > time_stamp.st_mtime) {
                    
                    flag_stat = 1;

                    break;
                }
            });
            vector_destroy(dependencies);
        }
        int done = 1;
        
        if (flag_stat) {
            VECTOR_FOR_EACH (rule->commands, vertex, {
                if (system(vertex) != 0) {

                    done = 0;
                    break;
                }
            });
        }
        //m lock
        pthread_mutex_lock(&m);
        vector *par = graph_antineighbors(g, target);
        VECTOR_FOR_EACH(par, vertex, {
            if (done) {
                rule_t *r = graph_get_vertex_value(g, vertex);
                r->state -= 1;
                if (r->state == 0) queue_push(q, vertex);
            }
            if (!strcmp(vertex, "")) {
                thread_count++;
                pthread_cond_signal(&cv);
            }
        });
        pthread_mutex_unlock(&m);
        //m unlock
        vector_destroy(par);
    }
    return NULL;
}


//take a goal vertex, recursively add its children
void add_task(char* target) {
    //already there
    if (*(int*)dictionary_get(d, target) == 1) {
        return;
    }
    //add to dict
    dictionary_set(d, target, &one);
    vector* dependencies = graph_neighbors(g, target);
    VECTOR_FOR_EACH(dependencies, var, {
        add_task(var);
    });
    if (true == vector_empty(dependencies)) {
        queue_push(q, target);
    }
    rule_t *curr_rule = graph_get_vertex_value(g, target);
    size_t direct_child_size = vector_size(dependencies);
    curr_rule->state = direct_child_size;
    vector_destroy(dependencies);
}

//after whole graph traversal, reset d before reuse
void reset_dict() {
    vector* all_vertex = graph_vertices(g);
    for (size_t i = 0; i < vector_size(all_vertex); i++) {
        void* curr_vertex = vector_get(all_vertex, i);
        //reset to 0
        dictionary_set(d, curr_vertex, &zero);
    }
    vector_destroy(all_vertex);
}

/*
int parmake(char *makefile, size_t num_threads, char **targets) {
    // good luck!
    //fxxk me GDLK!
    //for part1
    assert(num_threads == 1);
    visited = NULL;
    //1. find all goal rules (consides all: is used)
    g = parser_parse_makefile(makefile, targets);




    vector *goals = graph_neighbors(g, "");
    size_t goals_total = vector_size(goals);
    //printf("The total goals number are %zu\n", goals_total);

    //size_t vertex_total = vector_size(graph_vertices(g));
    for (size_t i = 0; i < goals_total; i++) {
        void* goal_failed = vector_get(goals, i);
        if (inCyclic(goal_failed)) {
            rule_t * rule = graph_get_vertex_value(g, goal_failed);
            rule->state = -1;
            print_cycle_failure(goal_failed);
            //printf("The cycle of node is %s", (char*)goal_failed);
        } else {
            execute_rule(goal_failed);
        }
    }
    vector_destroy(goals);
    graph_destroy(g);
    set_destroy(visited);
    //execute_rule(vector_get(goals, 0));
    return 0;
}
*/

int parmake(char *makefile, size_t num_threads, char **targets) {
    // good luck!
    //assert(num_threads == 1);
    visited = NULL;
    pthread_t pids[num_threads];
    //1. find all goal rules (consides all: is used)
    g = parser_parse_makefile(makefile, targets);

    q = queue_create(-1);

    d = string_to_int_dictionary_create();

    reset_dict();
    
    vector *goals = graph_neighbors(g, "");
    
    for (size_t i = 0; i < vector_size(goals); i++) {
        char *goal = vector_get(goals, i);
        if (inCyclic(goal)) {
            print_cycle_failure(goal);
            rule_t *rule = graph_get_vertex_value(g, goal);
            rule->state = -1;
            vector_erase(goals, i);
        }
    }

    rule_t *root = graph_get_vertex_value(g, "");
    root->state = vector_size(goals);
    
    if (vector_empty(goals)) {
        exit(1);
    }

    reset_dict();
    VECTOR_FOR_EACH(goals, target, {
        add_task(target);
    });
    reset_dict();

    //launch thread
    for (size_t i = 0; i < num_threads; i++) {
        pthread_create(&pids[i], NULL, execute, NULL);
    }
    
    pthread_mutex_lock(&m);
    while (thread_count != vector_size(goals)) {
        pthread_cond_wait(&cv, &m);
    }
    pthread_mutex_unlock(&m);
    
    //add NULL to stop threads
    for (size_t i = 0; i < num_threads + 1; i++) {
        queue_push(q, NULL);
    }
    //join all threads
    for (size_t i = 0; i < num_threads; i++) {
        pthread_join(pids[i], NULL);
    }
    //release resources
    vector_destroy(goals);
    graph_destroy(g);
    set_destroy(visited);
    queue_destroy(q);
    dictionary_destroy(d);
    pthread_mutex_destroy(&m);
    pthread_cond_destroy(&cv);
    return 0;
}