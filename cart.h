#ifndef CART_H
#define CART_H

#include "store.h"

/* One line in a cart. Items are kept sorted by pid, so CHECKOUT walks them
   in product ID order with no extra sorting. */
typedef struct CartItem {
    char pid[ID_LEN];    /* store the ID, not a Product*: the cart never points into the store */
    int qty;             /* total after all ADDs */
    struct CartItem *next;
} CartItem;

/* An open cart. A cart is removed and freed as soon as it is checked out,
   so "in the list" == "open", and a closed cart's ID simply is not found. */
typedef struct Cart {
    char id[ID_LEN];
    CartItem *items;     /* sorted by pid */
    struct Cart *next;
} Cart;

/* ---- carts ---- */
Cart *find_cart(Cart *carts, const char *cid);
void carts_free_all(Cart **carts);         /* used by LOAD and QUIT; sets *carts = NULL */

/* ---- commands ---- */
void cmd_promo(Store *s, const char *a, int n, const char *b, int p);
void cmd_cart(Cart **carts, const char *cid);
void cmd_add(Cart *carts, const Store *s, const char *cid, const char *pid, int qty);
/* Check -> price -> take stock (FIFO) -> gifts -> totals, then close the cart
   whether it succeeded or not. */
void cmd_checkout(Cart **carts, Store *s, const char *cid);

#endif