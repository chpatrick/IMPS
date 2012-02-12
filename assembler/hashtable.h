#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma once

typedef struct node {
  char *key;
  void *value;
  struct node *next;
} node;

typedef node** hashtable;

void add(hashtable table, char *name, void *value);
void *search(hashtable table, char* name);
hashtable newTable();
void freetable(hashtable table);
