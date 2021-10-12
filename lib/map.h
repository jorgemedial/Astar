# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>

# define N_NODES 23895681

const char delim[] = "|";

struct node{
  unsigned long id;  // Node identification
  char *name;
  double lat, lon;  // Node position
  unsigned short nsucc;  // Number of node successors; i. e. length of successors
  unsigned long *successors;
};


bool starts_with(const char *a, const char *b){
   if(strncmp(a, b, strlen(b)) == 0) return 1;
   else return 0;
}

void process_way(char * line){
  bool check_way(char * line){
    // Discard ways with less than two nodes, or with node ids without nodes
    return 1;
  }

  // Tokenize with delimiter |
  char *end = line;
  char *tok = strsep(&end, delim);  // we can skip the first field
  while ((tok = strsep(&end, delim)) != NULL){
    printf("%s\n", tok);
  }
  printf("---------------\n");
}

void process_node(char * line, struct node ** nodes, int i){
  // Allocate space for new node
  nodes[i] = malloc(sizeof(struct node));

  // Tokenize with delimiter |
  char *end = line;
  char *tok = strsep(&end, delim);  // we can skip the first field
  int counter = 0;
  while ((tok = strsep(&end, delim)) != NULL){
    switch (counter){
      case 0:
        nodes[i]->id = strtoul(tok, &tok, 10);
      case 1:
        // TODO
        nodes[i]->name = (char *) malloc(strlen(tok));
        strcpy(nodes[i]->name, tok);
      case 8:
        nodes[i]->lat = atof(tok);
      case 9:
        nodes[i]->lon = atof(tok);
    }
    counter++;
  }
}

void print_nodes(struct node ** nodes){
  for(int i = 0;i < N_NODES; i++){
    printf("Node id of node %d: %ld\n", i, nodes[i]->id);
    printf("Name of node %d: %s\n", i, nodes[i]->name);
    printf("Latitude of node %d: %f\n", i, nodes[i]->lat);
    printf("Longitude of node %d: %f\n", i, nodes[i]->lon);
    printf("No. of successors of node %d: %d\n\n", i, nodes[i]->nsucc);
  }
}

void create_map(char * path){
  FILE * fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;

  if ((fp = fopen(path ,"r")) == NULL){
    printf("Cannot open file.\n");
    exit(1);
  }

  // Create nodes array
  struct node ** nodes = malloc(N_NODES * sizeof(struct node *));
  int no_nodes = 0;

  // Read line by line
  while ((read = getline(&line, &len, fp)) != -1) {
    if (starts_with(line, "node")) {
      process_node(line, nodes, no_nodes);
      no_nodes++;
    }
    else if (starts_with(line, "way")) process_way(line);
    else continue;  // skip lines starting with # or relations
  }

  fclose(fp);
  if (line) free(line);

  print_nodes(nodes);
}
