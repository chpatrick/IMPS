#include "hashtable.h"
#define NUMBUCKETS (1 << 6)

hashtable newTable() {
  hashtable table = (node **) malloc(sizeof(node *) * NUMBUCKETS);
  int i;
  for (i = 0; i < NUMBUCKETS; i++) {
    table[i] = NULL;
  }
  return table;
}

unsigned long strhash(char *str) {
  unsigned long hash = 0;
  int i;
  for (i = 0; str[i]; i++) {
    hash = str[i] + (hash << 6) + (hash << 16) - hash;
  }
  return (hash & (NUMBUCKETS-1));
}

void add(hashtable table, char *name, void *value) {
  if (table == NULL) { perror("NULL hashtable specified"); exit(1);}
  unsigned long hash = strhash(name);
  node *head = table[hash];
  node *new = malloc(sizeof(node));
  new->value = value;
  new->key = name;
  new->next = head;
  table[hash] = new;
}

void *search(hashtable table, char* key) {
  if (table == NULL) { perror("NULL hashtable specified"); exit(1);}
  node* curr = table[strhash(key)];
  while (curr != NULL) {
    if (!strcmp(curr->key,key)) {
      return curr->value;
    }
    curr = curr->next;
  }
  return NULL;
}
