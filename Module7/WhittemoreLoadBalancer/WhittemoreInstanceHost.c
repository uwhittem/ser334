#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "InstanceHost-1.h"

struct host {
    pthread_mutex_t mutex;
    int active_instances;
};

// Structure to pass to processing thread
struct process_args {
    struct job_node* batch;
};

// Helper function to process a single job
void process_job(struct job_node* job) {
    if (job && job->data_result) {
        *(job->data_result) = job->data * job->data;  // Square the number
    }
}

// Thread function to process batch
void* process_batch(void* args) {
    struct process_args* pargs = (struct process_args*)args;
    struct job_node* current = pargs->batch;
    
    // Process each job in the batch
    while (current != NULL) {
        process_job(current);
        struct job_node* temp = current;
        current = current->next;
        free(temp);  // Free the job node after processing
    }
    
    free(pargs);
    return NULL;
}

host* host_create() {
    host* h = (host*)malloc(sizeof(host));
    pthread_mutex_init(&h->mutex, NULL);
    h->active_instances = 0;
    return h;
}

void host_destroy(host** h) {
    if (!h || !*h) return;
    
    pthread_mutex_destroy(&(*h)->mutex);
    free(*h);
    *h = NULL;
}

void host_request_instance(host* h, struct job_node* batch) {
    pthread_t thread;
    struct process_args* args = malloc(sizeof(struct process_args));
    args->batch = batch;
    
    pthread_mutex_lock(&h->mutex);
    h->active_instances++;
    pthread_mutex_unlock(&h->mutex);
    
    pthread_create(&thread, NULL, process_batch, args);
    pthread_detach(thread);  // Detach thread to allow it to clean up automatically
}