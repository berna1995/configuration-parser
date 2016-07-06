# configuration-parser
A very simple configuration parser in C.

With this simple library you can easily parse configuration files formatted as follow:
```
# This is a comment
Key1  =  Value1
Key2  =  Value2
...
KeyN  =  ValueN
```
## Library Usage
To use the configuration parser just include the header and use the functions like that:

```C
FILE* conf_file = fopen("path/to/conf/file");
conf_entries_t* conf = configuration_parse_file(conf_file);
if(conf == NULL)
  printf("Invalid configuration file...");
else {
  char* value = configuration_read_value(conf, "Key1");
  if(value == NULL)
    printf("There's no value for key: Key1");
  else {
    doSomething();
    doSomething();
    doSomething();
  }
  configuration_free(&conf);
}
```

## Project Structure
* _/samples: contains some configuration file examples used by the test file_
* _/src: contains all the source and header files_
* _Makefile: the makefile yay!_
* _LICENSE: the license of the project_
* _README.md: this description!_

## Makefile
To test the library just use:
> make check

To just compile the test file use:
> make test
g
To clean the build directory use:
> make clean
