/**
 * mini_memcheck
 * CS 241 - Fall 2021
 */
#include "mini_memcheck.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

//extern variable defined here
//meta_data *head;
meta_data* head;
size_t total_memory_requested;
size_t total_memory_freed;
size_t invalid_addresses;
static const size_t meta_size = sizeof(meta_data);

//check if ptr is a valid ptr2
meta_data* is_valid_ptr(meta_data* head, meta_data* ptr);
meta_data* get_previous_node(meta_data* head, meta_data* ptr);

void *mini_malloc(size_t request_size, const char *filename,
                  void *instruction) {
    // your code here
    size_t real_size = request_size + meta_size;
    //explicitly using char*, so no casting 
    void* ptr1 = malloc(real_size);
    if (!ptr1) {
        //allocation fail
        return NULL;
    }
    void* ptr2 = ptr1 + meta_size;

    total_memory_requested += request_size;
    ((meta_data*)ptr1)->request_size = request_size;
    ((meta_data*)ptr1)->filename = filename;
    ((meta_data*)ptr1)->instruction = instruction;

    //insert to beginning of LinkedList
    meta_data* temp = head;

    
    ((meta_data*)ptr1)->next = temp;
    head = (meta_data*)ptr1;

    return ptr2;
}

void *mini_calloc(size_t num_elements, size_t element_size,
                  const char *filename, void *instruction) {
    // your code here
    size_t request_size = num_elements * element_size;
    void* res = mini_malloc(request_size, filename, instruction);
    //return NULL;
    memset(res, '\0', request_size);
    return res;
}

void *mini_realloc(void *payload, size_t request_size, const char *filename,
                   void *instruction) {
    if (!payload) {
        return mini_malloc(request_size, filename, instruction);
    }
    if (request_size == 0) {
        mini_free(payload);
        return NULL;
    }          
    if (NULL == is_valid_ptr(head, (meta_data*)payload)) {
        invalid_addresses++;
        return NULL;
    }

    meta_data* old_target = (meta_data*) (payload - meta_size);
    size_t old_size = old_target->request_size;
    
    if (request_size > old_size) {
        total_memory_requested += (request_size - old_size);
    }
    if (request_size < old_size) {
        total_memory_freed += (old_size - request_size);
    }


    //do real realloc here
    meta_data* prev = get_previous_node(head, payload);
    if (prev == NULL) {
        //at head
        meta_data* new_target = realloc(old_target, request_size + meta_size);
        new_target->request_size = request_size;
        fprintf(stderr, "New size is %zu", request_size);
        head = new_target;
        return (void*)(head + 1);
    } else {
        meta_data* new_target = realloc(old_target, request_size + meta_size);
        new_target->request_size = request_size;
        prev->next = new_target;
        return (void*)(new_target + 1);
    }
    
    //return NULL;
}

void mini_free(void *payload) {
    if (!payload) {
        return;
    }
    //check invalid free and double free
    meta_data* target = is_valid_ptr(head, (meta_data*)payload);
    if (NULL == target) {
        invalid_addresses++;
        return;
    } else {
        //get a match
        //1. manipulate linked list
        if (target == head) {
            head = target->next;
        } else {
            meta_data* prev = get_previous_node(head, payload);
            prev->next = target->next;
        }
        //2. free ptr1
        total_memory_freed += target->request_size;
        free((void*) target);
    }

    return;
}

//ptr is ptr2 type
//return ptr1 type
meta_data* is_valid_ptr(meta_data* head, meta_data* ptr) {
    if (!head) {
        //no memory allocated yet
        return NULL;
    }
    while (head != NULL) {
        if (head + 1 == ptr) {
            return head;
        }
        head = head->next;
    }
    return NULL;
}

//ptr is ptr2 type
//return ptr1 type
meta_data* get_previous_node(meta_data* head, meta_data* ptr) {
    if (head == ptr) {
        return NULL;
    }
    if (!head) {
        return NULL;
    }
    while (head->next != NULL) {
        if (head->next+1 == ptr) {
            return head;
        }
        head = head->next;
    }
    return NULL;
}

