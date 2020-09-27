#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"

typedef struct Pair Pair;
typedef struct HashMap HashMap;
int enlarge_called=0;

struct Pair {
     char * key;
     void * value;
};

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
  long h= hash(key,map->capacity);
  Pair* x = createPair(key,value);

  for (long i=h;i<map->capacity;i++){
    
    if ((map->buckets[i] == NULL) && (x->key!=NULL)){
        map->buckets[i]=x;
        break;
    }
    else if (is_equal(map->buckets[i]->key,key)){
      break;
    }
    else if (i==map->capacity-1){
      i=0;
    }
  }
  map->size=map->size +1;
}

void enlarge(HashMap * map) {
    Pair ** aux = map->buckets;
    map->capacity*=2;
    Pair ** newbackets = (Pair**)malloc(sizeof(Pair*)*map->capacity);
    map->buckets=newbackets;
    for (int i=0;i<map->capacity/2;i++){
      if (aux[i]!=NULL && aux[i]->key!=NULL){
        insertMap(map, aux[i]->key, aux[i]->value);
      }
    }
    enlarge_called = 1; //no borrar (testing purposes)


}


HashMap * createMap(long capacity) {
  HashMap * map = (HashMap *)malloc(sizeof(HashMap));
  map->capacity=capacity;
  map->size =0;
  map->current=-1;
  map->buckets= (Pair**)malloc(sizeof(Pair*)*capacity);

    return map;
}

void eraseMap(HashMap * map,  char * key) {    
    int i;
    for (i=0;i<map->capacity;i++){
      if (map->buckets[i]!=NULL && is_equal(searchMap(map,key),         map->buckets[i]->value)){
        map->buckets[i]->key= NULL;
        map->buckets[i]->value = NULL;
        map->size=map->size-1;
      }
    }
}

void * searchMap(HashMap * map,  char * key) {   
  long h= hash(key,map->capacity);
  int i;
  for (i=h;i<map->capacity;i++){
    if(map->buckets[i]!=NULL){
      if (is_equal(map->buckets[i]->key,key)){
        map->current=i;
        return map->buckets[i]->value;
        break;
      }
      else{
        continue;
      }
      if (i==map->capacity-1){
        i=0;
      }
      if (is_equal(map->buckets[i]->key, NULL)){
        return NULL;
        break;
      }
    
    }
  }
  return NULL;  
  
}

void * firstMap(HashMap * map) {
    long i;
    for (i=0;i<map->capacity;i++){
      if (map->buckets[i]!= NULL && map->buckets[i]->key!=NULL){
        map->current=i;
        return map->buckets[i]->value;
         
      }
    }
    return NULL;
}

void * nextMap(HashMap * map) {
  long i;
  for (i=map->current+1;i<map->capacity;i++){
    if (map->buckets[i]!= NULL && map->buckets[i]->key!=NULL){
      map->current=i;
      return map->buckets[i]->value;
      
    }

  }
    return NULL;
}
