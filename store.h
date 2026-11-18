#ifndef STORE_H
#define STORE_H

/* Sizes from the assignment: IDs 1-15 chars, names 1-31 chars, lines <= 128 chars. */
#define ID_LEN    16    /* 15 + '\0' */
#define NAME_LEN  32    /* 31 + '\0' */
#define LINE_LEN  256   /* 128 + "\r\n\0" with room to spare */
#define NO_PRICE  (-1)  /* product has no selling price yet */

/* One batch of stock. Batches of a product form a FIFO queue: head = oldest. */
typedef struct Batch {
    int id;              /* global batch ID, never reused */
    int qty;             /* units left in this batch (> 0 while it is in the list) */
    int cost;            /* unit cost */
    struct Batch *next;
} Batch;

/* One product. The product list is kept sorted by id (strcmp), so every
   "in ID order" output is just a walk from the head. */
typedef struct Product {
    char id[ID_LEN];
    char name[NAME_LEN]; /* kept exactly, including leading/trailing spaces */
    int price;           /* NO_PRICE if not set */
    Batch *head;         /* oldest batch (taken first) */
    Batch *tail;         /* newest batch (new imports go here); NULL when empty */
    struct Product *next;
} Product;

/* The current promotion: if the cart has >= n of a, product b costs p% of its price. */
typedef struct Promo {
    int active;          /* 0 = no promotion */
    char a[ID_LEN];
    int n;
    char b[ID_LEN];
    int p;
} Promo;

/* Everything SAVE writes and LOAD restores. Carts are NOT in here. */
typedef struct Store {
    Product *products;   /* sorted by id */
    Promo promo;
    int revenue;         /* accumulated TOTALs */
    int cogs;            /* accumulated COSTs */
    int gift_cost;       /* accumulated gift costs */
    int next_batch_id;   /* starts at 1 */
} Store;

/* ---- setup / teardown ---- */
void store_init(Store *s);                 /* empty store, next_batch_id = 1 */
void store_free(Store *s);                 /* free all products and batches, then store_init */

/* ---- products ---- */
Product *find_product(const Store *s, const char *pid);
/* Return the product with this id, creating it (no price, no stock) in sorted
   position if it does not exist yet. */
Product *get_or_create_product(Store *s, const char *pid, const char *name);

/* ---- batches ---- */
void add_batch(Product *p, int batch_id, int qty, int cost);   /* append at tail */
/* Take qty units FIFO (caller has checked stock). Frees every batch that
   reaches 0 immediately. Returns the total cost of the units taken. */
int take_fifo(Product *p, int qty);
int product_stock(const Product *p);       /* sum of qty over batches */
int product_value(const Product *p);       /* sum of qty * cost over batches */

/* ---- helpers ---- */
void strip_newline(char *line);            /* remove trailing '\n' and '\r' only */

/* ---- commands (each prints its own output line(s)) ---- */
void cmd_import(Store *s, const char *filename);
void cmd_price(Store *s, const char *pid, int price);
void cmd_query(const Store *s, const char *pid);
void cmd_report(const Store *s);

#endif