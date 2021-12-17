#ifndef _POINT_QUEUE_H_
#define _POINT_QUEUE_H_

#include <stdint.h>

#define QUEUE_CAPACITY  16

typedef struct {
    int r;
    int c;
    uint32_t dist;
} point_t;

typedef struct point_queue_t{
    /**
     * @brief The array of point_t objects
     * 
     */
    point_t* points;
    
    /**
     * @brief How many point_t points can hold
     * 
     */
    uint32_t capacity;

    /**
     * @brief The number of point_t objects in points
     * 
     */
    uint32_t count;

    /**
     * @brief Pop the next point_t in the priority queue
     * 
     */
    point_t (* next)(struct point_queue_t *);

    /**
     * @brief Insert a point_t (will update an existing point if it exists)
     * 
     */
    void    (* insert)(struct point_queue_t *, point_t);
} point_queue_t;

void init_point_queue(point_queue_t* q);



#endif //_POINT_QUEUE_H_