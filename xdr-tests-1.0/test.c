/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "test.h"

bool_t
xdr_stringentry (XDR *xdrs, stringentry *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->item, 1024))
		 return FALSE;
	 if (!xdr_pointer (xdrs, (char **)&objp->next, sizeof (stringentry), (xdrproc_t) xdr_stringentry))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_stringlist (XDR *xdrs, stringlist *objp)
{
	register int32_t *buf;

	 if (!xdr_pointer (xdrs, (char **)objp, sizeof (stringentry), (xdrproc_t) xdr_stringentry))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_error2 (XDR *xdrs, error2 *objp)
{
	register int32_t *buf;

	 if (!xdr_quad_t (xdrs, &objp->subcode))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->message, ~0))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_error (XDR *xdrs, error *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->code))
		 return FALSE;
	switch (objp->code) {
	case 0:
		break;
	case 1:
		 if (!xdr_string (xdrs, &objp->error_u.message, ~0))
			 return FALSE;
		break;
	case 2:
		 if (!xdr_error2 (xdrs, &objp->error_u.error2))
			 return FALSE;
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

bool_t
xdr_request (XDR *xdrs, request *objp)
{
	register int32_t *buf;

	 if (!xdr_stringlist (xdrs, &objp->strings))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_reply (XDR *xdrs, reply *objp)
{
	register int32_t *buf;

	 if (!xdr_bool (xdrs, &objp->ok))
		 return FALSE;
	switch (objp->ok) {
	case TRUE:
		 if (!xdr_stringlist (xdrs, &objp->reply_u.strings))
			 return FALSE;
		break;
	case FALSE:
		 if (!xdr_error (xdrs, &objp->reply_u.err))
			 return FALSE;
		break;
	default:
		return FALSE;
	}
	return TRUE;
}