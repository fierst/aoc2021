#include "pointqueue.h"

#include <stdlib.h>


static void pq_insert(point_queue_t * q, point_t p);
static point_t  pq_next(point_queue_t * q);
static point_t* pq_find(point_queue_t * q, point_t p);

static int point_queue_sort_function(const void * a, const void * b);

void init_point_queue(point_queue_t* q) {
    q->capacity = QUEUE_CAPACITY;
    q->count = 0;
    q->points = calloc(QUEUE_CAPACITY, sizeof(point_t));
    q->next = pq_next;
    q->insert = pq_insert;
}

static point_t* pq_find(point_queue_t * q, point_t p) {
    point_t * ret = NULL;

    for(int i = 0; i < q->count && ret == NULL; q++) {
        if(q->points[i].r == p.r && q->points[i].c == p.c)
            ret = &q->points[i];
    }

    return ret;
}

static void pq_insert(point_queue_t * q, point_t p) {
    
    point_t* e = pq_find(q, p); // existing point in queue

    // If e is null, the point doesn't exist so add it and sort
    if(e == NULL) {
        q->points[q->count++] = p;
        if(q->count == q->capacity) {
            q->capacity *= 2;
            q->points = reallocarray(q->points, q->capacity, sizeof(point_t));
        }
    } else {
        *e = p;
    }
    qsort(q->points, q->count, sizeof(point_t), point_queue_sort_function);
}

static int point_queue_sort_function(const void * a, const void * b) {
    uint32_t dista = (*(point_t *)a).dist;
    uint32_t distb = (*(point_t *)b).dist;

    if(dista > distb) {
        return -1;
    } else if(dista < distb) {
        return 1;
    } else {
        return 0;
    }
}

static point_t pq_next(point_queue_t * q) {
    return q->points[--(q->count)];
}