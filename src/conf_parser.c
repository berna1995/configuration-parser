/**
 * @file conf_parser.c
 * @author Matteo Bernabito
 * @brief Implemetations of the functions in conf_parser.h
 */

#include "conf_parser.h"
#include <string.h>
#include <stdlib.h>

/**
 * Static function that initialize the entries structure
 * @param  defaultSize the default size of arrays of values and keys
 * @return a pointer to a configuration entries structure or NULL if a memory error occured
 */
static conf_entries_t* initializeEntries(unsigned int defaultSize) {
  conf_entries_t* ent = (conf_entries_t*) malloc(sizeof(conf_entries_t));
  if(!ent)
    return NULL;
  ent->entries_number = 0;
  ent->real_entries_number = defaultSize;
  ent->keys = (char**) malloc(sizeof(char*) * defaultSize);
  ent->values = (char**) malloc(sizeof(char*) * defaultSize);
  if(!ent->keys || !ent->values) {
    configuration_free(&ent);
    return NULL;
  }

  for(int i=0; i<defaultSize; i++) {
    ent->keys[i] = NULL;
    ent->values[i] = NULL;
  }
  
  return ent;
}

/**
 * Static function to add entries and adjust the structure size everytime we add an entry
 * @param  entries a pointer to a configuration entries structure
 * @param  key     the key that we want to add
 * @param  value   the value associated with the key
 * @return 0 if everything went ok, -1 if an error occured
 */
static int addEntry(conf_entries_t* entries, char* key, char* value) {
  if(entries == NULL || key == NULL || value == NULL)
    return -1;

  if(entries->entries_number == entries->real_entries_number) {
    entries->real_entries_number += DEFAULT_REALLOC_STEP;
    entries->values = (char**) realloc(entries->values, sizeof(char*) * entries->real_entries_number);
    entries->keys = (char**) realloc(entries->keys, sizeof(char*) * entries->real_entries_number);
    if(entries->values == NULL || entries->keys == NULL)
      return -1;
  }

  int keyLen = strlen(key) + 1;
  int valueLen = strlen(value) + 1;

  entries->keys[entries->entries_number] = (char*) malloc(sizeof(char) * keyLen);
  if(entries->keys[entries->entries_number] == NULL) return -1;
  entries->values[entries->entries_number] = (char*) malloc(sizeof(char) * valueLen);
  if(entries->values[entries->entries_number] == NULL) return -1;
  strncpy(entries->keys[entries->entries_number], key, keyLen);
  strncpy(entries->values[entries->entries_number], value, valueLen);

  entries->entries_number++;

  return 0;
}

/**
 * Static function to remove from a string the characters passed as argument
 * @param  str           the string we want to remove the characters from
 * @param  charsToRemove the string containing all the characters that we want to remove
 * @return               the string without the charsToRemove characters
 */
static char* removeCharacters(char* str, char* charsToRemove) {
  if(str == NULL || charsToRemove == NULL)
    return NULL;

  int strLen = strlen(str);
  int charsToRemoveLen = strlen(charsToRemove);

  for(int i=0; i<strLen; i++) {
    for(int j=0; j<charsToRemoveLen; j++) {
      if(str[i] == charsToRemove[j]) {
        for(int k=i; k<strLen; k++)
          str[k] = str[k+1];
        i--;
        strLen--;
      }
    }
  }

  return str;
}

conf_entries_t* configuration_parse_file(FILE* file) {
  if(file == NULL)
    return NULL;
  conf_entries_t* ent = initializeEntries(DEFAULT_INIT_SIZE);
  if(!ent)
    return NULL;

  char lineBuffer[DEFAULT_MAX_LINE_SIZE];
  char* p;
  char* tokens[2];
  int counter;

  while(fgets(lineBuffer, DEFAULT_MAX_LINE_SIZE, file) != NULL) {
    p = removeCharacters(lineBuffer, " \t\n");
    if(*p == '\0' || *p == DEFAULT_COMMENT_TOKEN)
      continue;
    p = strtok(p, "=");
    counter = 0;
    while(p != NULL) {
      if(counter < 2)
        tokens[counter] = p;
      p = strtok(NULL, "=");
      counter++;
    }
    if(counter == 2) {
      if(addEntry(ent, tokens[0], tokens[1]) != 0) {
        configuration_free(&ent);
        return NULL;
      }
    }
    else {
      configuration_free(&ent);
      return NULL;
    }
  }

  if(ferror(file)) {
    configuration_free(&ent);
    return NULL;
  }

  if(ent->entries_number != ent->real_entries_number) {
    ent->keys = (char**) realloc(ent->keys, sizeof(char*) * ent->entries_number);
    ent->values = (char**) realloc(ent->values, sizeof(char*) * ent->entries_number);
    ent->real_entries_number = ent->entries_number;
    if(ent->values == NULL || ent->keys == NULL) {
      configuration_free(&ent);
      return NULL;
    }
  }

  return ent;
}

void configuration_free(conf_entries_t** entries) {
  if(entries == NULL || *entries == NULL)
    return;

  conf_entries_t* p = *entries;

  if(p->values != NULL)
    for(int i=0; i<p->real_entries_number; i++)
      free(p->values[i]);

  if(p->keys != NULL)
    for(int i=0; i<p->real_entries_number; i++)
      free(p->keys[i]);

  free(p->values);
  free(p->keys);
  free(p);
  *entries = NULL;
}

char* configuration_read_value(conf_entries_t* entries, char* key) {
  if(entries == NULL || key == NULL)
    return NULL;

  for(int i=0; i<entries->entries_number; i++)
    if(strcmp(entries->keys[i], key) == 0)
      return entries->values[i];

  return NULL;
}
