/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _TEST_H_RPCGEN
#define _TEST_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


struct stringentry {
	char *item;
	struct stringentry *next;
};
typedef struct stringentry stringentry;

typedef stringentry *stringlist;

struct error2 {
	quad_t subcode;
	char *message;
};
typedef struct error2 error2;

struct error {
	int code;
	union {
		char *message;
		error2 error2;
	} error_u;
};
typedef struct error error;

struct request {
	stringlist strings;
};
typedef struct request request;

struct reply {
	bool_t ok;
	union {
		stringlist strings;
		error err;
	} reply_u;
};
typedef struct reply reply;

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_stringentry (XDR *, stringentry*);
extern  bool_t xdr_stringlist (XDR *, stringlist*);
extern  bool_t xdr_error2 (XDR *, error2*);
extern  bool_t xdr_error (XDR *, error*);
extern  bool_t xdr_request (XDR *, request*);
extern  bool_t xdr_reply (XDR *, reply*);

#else /* K&R C */
extern bool_t xdr_stringentry ();
extern bool_t xdr_stringlist ();
extern bool_t xdr_error2 ();
extern bool_t xdr_error ();
extern bool_t xdr_request ();
extern bool_t xdr_reply ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_TEST_H_RPCGEN */
