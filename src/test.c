#include <conf_parser.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

int main() {
  char* value;
  char buffer1[100];
  char buffer2[100];
  int res;

  // TEST 1

  FILE* confFile = fopen("../samples/sample_one.conf", "r");
  assert(confFile != NULL);

  conf_entries_t* entries = configuration_parse_file(confFile);
  assert(entries);
  assert(entries->entries_number == 2);

  res = fclose(confFile);
  assert(res == 0);

  value = configuration_read_value(entries, "Key_1");
  assert(value != NULL);
  res = strcmp(value, "Value_1");
  assert(res == 0);

  value = configuration_read_value(entries, "Key_2");
  assert(value != NULL);
  res = strcmp(value, "Value_2");
  assert(res == 0);

  configuration_free(&entries);
  assert(entries == NULL);

  // TEST 2

  confFile = fopen("../samples/sample_two.conf", "r");
  assert(confFile != NULL);

  entries = configuration_parse_file(confFile);
  assert(entries);
  assert(entries->entries_number == 2);

  res = fclose(confFile);
  assert(res == 0);

  value = configuration_read_value(entries, "RandomKey");
  assert(value != NULL);
  res = strcmp(value, "RandomValue");
  assert(res == 0);

  value = configuration_read_value(entries, "RandomKey2");
  assert(value != NULL);
  res = strcmp(value, "RandomValue2");
  assert(res == 0);

  configuration_free(&entries);
  assert(entries == NULL);

  // TEST 3

  confFile = fopen("../samples/sample_three.conf", "r");
  assert(confFile != NULL);

  entries = configuration_parse_file(confFile);
  assert(entries);
  assert(entries->entries_number == 20);

  res = fclose(confFile);
  assert(res == 0);

  for(int i=1; i<=20; i++) {
    sprintf(buffer1, "Key%d", i);
    sprintf(buffer2, "Value%d", i);

    value = configuration_read_value(entries, buffer1);
    assert(value != NULL);
    res = strcmp(value, buffer2);
    assert(res == 0);
  }

  configuration_free(&entries);
  assert(entries == NULL);

  // TEST 4

  confFile = fopen("../samples/sample_four.conf", "r");
  assert(confFile != NULL);

  entries = configuration_parse_file(confFile);
  assert(!entries);

  res = fclose(confFile);
  assert(res == 0);

  printf("TESTS DONE!\n");
  return 0;
}
