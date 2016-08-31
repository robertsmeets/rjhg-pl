/* -*- c -*-
 * $Id: test.x,v 1.2 2007/04/11 09:29:53 rjones Exp $
 */


/* For test1.c */
struct stringentry {
  string item<1024>;
  stringentry *next;
};

typedef stringentry *stringlist;

/* For test2.c */
struct error2 {
  hyper subcode;
  string message<>;
};

union error switch (int code) {
 case 0:
   void;
 case 1:
   string message<>;
 case 2:
   error2 error2;
};

struct request {
  stringlist strings;
};

union reply switch (bool ok) {
 case TRUE:
   stringlist strings;

 case FALSE:
   error err;
};
