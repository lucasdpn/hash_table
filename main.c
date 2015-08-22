/* 
 * File:   main.c
 * Author: Lucas.Dalpiaz
 *
 * Created on July 10, 2015, 2:49 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "hash.h"

// in case of dynamic memory allocation.
int hash_node_delete(Node *node) {
    free (node->key);
    free (node->data);
    return HASH_SUCCESS;
}

int main(int argc, char** argv) {

    HashTable hash_table;
    Node *node;
    
    // create table.
    ht_create(&hash_table, 23, NULL, NULL, NULL);
    
    // insert nodes.
    ht_insert( &hash_table, "hg456h", "hh");
    ht_insert( &hash_table, "i46fh5LL", "ii");
    ht_insert( &hash_table, "7dfgsd89", "ee77777");
    ht_insert( &hash_table, "f7ert89f", "ff");
    ht_insert( &hash_table, "g2fghsd52g", "gg");
    ht_insert( &hash_table, "hg43457", "hh");
    ht_insert( &hash_table, "i46dfgkffh5i", "ii");
    ht_insert( &hash_table, "78çlikj9", "ee");
    ht_insert( &hash_table, "f78dcfb9f", "ff");
    ht_insert( &hash_table, "g254342", "gg");
    ht_insert( &hash_table, "hg445656h", "hh");
    ht_insert( &hash_table, "i4645645fh5i99", "ii99"); 
    
    // retrieve information
    node = ht_get_node(&hash_table, "i4645645fh5i99");
    if (node != NULL)
        printf("data %s\n", (char *)node->data);
    
    node = ht_get_node(&hash_table, "789");
    if (node != NULL)
        printf("data %s\n", (char *)node->data);
    
    node = ht_get_node(&hash_table, "7dfgsd89");
    if (node != NULL)
        printf("data %s\n\n", (char *)node->data);
    
    // delete nodes
    ht_delete_entry(&hash_table, "hg456h");
    ht_delete_entry(&hash_table, "i4645645fh5i99");
    
    // print data.
    printf("*-*-*-*-*- Print all *-*-*-*-*-\n\n");
    ht_print_all( &hash_table );
    
    ht_delete( &hash_table );
    return (EXIT_SUCCESS);
}

