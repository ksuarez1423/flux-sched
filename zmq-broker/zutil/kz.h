#ifndef HAVE_FLUX_KZ_H
#define HAVE_FLUX_KZ_H

typedef struct kz_struct *kz_t;

typedef void (*kz_ready_f) (kz_t kz, void *arg);

enum {
    KZ_FLAGS_READ           = 0x0001, /* choose read or write, not both */
    KZ_FLAGS_WRITE          = 0x0002,
    KZ_FLAGS_MODEMASK       = 0x0003,

    KZ_FLAGS_NONBLOCK       = 0x0010, /* currently only applies to reads */

    KZ_FLAGS_TRUNC          = 0x0100, /* remove contents before writing */
    KZ_FLAGS_DELAYCOMMIT    = 0x0200, /* commit on flush/close */
};    

/* Prepare to read or write a KVS stream.
 */
kz_t kz_open (flux_t h, const char *name, int flags);

/* Write one block of data to a KVS stream.  Unless kz was opened with
 * KZ_FLAGS_DELAYCOMMIT, data will committed to the KVS.
 * Returns len (no short writes) or -1 on error with errno set.
 */
int kz_put (kz_t kz, char *data, int len);

/* Read one block of data to a KVS stream.  Returns the number of bytes
 * read, 0 on EOF, or -1 on errro with errno set.  If no data is available,
 * kz_get() will return EAGAIN if kz was opened with KZ_FLAGS_NONBLOCK;
 * otherwise it will block until data is available.
 */
int kz_get (kz_t kz, char **datap);

/* Commit any data written to the stream which has not already
 * been committed.  Calling this on a kz opened with KZ_FLAGS_READ is a no-op.
 */
int kz_flush (kz_t kz);

/* Destroy the kz handle.  If kz was opened with KZ_FLAGS_WRITE, write
 * an EOF and commit any data written to the strewam  which has not already
 * been committed.
 */
int kz_close (kz_t kz);

/* Register a callback that will be called when data is available to
 * be read from kz.  Call kz_open with (KZ_FLAGS_READ | KZ_FLAGS_NONBLOCK).
 * Your function should call kz_get() until it returns -1, errno = EAGAIN.
 */
int kz_set_ready_cb (kz_t kz, kz_ready_f ready_cb, void *arg);

#endif /* !HAVE_FLUX_KZ_H */

/*
 * vi:tabstop=4 shiftwidth=4 expandtab
 */
