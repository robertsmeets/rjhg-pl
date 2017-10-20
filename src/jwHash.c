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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jwHash.h"

////////////////////////////////////////////////////////////////////////////////
// STATIC HELPER FUNCTIONS

// Spin-locking
// http://stackoverflow.com/questions/1383363/is-my-spin-lock-implementation-correct-and-optimal

// http://stackoverflow.com/a/12996028
// hash function for int keys
static inline long int hashInt(long int x)
{
   x = ((x >> 16) ^ x) * 0x45d9f3b;
   x = ((x >> 16) ^ x) * 0x45d9f3b;
   x = ((x >> 16) ^ x);
   return x;
}

// http://www.cse.yorku.ca/~oz/hash.html
// hash function for string keys djb2
static inline long int hashString(char * str)
{
   unsigned long hash = 5381;
   int c;

   while (c = *str++)
      hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
   return hash;
}

// helper for copying string keys and values
static inline char * copystring(char * value)
{
   char * copy = (char *)GC_MALLOC(strlen(value)+1);
   if(!copy) {
      printf("Unable to allocate string value %s\n",value);
      abort();
   }
   strcpy(copy,value);
   return copy;
}


////////////////////////////////////////////////////////////////////////////////
// CREATING A NEW HASH TABLE

// Create hash table
jwHashTable *create_hash( size_t buckets )
{
   // allocate space
   jwHashTable *table= (jwHashTable *)GC_MALLOC(sizeof(jwHashTable));
   if(!table) {
      // unable to allocate
      return NULL;
   }
   // locks
   // setup
   table->bucket = (jwHashEntry **)GC_MALLOC(buckets*sizeof(void*));
   if( !table->bucket ) {
      // free(table);
      return NULL;
   }
   memset(table->bucket,0,buckets*sizeof(void*));
   table->buckets = table->bucketsinitial = buckets;
   return table;
}

////////////////////////////////////////////////////////////////////////////////
// ADDING / DELETING / GETTING BY STRING KEY

HASHRESULT add_ptr_by_int( jwHashTable *table, long int key, void *value )
{
   //
   // compute hash on key
   //
   size_t hash = hashInt(key) % table->buckets;
   //
   // add entry
   //
   jwHashEntry *entry = table->bucket[hash];
   // 
   // already an entry
   //
   while(entry!=0)
   {
      //HASH_DEBUG("checking entry: %x\n",entry);
      //
      // check for already indexed
      //
      if(entry->key.intValue==key && (value==entry->value.ptrValue))
      {
         return HASHALREADYADDED;
      }
      //
      // check for replacing entry
      //
      if(entry->key.intValue==key && (value==entry->value.ptrValue))
      {
         entry->value.ptrValue = value;
         return HASHREPLACEDVALUE;
      }
      // move to next entry
      entry = entry->next;
   }
   
   // create a new entry and add at head of bucket
   entry = (jwHashEntry *)GC_MALLOC(sizeof(jwHashEntry));
   entry->key.intValue = key;
   entry->valtag = HASHPTR;
   entry->value.ptrValue = value;
   entry->next = table->bucket[hash];
   table->bucket[hash] = entry;
   return HASHOK;
}


// Lookup ptr - keyed by int
HASHRESULT get_ptr_by_int( jwHashTable *table, long int key, void **value )
{
   // compute hash on key
   size_t hash = hashInt(key) % table->buckets;
   // get entry
   jwHashEntry *entry = table->bucket[hash];
   // already an entry
   while(entry)
   {
      // check for key
      if(entry->key.intValue==key) {
         *value = entry->value.ptrValue;
         return HASHOK;
      }
      // move to next entry
      entry = entry->next;
   }
*  value = NULL; 
   // not found
   return HASHNOTFOUND;
}


