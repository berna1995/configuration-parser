/**
 * @file conf_parser.h
 * @author Matteo Bernabito
 * @brief Header of the configuration parser library
 *
 * This very small library offer the functions to read configuration files formatted as follow:
 *
 * 		Key1 = Value1
 *    Key2 = Value2
 *    # This is a random comment
 *    Key3 = Value3
 *    ...
 *    KeyN = ValueN
 *
 */

#ifndef CONF_PARSER_H
#define CONF_PARSER_H

#include <stdio.h>

#define DEFAULT_INIT_SIZE 10
#define DEFAULT_REALLOC_STEP 10
#define DEFAULT_COMMENT_TOKEN '#'
#define DEFAULT_MAX_LINE_SIZE 2048

/** The structure containing all the entries of the configuration file */
typedef struct _conf_entries_t {
  char** keys;
  char** values;
  unsigned int entries_number;
  unsigned int real_entries_number;
} conf_entries_t;

/**
 * Function that reads all the entries in a configuration file.
 * @param  file the configuration file
 * @return a pointer to a configuration structure or NULL if something went wrong.
 */
conf_entries_t* configuration_parse_file(FILE* file);

/**
 * Utility function, it reads the the key associated value from the configuration structure.
 * @param  entries the structure containing all the configuration file entries.
 * @param  key     the key from which we want the associated value.
 * @return the string containing the associated value or NULL if the key is not in the list of entries.
 */
char* configuration_read_value(conf_entries_t* entries, char* key);

/**
 * Function that frees all the memory used by the configuration structure.
 * @param entries the pointer to the pointer of the configuration structure.
 */
void configuration_free(conf_entries_t** entries);

#endif
