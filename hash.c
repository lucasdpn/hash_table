#include "hash.h"

static unsigned int default_hash_addr(const HashTable *hash_table, void *key) {
    unsigned int address = 1;
    char *key_txt = (char *) key;
    int key_length = strlen(key);
    int i = 0;

    for (; i < key_length; ++i) {
        // + 1. Otherwise, adress[0] will be super populate.
        address = ((address * key_txt[i]) % hash_table->size) + 1;
    }
    address--;

    return address;
}

static int default_compare(void *a, void *b) {
    return strcmp((char *) a, (char *) b);
}

static int default_delete_node(Node *node){
    return HASH_SUCCESS;
}

int ht_create(HashTable* hash_table, unsigned int size_table, Hash_address hash_address, Hash_key_compare hash_compare, Hash_node_delete hash_node_delete) {
    int i;

    if (size_table < 1) {
        return HASH_INVALID_SIZE;
    }

    /* Allocate pointers to the head nodes. */
    hash_table->table = (HashmapNode **)malloc(sizeof (HashmapNode *) * size_table);
    if (hash_table->table == NULL) {
        return HASH_CANT_ALOC_MEM;
    }

    for (i = 0; i < size_table; i++) {
        hash_table->table[i] = NULL;
    }
    hash_table->size = size_table;
    hash_table->hash_compare = (hash_compare == NULL) ? default_compare : hash_compare;
    hash_table->hash_address = (hash_address == NULL) ? default_hash_addr : hash_address;
    hash_table->hash_node_delete = (hash_node_delete == NULL) ? default_delete_node : hash_node_delete;
    
    return HASH_SUCCESS;
}

Node *ht_get_node(const HashTable *hash_table, void *key) {
    unsigned int address;
    HashmapNode* entry;

    address = hash_table->hash_address(hash_table, key);
    entry = hash_table->table[address];

    // find node
    for (; entry != NULL; entry = entry->next) {
        if (hash_table->hash_compare(entry->node->key, key) == 0)
            return entry->node;
    }

    // Not found. Entry equals NULL.
    return NULL;
}

static HashmapNode * ht_new_entry(void *key, void *data) {
    HashmapNode *new_hash_node;
    Node *node;
    
    if (key == NULL || data == NULL) {
        return NULL;
    }

    node = (Node *)malloc(sizeof(Node));
    if (node == NULL) {
        return NULL;
    }
    node->key = key;
    node->data = data;
    
    new_hash_node = (HashmapNode *)malloc(sizeof(HashmapNode));
    if (new_hash_node == NULL) {
        return NULL;
    }

    new_hash_node->node = node;
    new_hash_node->next = NULL;

    return new_hash_node;
}

int ht_insert(const HashTable *hash_table, void *key, void *data) {
    unsigned int address;
    HashmapNode* first_node;
    HashmapNode* previous_node;
    HashmapNode* cur_node;
    HashmapNode* new_node;
    
    address = hash_table->hash_address(hash_table, key);
    cur_node = hash_table->table[address];
    first_node = cur_node;
    
    // put key in the last entry if it doesn't exist
    while ( cur_node != NULL ) {
        // key must be unique.
        if (hash_table->hash_compare(cur_node->node->key, key) == 0) {
            return HASH_KEY_ALREADY_ENTERED;
        }
        previous_node = cur_node;
        cur_node = cur_node->next;
    }
    
    // generate new entry
    new_node = ht_new_entry( key, data );
    if (new_node == NULL) {
        return HASH_CANT_ALOC_MEM;
    }
    
    // first entry.
    if (cur_node == first_node) {
        hash_table->table[address] = new_node;
    }
    else {
        previous_node->next = new_node;
    }

    return HASH_SUCCESS;
}

void ht_delete(const HashTable *hash_table ) {
    int i;
    HashmapNode *cur_node;
    HashmapNode *previous_node;
    
    for (i = 0; i < hash_table->size; i++) {
        cur_node = hash_table->table[i];
        while ( cur_node != NULL ) {
            previous_node = cur_node;
            cur_node = cur_node->next;
            hash_table->hash_node_delete( previous_node->node );
            free( previous_node->node );
            free( previous_node );
        }
    }
    free(hash_table->table);
}

int ht_delete_entry(const HashTable *hash_table, void *key ) {
    unsigned int address;
    HashmapNode* first_node;
    HashmapNode* previous_node = NULL;
    HashmapNode* cur_node;

    address = hash_table->hash_address(hash_table, key);
    first_node = hash_table->table[address];
    
    // find 
    for (cur_node = first_node; cur_node != NULL; cur_node = cur_node->next) {
        if (hash_table->hash_compare(cur_node->node->key, key) == 0) {
            // start of the linked list
            if ( cur_node == first_node) {
                // has next
                if ( cur_node->next != NULL ) {
                    hash_table->table[address] = cur_node->next;
                }
                else {
                    hash_table->table[address] = NULL;
                }
            }
            // middle of the linked list
            else if ( cur_node->next != NULL ) {
                previous_node->next = cur_node->next;
            }
            // end of the linked list
            else {
                previous_node->next = NULL;
            }   
            hash_table->hash_node_delete( cur_node->node );
            free( cur_node->node );
            free( cur_node );
            return HASH_ENTRY_DELETED;
        }
        previous_node = cur_node;
    }
    return HASH_ENTRY_NOT_FOUND;
}

#ifdef HASH_DEBUG
void ht_print_all(const HashTable *hash_table ) {
    int i;
    HashmapNode *cur_node;
    
    for (i = 0; i < hash_table->size; i++) {
        cur_node = hash_table->table[i];
        while ( cur_node != NULL ) {
            printf("Addr [%d] key = %s data = %s \n", i, (char *)cur_node->node->key, (char *)cur_node->node->data);
            cur_node = cur_node->next;
        }
    }
}
#endif
