/*

Copyright 2015 Jonathan Watmough
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
    http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*/

#ifndef jwhash_h
#define jwhash_h

// needed for size_t
#include <stddef.h>
#include "gc.h"

// resuts codes
typedef enum 
{
	HASHOK,
	HASHADDED,
	HASHREPLACEDVALUE,
	HASHALREADYADDED,
	HASHDELETED,
	HASHNOTFOUND,
} HASHRESULT;

typedef enum
{
	HASHPTR
} HASHVALTAG;
	

typedef struct jwHashEntry jwHashEntry;
struct jwHashEntry
{
	union
	{
		char  *strValue;
		double dblValue;
		int	   intValue;
	} key;
	HASHVALTAG valtag;
	union
	{
		char  *strValue;
		double dblValue;
		int	   intValue;
		void  *ptrValue;
	} value;
	jwHashEntry *next;
};

typedef struct jwHashTable jwHashTable;
struct jwHashTable
{
	jwHashEntry **bucket;			// pointer to array of buckets
	size_t buckets;
	size_t bucketsinitial;			// if we resize, may need to hash multiple times
	HASHRESULT lastError;
};

// Create/delete hash table
jwHashTable *create_hash( size_t buckets );
void *delete_hash( jwHashTable *table );		// clean up all memory


HASHRESULT add_ptr_by_int( jwHashTable*, long int key, void *value );
HASHRESULT get_ptr_by_int( jwHashTable *table, long int key, void **value );

#endif



