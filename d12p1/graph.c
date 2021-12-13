#include "graph.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct {
    int top;
    node_t* values[NODE_STACK_SIZE];
    void (* push)(node_t *);
    node_t* (* pop)();
    bool (* contains)(node_t *);

} node_stack_t;

graph_t graph;
node_stack_t stack;

static void stack_push(node_t* n) {
    stack.values[stack.top++] = n;
}

static node_t* stack_pop(void) {
    node_t* n = stack.values[--stack.top];
    stack.values[stack.top] = NULL;
    return n;
}

static bool stack_contains(node_t* n) {
    for(int i = 0; i < stack.top; i++) {
        if(stack.values[i] == n)    return true;
    }

    return false;
}

void init_graph() {
    graph.count = 0;
    graph.capacity = INITIAL_CAPACITY;
    graph.nodes = calloc(INITIAL_CAPACITY, sizeof(node_t));    
    graph.start_node = NULL;
    graph.end_node = NULL;
}

void init_stack() {
    stack.top = 0;
    memset(stack.values, 0, NODE_STACK_SIZE * sizeof(node_t*));
    stack.push = stack_push;
    stack.pop = stack_pop;
    stack.contains = stack_contains;
}

void clean_graph() {
    free(graph.nodes);
}

void add_adjacency_to_graph(const char* src, const char* dest) {

    node_t *src_node = get_node_from_graph_by_id(src);
    node_t *dest_node = get_node_from_graph_by_id(dest);

    if(src_node == NULL) {
        src_node = add_node(src);
    }

    if(dest_node == NULL) {
        dest_node = add_node(dest);
    }

    add_adjacent_node(src_node, dest_node);
    add_adjacent_node(dest_node, src_node);
}

node_t * add_node(const char* id) {

    node_t* ret = NULL;
    
    strncpy(graph.nodes[graph.count].id, id, MAX_ID_LEN);

    // Figure out if it's big or not
    uint8_t big = 1;
    for(int i = 0; i < strlen(id); i++) {
        if(id[i] >= 'a' && id[i] <= 'z') {
            big = 0;
        }
    }    

    graph.nodes[graph.count].is_big = big;
    graph.nodes[graph.count].adj_cnt = 0;

    ret = &graph.nodes[graph.count];

    graph.count++;

    if(graph.count == graph.capacity) {
        graph.capacity *= 2;
        graph.nodes = reallocarray(graph.nodes, graph.capacity, sizeof(node_t));
    }

    return ret;
}

node_t * get_node_from_graph_by_id(const char* id) {

    node_t * n = NULL;

    int i = 0;
    
    while(i < graph.count && n == NULL) {
        if(strncmp(graph.nodes[i].id, id, MAX_ID_LEN) == 0) {
          n = &graph.nodes[i];
        }
        i++;
    }

    return n;
}

void add_adjacent_node(node_t * node, node_t * adj) {

    uint8_t found = 0;
    for(int i = 0; i < node->adj_cnt && !found; i++) {
        if(node->adj[i] == adj) {
            found = 1;
        }
    }

    if(!found) {
        node->adj[node->adj_cnt++] = adj;
    }
}

int depth=0;

static void print_depth() {
    for(int d = 0; d < depth; d++) {
        printf(" ");
    }
}


int search(node_t* n) {
    int paths = 0;
    
    stack.push(n);

    print_depth(); 
    printf("%s\n", n->id);

    for(int i = 0; i < n->adj_cnt; i++) {
        if(n->adj[i] == graph.end_node) {
            paths++;
            depth++;
            print_depth();
            printf("end\n");
            depth--;
        } else {
            if(!stack.contains(n->adj[i]) || n->adj[i]->is_big) {
                depth++;
                paths += search(n->adj[i]);
            }
        }
        
    }

    stack.pop();

    depth--;
    return paths;
}

int bfs(void) {
    init_stack();

    graph.start_node = get_node_from_graph_by_id("start");
    graph.end_node = get_node_from_graph_by_id("end");

    int paths = search(graph.start_node);

    return paths;
}