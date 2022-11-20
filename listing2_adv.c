/******************************************************************************

Name: Mammoth Maze Advanced
Version: 1.0
Ported: Greenonline
Date: 19 Nov 2022
Original concept: G.T.Relf

TODO:

 - Prevent zero for x and y 
 
Original code:

10 PRINT "MAMMOTH MAZE in 2.5K BYTES BY G.T.RELF"
20 REM SET STARTING POSITION (EAST, WEST, DOWN):
30 X=103 : Y=97 : Z=1
40 REM SET THREASHOLD
50 T=0.3
60 REM RANDOMIZE FOR EARTHQUAKES
70 RANDOM
80 REM INSERT ANY OTHER INITIALISING STATEMENTS HERE.
90 REM MAIN LOOP:
100 REM COMPUTE PROPERTIES OF CURRENT LOCATION
110 GOSUB 250
120 REM GET COMMAND & BRANCH ACCORDINGLY:
130 INPUT "COMMAND"; CM$
140 CM$=LEFT(CM$,1)
150 IF CM$="N" THEN 490
160 IF CM$="E" THEN 530
170 IF CM$="W" THEN 570
180 IF CM$="S" THEN 610
190 IF CM$="U" THEN 650
200 IF CM$="D" THEN 690
210 PRINT "IMPOSSIBLE!"
220 GOTO 110
230 REM END OF MAIN LOOP
240 REM COMPUTE ATTRIBUTES OF CURRENT LOCATION:
250 GOSUB 720
255 V1=INT(W*1000)
260 IF V1>35 AND V1<39 AND Z>1 THEN PRINT "HERE IS A CROCK OF GOLD!"
270 REM (YOU WONT FIND CROCKS OF GOLD JUST LYING ON THE GROUND!)
280 REM INSERT OTHER ATTRIBUTE TESTS HERE.
290 REM CHECK FOR EARTHQUAKES:
300 IF RND(1000)=9 THEN PRINT "RUMBLE, RUMBLE..." : T=T+0.2*RND(0)-0.1
310 REM INSERT OTHER LOCATION-DEPENDANT TESTS HERE.
320 REM IDENTIFY CORRIDORS FROM CURRENT LOCATION (CHANGES W):
330 IF Z=1 THEN 440
340 PRINT "CORRIDORS: "; : NC=0
350 X=X+1: GOSUB 720 : IF W<T THEN PRINT "E "; : NC=NC+1
360 X=X-2 : GOSUB 720 : X=X+1 : IF W<T THEN PRINT "W "; : NC=NC+1
370 Y=Y+1: GOSUB 720 : IF W<T THEN PRINT "N "; : NC=NC+1
380 Y=Y-2 : GOSUB 720 : Y=Y+1 : IF W<T THEN PRINT "S "; : NC=NC+1
390 Z=Z+1: GOSUB 720 : IF W<T THEN PRINT "D "; : NC=NC+1
400 Z=Z-2 : GOSUB 720 : Z=Z+1 : IF W<T OR Z=2 THEN PRINT "U" : NC=NC+1
410 IF NC=0 THEN PRINT "NONE - THE EARTHQUAKE HAS TRAPPED YOU!"
420 PRINT
430 RETURN
440 PRINT "GROUND LEVEL."
450 Z=Z+1 : GOSUB 720 : Z=Z-1
460 IF W<T THEN PRINT "A SHAFT DESCENDS FROM HERE."
470 RETURN
480 REM MOVE NORTH:
490 Y=Y+1 : IF Z=1 THEN 110
500 GOSUB 720 : IF W<T THEN 110
510 Y=Y-1 : GOTO 210
520 REM MOVE EAST:
530 X=X+1 : IF Z=1 THEN 110
540 GOSUB 720 : IF W<T THEN 110
550 X=X-1 : GOTO 210
560 REM MOVE WEST:
570 X=X-1 : IF Z=1 THEN 110
580 GOSUB 720 : IF W<T THEN 110
590 X=X+1 : GOTO 210
600 REM MOVE SOUTH:
610 Y=Y-1 : IF Z=1 THEN 110
620 GOSUB 720 : IF W<T THEN 110
630 Y=Y+1 : GOTO 210
640 REM MOVE UP:
650 IF Z=1 THEN 210
655 Z=Z-1 : IF Z=1 THEN 110
660 GOSUB 720 : IF W<T THEN 110
670 Z=Z+1 : GOTO 210
680 REM MOVE DOWN:
690 Z=Z+1 : GOSUB 720 : IF W<T THEN 110
700 Z=Z-1 : GOTO 210 
710 REM RANDOM FUNCTION
720 U=U*100*SQR(X*X+Y*Y*Z)
730 W=U-INT(U)
740 RETURN
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
/* https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html */
#include <ctype.h>
#include <unistd.h>

/*
int x=103,y=97,z=1;
int threashold=0.3;
*/
/*int w;*/
int x, y, z;
float threshold;
int crocks;
int gold_pieces;

int DEBUG = 0;
int VERBOSE_MOVE = 0;
int VERBOSE_LOCATION = 0;

void set_random_seed() {
  srand(0);
  /* https://stackoverflow.com/questions/1694827/random-float-number */
  /* https://stackoverflow.com/questions/822323/how-to-generate-a-random-int-in-c */
  /*srand(time(NULL));*/
}

/* Generate a float between 0 and 1 */
/* https://stackoverflow.com/questions/1694827/random-float-number */
float randomFloat() {
  float r = (float) rand() / (float) RAND_MAX;
  return r;
}

float location_random() {
  if (DEBUG) {
    printf("DEBUG: multi= %d\n", x * x + y * y * z);
    printf("DEBUG: sqrt= %f\n", sqrt(x * x + y * y * z));
  }
  float u = 100 * sqrt(x * x + y * y * z);
  return (u - (int) u);
}

float w() {
  return (location_random());
}

void go_north(void) {
  y = y + 1;
  if (w() < threshold || z == 1) {
    if (VERBOSE_MOVE) {
      printf("You go north.\n");
    }  
  } else {
    printf("You can't move in that direction.\n");
    y = y - 1;
  }
}

void go_south(void) {
  y = y - 1;
  if (w() < threshold || z == 1) {
    if (VERBOSE_MOVE) {
      printf("You go south.\n");
    }
  } else {
    printf("You can't move in that direction.\n");
    y = y + 1;
  }
}

void go_east(void) {
  x = x + 1;
  if (w() < threshold || z == 1) {
    if (VERBOSE_MOVE) {
      printf("You go east.\n");
    }
  } else {
    printf("You can't move in that direction.\n");
    x = x - 1;
  }
}

void go_west(void) {
  x = x - 1;
  if (w() < threshold || z == 1) {
    if (VERBOSE_MOVE) {
      printf("You go west.\n");
    }
  } else {
    printf("You can't move in that direction.\n");
    x = x + 1;
  }
}

void go_up(void) {
  if (z == 1) {
    printf("You can't move in that direction.\n");
  } else {
    z = z - 1;
    if (w() < threshold || z == 1) {
      if (VERBOSE_MOVE) {
        printf("You go up.\n");
      }
    } else {
      printf("You can't move in that direction.\n");
      z = z + 1;
    }
  }
}

void go_down(void) {
  z = z + 1;
  if (w() < threshold) {
    if (VERBOSE_MOVE) {    
      printf("You go down.\n");
    }
  } else {
    printf("You can't move in that direction.\n");
    z = z - 1;
  }
}

void do_look(void) {
  printf("Location: %d-%d-%d\n", x, y, z);
  printf("Crocks: %d\n", crocks);
  printf("Gold pieces: %d\n", gold_pieces);
}

/* Level one functions */

void set_options(int argc, char **argv) {
  /* https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html */
  int aflag = 0;
  int bflag = 0;
  char *cvalue = NULL;
  int index;
  int c;

  opterr = 0;

  while ((c = getopt (argc, argv, "abc:")) != -1)
    switch (c)
      {
      case 'd':
        DEBUG = 1;
        break;
      case 'l':
        VERBOSE_LOCATION = 1;
        break;
      case 'm':
        VERBOSE_MOVE = 1;
        break;
      case 'c':
        cvalue = optarg;
        break;
      case '?':
        if (optopt == 'c')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr,
                   "Unknown option character `\\x%x'.\n",
                   optopt);
        return 1;
      default:
        abort ();
      }

}

void init(void) {
  x = 103, y = 97, z = 1;
  threshold = 0.3;
  crocks = 0;
  gold_pieces = 0;
  set_random_seed();
}

void current_location_attributes(void) {
  int v = (int)(w() * 1000);
  if (v > 35 && v < 39 && z > 1) {
    printf("HERE IS A CROCK OF GOLD!");
    /* YOU WONT FIND CROCKS OF GOLD JUST LYING ON THE GROUND! */
    int crock=rand() % 100;
    printf("You found %d gold pieces", crock);
    crocks=crocks+1;
    gold_pieces=gold_pieces+crock;
  }
  /*
     INSERT OTHER ATTRIBUTE TESTS HERE.
   */
  /* CHECK FOR EARTHQUAKES: */
  if (rand() % 1000 == 9) {
    printf("RUMBLE, RUMBLE...");
    /*threshold = threshold + 0.2 * rand() - 0.1;*/
    threshold = threshold + 0.2 * randomFloat() - 0.1;
  }
  /* INSERT OTHER LOCATION-DEPENDANT TESTS HERE.*/
  if (VERBOSE_LOCATION){
    printf("%d_%d_%d - ",x,y,z);
  }
  if (z == 1) {
    printf("Ground level - ");
    z = z + 1;
    if (w() < threshold) {
      printf("A shaft descends from here...\n");
    } else {
      printf("There is no shaft - keep searching North, South, East or West...\n");
    }
    z = z - 1;
  } else {
    printf("Corridors: ");
    int nc = 0;
    x = x + 1;
    if (w() < threshold) {
      printf("E ");
      nc = nc + 1;
    }
    x = x - 2;
    if (w() < threshold) {
      printf("W ");
      nc = nc + 1;
    }
    x = x + 1;
    y = y + 1;
    if (w() < threshold) {
      printf("N ");
      nc = nc + 1;
    }
    y = y - 2;
    if (w() < threshold) {
      printf("S ");
      nc = nc + 1;
    }
    y = y + 1;
    z = z + 1;
    if (w() < threshold) {
      printf("D ");
      nc = nc + 1;
    }
    z = z - 2;
    if (w() < threshold || z == 2) {
      printf("U");
      nc = nc + 1;
    }
    z = z + 1;
    if (nc == 0) {
      printf("None - the earthquake has trapped you.");
    }
    printf("\n");

  }
}

void get_command(void){
 char command;

 printf("Enter a direction: ");
 /* https://stackoverflow.com/questions/24099976/read-two-characters-consecutively-using-scanf-in-c */
 scanf(" %c", &command);
 /* or */
 /*scanf("%c", &command);*/
 /*getchar();*/
 switch (command) {
    case 'n':
      go_north();
      break;
    case 's':
      go_south();
      break;
    case 'e':
      go_east();
      break;
    case 'w':
      go_west();
      break;
    case 'u':
      go_up();
      break;
    case 'd':
      go_down();
      break;
    case 'l':
      do_look();
      break;
    default:
      printf("Invalid input!\n");
  }
}

int main(int argc, char **argv) {

  set_options(argc, argv);

  init();

  /* main loop */
  do {
    current_location_attributes();
    if (DEBUG) {
      printf("DEBUG: w=%f\n", w());
    }
    get_command();

  }
  while (1);

  return 0;
}
