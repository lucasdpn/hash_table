/*! \file hash.h
    \brief An hash implementation.
 * 
 * An instance of ::HashTable can overwrite three methods:
 * - ::Hash_address
 * - ::Hash_key_compare
 * - ::Hash_node_delete
 * 
 * They are passed to the method ht_create that must be called when creating a
 * ::HashTable. A default behavior is generated for this functions if a NULL 
 * pointer is passed as argument. In this case, the field key must be an String.
*/

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/** @name hash return codes
 * \anchor HASH_RETURN_CODES
 *  Returned codes used for the hash functions.
 */
///@{
/** error,  */
#define HASH_ERROR -1
#define HASH_INVALID_SIZE -2
#define HASH_CANT_ALOC_MEM -3
#define HASH_KEY_ALREADY_ENTERED -4
#define HASH_ENTRY_NOT_FOUND -5

/** success */
#define HASH_SUCCESS 0
#define HASH_ENTRY_DELETED 1

///@}

/*! Enable debug functions */
#define HASH_DEBUG

/** @struct s_Node
 *  @brief This structure stores the references for the data.
 *  @var s_Node::key
 *  Member 'key' contains the pointer to the key value.
 *  @var s_Node::data
 *  Member 'data' contains the pointer to the data value.
 */
struct s_Node {
    void *key;
    void *data;
};

/*! \var typedef Node
    \brief A type definition for a struct s_Node.
*/
typedef struct s_Node Node;

/** @struct s_HashmapNode
 *  @brief This structure has a *next. If different keys generates the same hash address,
 *  a linked list is created.
 *  @var s_HashmapNode::node
 *  Member 'node' an node reference.
 *  @var s_HashmapNode::next
 *  Member 'next' pointer to the next node.
 */
struct s_HashmapNode {
    Node *node;
    struct s_HashmapNode  *next;
};

/*! \var typedef HashmapNode
    \brief A type definition for a struct s_HashmapNode.
*/
typedef struct s_HashmapNode HashmapNode;

/*! \var typedef HashTable
    \brief A type definition for a struct s_HashTable.
*/
typedef struct s_HashTable HashTable;

/*! \var typedef unsigned int (*Hash_address)(const HashTable *hash_table, void *key)
    \brief This is the hashing function and it's responsible for generating a key.
*/
typedef unsigned int (*Hash_address)(const HashTable *hash_table, void *key);

/*! \var typedef typedef int (*Hash_key_compare)(void *a, void *b)
    \brief Function that the HashTable uses to actually find elements by their key.
 * if the elements are equal, the function must return 0.
*/
typedef int (*Hash_key_compare)(void *a, void *b);

/*! \var typedef int (*Hash_node_delete)(HashmapNode *node)
    \brief Function executed when a node is deleted. Usually, data and key fields are
    allocated dynamically and must be freed before being deleted from the hash table.
 * NOTE: The default function just return HASH_SUCCESS. So, it doesn't deallocated 
 * memory.
*/
typedef int (*Hash_node_delete)(Node *node);

/** @struct s_HashTable
 *  @brief This structure represents the hash table itself. The easiest way to 
 *  use it is using its functions, instead of directly manipulating data.
 *  @var s_HashTable::table
 *  Member 'table' reference to the table.
 *  @var s_HashTable::hash_address
 *  Member 'hash_address' ::Hash_address
 *  @var s_HashTable::hash_compare
 *  Member 'hash_compare' ::Hash_key_compare
 *  @var s_HashTable::hash_node_delete 
 *  Member 'hash_node_delete' ::Hash_node_delete
 *  @var s_HashTable::size 
 *  Member 'size' sets the number of buckets. A prime number usually generates a 
 *  better distribution. 
 */
struct s_HashTable {
    HashmapNode **table;
    Hash_address hash_address;
    Hash_key_compare hash_compare;
    Hash_node_delete hash_node_delete;
    unsigned int size;
};

/*! \fn int ht_create(HashTable* hash_table, unsigned int size_table, 
 * Hash_address hash_address, Hash_key_compare hash_compare, 
 * Hash_node_delete hash_node_delete)
    \brief Create a new hash table.
    \param hash_table The descriptor to a hash table.
    \param size_table The hash table size.
    \param hash_address Pointer to a function that it's responsible for taking a 
 * key, processing its contents, and producing an index number < than size_table.
 * ::Hash_address
    \param hash_compare Pointer to a function that the Hashmap uses to actually
 *  find elements by their key. ::Hash_key_compare
    \param hash_node_delete Pointer to a function that is executed when a node
 *  is deleted. ::Hash_node_delete
 * \return 
 * \ref HASH_RETURN_CODES
*/
int ht_create(HashTable* hash_table, unsigned int size_table, Hash_address hash_address, Hash_key_compare hash_compare, Hash_node_delete hash_node_delete); // created

/*! \fn Node *ht_get_node(const HashTable *hash_table, void *key)
    \brief Returns a node if the given key was inserted in the hash table. Otherwise a null pointer is returned.
    \param hash_table The descriptor to a hash table.
    \param key The key associated to an data.
    \return NULL if not found. Otherwise, the node reference.
*/
Node *ht_get_node(const HashTable *hash_table, void *key);

/*! \fn int ht_insert(const HashTable *hash_table, void *key, void *value )
    \brief Insert a key-value pair into a hash table.
    \param hash_table The descriptor to a hash table.
    \param key The key associated to an data.
    \param data The data.
    \return 
 *  \ref HASH_RETURN_CODES
*/
int ht_insert(const HashTable *hash_table, void *key, void *data );

/*! \fn int ht_delete_entry(const HashTable *hash_table, void *key)
    \brief Delete an entry from a hash table.
    \param hash_table The descriptor to a hash table.
    \param key The key associated to an data.
    \return 
 *  \ref HASH_RETURN_CODES
*/
int ht_delete_entry(const HashTable *hash_table, void *key);

/*! \fn void ht_delete(const HashTable *hash_table )
    \brief Delete all elements from hash table.
    \param hash_table The descriptor to a hash table.
*/
void ht_delete(const HashTable *hash_table );

#ifdef HASH_DEBUG
/*! \fn void ht_print_all(const HashTable *hash_table )
    \brief Print all hash table and the addresses generated. Useful for visualizing the key distribution.  
    \param hash_table The descriptor to a hash table.
*/
void ht_print_all(const HashTable *hash_table );
#endif

#endif
