#include "types.h"
#include "user.h"

int
main(int argc, char **argv)
{
  if(argc < 1 ){
    printf(2, "usage: history history_id\n");
    exit();
  }

  int historyId = atoi(argv[1]);

  if (history(historyId)<0){
    printf(2, "Error: uable to retrive history with id %s\n", argv[1]);
  }
  
  exit();
}
