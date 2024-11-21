#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "LoadBalancer-1.h"

struct balancer {
    int batch_size;           // size of each batch
    struct job_node* head;    // head of job list
    int current_batch_count;  // current number of jobs in batch
    pthread_mutex_t mutex;    // mutex for protecting job list
    host* instance_host;      // pointer to instance host
};

// Helper function to create a new job node
struct job_node* create_job_node(int user_id, int data, int* data_return) {
    struct job_node* node = (struct job_node*)malloc(sizeof(struct job_node));
    node->user_id = user_id;
    node->data = data;
    node->data_result = data_return;
    node->next = NULL;
    return node;
}

balancer* balancer_create(int batch_size) {
    balancer* lb = (balancer*)malloc(sizeof(balancer));
    lb->batch_size = batch_size;
    lb->head = NULL;
    lb->current_batch_count = 0;
    pthread_mutex_init(&lb->mutex, NULL);
    lb->instance_host = host_create();
    return lb;
}

void balancer_destroy(balancer** lb) {
    if (!lb || !*lb) return;
    
    // Process any remaining jobs
    if ((*lb)->head != NULL) {
        host_request_instance((*lb)->instance_host, (*lb)->head);
    }
    
    pthread_mutex_destroy(&(*lb)->mutex);
    host_destroy(&(*lb)->instance_host);
    free(*lb);
    *lb = NULL;
}

void balancer_add_job(balancer* lb, int user_id, int data, int* data_return) {
    struct job_node* new_node = create_job_node(user_id, data, data_return);
    
    pthread_mutex_lock(&lb->mutex);
    
    // Add job to list
    if (lb->head == NULL) {
        lb->head = new_node;
    } else {
        struct job_node* current = lb->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
    
    lb->current_batch_count++;
    
    // If batch is full, send to instance host
    if (lb->current_batch_count >= lb->batch_size) {
        host_request_instance(lb->instance_host, lb->head);
        lb->head = NULL;
        lb->current_batch_count = 0;
    }
    
    pthread_mutex_unlock(&lb->mutex);
}