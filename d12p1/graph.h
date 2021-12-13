#ifndef _graph_h
#define _graph_h

#include <stdint.h>

#define INITIAL_CAPACITY    16

#define MAX_ID_LEN          6
#define MAX_CHILDREN        10

#define NODE_STACK_SIZE     32  // This corresponds to the maximum number of steps we anticipate any path being

typedef struct node_t node_t;

struct node_t {
    char id[MAX_ID_LEN];        // id of node
    uint8_t is_big;             // whether or not node is 'big'
    node_t* adj[MAX_CHILDREN];  // adjacent nodes
    int adj_cnt;                // Number of adjacent nodes
};

typedef struct {
    int capacity;
    int count;
    node_t* nodes;
    node_t* start_node;
    node_t* end_node;
} graph_t;

/**
 * @brief Initialize the graph
 * 
 */
void init_graph();

/**
 * @brief Initialize the stack
 * 
 */
void init_stack();

/**
 * @brief Free up all dynamic memory to make valgrind happy
 * 
 */
void clean_graph();

/**
 * @brief Add an adjacency to the graph and create nodes as necessary
 * 
 * @param src id of the source node
 * @param dest id of the destination node
 */
void add_adjacency_to_graph(const char * src, const char * dest);

/**
 * @brief Add an adjacent node to an existing node
 * 
 * @param node the existing node
 * @param adj the node to add as adjacent
 */
void add_adjacent_node(node_t * node, node_t * adj);

/**
 * @brief Add a node to the graph
 * 
 * @param id id of the node
 * @return node_t* pointer to the created node
 */
node_t* add_node(const char * id);

/**
 * @brief Get a node (if it exists) from the graph by its id
 * 
 * @param id id of the node
 * @return node_t* pointer to matching node (or NULL if it doesn't exist)
 */
node_t* get_node_from_graph_by_id(const char * id);

/**
 * @brief Perform a breadth-first search on the graph
 * 
 * @return int number of paths between 'start' and 'end' nodes
 */
int bfs(void);

#endif //_graph_h