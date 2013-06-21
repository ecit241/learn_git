#ifndef _MYDB_H_
#define _MYDB_H_

# include <string.h>
# include <assert.h>

#define SQLITE_LIMIT_LENGTH                    0
#define SQLITE_LIMIT_SQL_LENGTH                1
#define SQLITE_LIMIT_COLUMN                    2
#define SQLITE_LIMIT_EXPR_DEPTH                3
#define SQLITE_LIMIT_COMPOUND_SELECT           4
#define SQLITE_LIMIT_VDBE_OP                   5
#define SQLITE_LIMIT_FUNCTION_ARG              6
#define SQLITE_LIMIT_ATTACHED                  7
#define SQLITE_LIMIT_LIKE_PATTERN_LENGTH       8
#define SQLITE_LIMIT_VARIABLE_NUMBER           9
#define SQLITE_LIMIT_TRIGGER_DEPTH            10

/*
** Integers of known sizes.  These typedefs might change for architectures
** where the sizes very.  Preprocessor macros are available so that the
** types can be conveniently redefined at compile-type.  Like this:
**
**         cc '-DUINTPTR_TYPE=long long int' ...
*/
#ifndef UINT32_TYPE
# ifdef HAVE_UINT32_T
#  define UINT32_TYPE uint32_t
# else
#  define UINT32_TYPE unsigned int
# endif
#endif
#ifndef UINT16_TYPE
# ifdef HAVE_UINT16_T
#  define UINT16_TYPE uint16_t
# else
#  define UINT16_TYPE unsigned short int
# endif
#endif
#ifndef INT16_TYPE
# ifdef HAVE_INT16_T
#  define INT16_TYPE int16_t
# else
#  define INT16_TYPE short int
# endif
#endif
#ifndef UINT8_TYPE
# ifdef HAVE_UINT8_T
#  define UINT8_TYPE uint8_t
# else
#  define UINT8_TYPE unsigned char
# endif
#endif
#ifndef INT8_TYPE
# ifdef HAVE_INT8_T
#  define INT8_TYPE int8_t
# else
#  define INT8_TYPE signed char
# endif
#endif
#ifndef LONGDOUBLE_TYPE
# define LONGDOUBLE_TYPE long double
#endif

typedef sqlite_int64 i64;          /* 8-byte signed integer */
typedef sqlite_uint64 u64;         /* 8-byte unsigned integer */
typedef UINT32_TYPE u32;           /* 4-byte unsigned integer */
typedef UINT16_TYPE u16;           /* 2-byte unsigned integer */
typedef INT16_TYPE i16;            /* 2-byte signed integer */
typedef UINT8_TYPE u8;             /* 1-byte unsigned integer */
typedef INT8_TYPE i8;              /* 1-byte signed integer */

/*
** Forward references to structures
*/
typedef struct AggInfo AggInfo;
typedef struct AuthContext AuthContext;
typedef struct AutoincInfo AutoincInfo;
typedef struct Bitvec Bitvec;
typedef struct RowSet RowSet;
typedef struct CollSeq CollSeq;
typedef struct Column Column;
typedef struct Db Db;
typedef struct Schema Schema;
typedef struct Expr Expr;
typedef struct ExprList ExprList;
typedef struct ExprSpan ExprSpan;
typedef struct FKey FKey;
typedef struct FuncDef FuncDef;
typedef struct FuncDefHash FuncDefHash;
typedef struct IdList IdList;
typedef struct Index Index;
typedef struct IndexSample IndexSample;
typedef struct KeyClass KeyClass;
typedef struct KeyInfo KeyInfo;
typedef struct Lookaside Lookaside;
typedef struct LookasideSlot LookasideSlot;
typedef struct Module Module;
typedef struct NameContext NameContext;
typedef struct Parse Parse;
typedef struct Savepoint Savepoint;
typedef struct Select Select;
typedef struct SrcList SrcList;
typedef struct StrAccum StrAccum;
typedef struct Table Table;
typedef struct TableLock TableLock;
typedef struct Token Token;
typedef struct TriggerPrg TriggerPrg;
typedef struct TriggerStep TriggerStep;
typedef struct Trigger Trigger;
typedef struct UnpackedRecord UnpackedRecord;
typedef struct VTable VTable;
typedef struct Walker Walker;
typedef struct WherePlan WherePlan;
typedef struct WhereInfo WhereInfo;
typedef struct WhereLevel WhereLevel;

/*
** Forward declarations of structure
*/
typedef struct Btree Btree;
typedef struct BtCursor BtCursor;
typedef struct BtShared BtShared;
typedef struct BtreeMutexArray BtreeMutexArray;

/* Forward declarations */
typedef struct MemPage MemPage;
typedef struct BtLock BtLock;

/*
** The type used to represent a page number.  The first page in a file
** is called page 1.  0 is used to represent "not a page".
*/
typedef u32 Pgno;

/*
** Each open file is managed by a separate instance of the "Pager" structure.
*/
typedef struct Pager Pager;

/*
** Handle type for pages.
*/
typedef struct PgHdr DbPage;


typedef struct PgHdr PgHdr;
typedef struct PCache PCache;

/*
** Every page in the cache is controlled by an instance of the following
** structure.
*/
struct PgHdr {
  void *pData;                   /* Content of this page */
  void *pExtra;                  /* Extra content */
  PgHdr *pDirty;                 /* Transient list of dirty pages */
  Pgno pgno;                     /* Page number for this page */
  Pager *pPager;                 /* The pager this page is part of */
#ifdef SQLITE_CHECK_PAGES
  u32 pageHash;                  /* Hash of page content */
#endif
  u16 flags;                     /* PGHDR flags defined below */

  /**********************************************************************
  ** Elements above are public.  All that follows is private to pcache.c
  ** and should not be accessed by other modules.
  */
  i16 nRef;                      /* Number of users of this page */
  PCache *pCache;                /* Cache that owns this page */

  PgHdr *pDirtyNext;             /* Next element in list of dirty pages */
  PgHdr *pDirtyPrev;             /* Previous element in list of dirty pages */
};


/*
** A hash table for function definitions.
**
** Hash each FuncDef structure into one of the FuncDefHash.a[] slots.
** Collisions are on the FuncDef.pHash chain.
*/
struct FuncDefHash {
  FuncDef *a[23];       /* Hash table for functions */
};



struct CollSeq {
  char *zName;          /* Name of the collating sequence, UTF-8 encoded */
  u8 enc;               /* Text encoding handled by xCmp() */
  u8 type;              /* One of the SQLITE_COLL_... values below */
  void *pUser;          /* First argument to xCmp() */
  int (*xCmp)(void*,int, const void*, int, const void*);
  void (*xDel)(void*);  /* Destructor for pUser */
};

/*
** Each database file to be accessed by the system is an instance
** of the following structure.  There are normally two of these structures
** in the sqlite.aDb[] array.  aDb[0] is the main database file and
** aDb[1] is the database file used to hold temporary tables.  Additional
** databases may be attached.
*/
struct Db {
  char *zName;         /* Name of this database */
  Btree *pBt;          /* The B*Tree structure for this database file */
  u8 inTrans;          /* 0: not writable.  1: Transaction.  2: Checkpoint */
  u8 safety_level;     /* How aggressive at syncing data to disk */
  Schema *pSchema;     /* Pointer to database schema (possibly shared) */
};



/*
** The number of different kinds of things that can be limited
** using the sqlite3_limit() interface.
*/
#define SQLITE_N_LIMIT (SQLITE_LIMIT_TRIGGER_DEPTH+1)


/*
** Lookaside malloc is a set of fixed-size buffers that can be used
** to satisfy small transient memory allocation requests for objects
** associated with a particular database connection.  The use of
** lookaside malloc provides a significant performance enhancement
** (approx 10%) by avoiding numerous malloc/free requests while parsing
** SQL statements.
**
** The Lookaside structure holds configuration information about the
** lookaside malloc subsystem.  Each available memory allocation in
** the lookaside subsystem is stored on a linked list of LookasideSlot
** objects.
**
** Lookaside allocations are only allowed for objects that are associated
** with a particular database connection.  Hence, schema information cannot
** be stored in lookaside because in shared cache mode the schema information
** is shared by multiple database connections.  Therefore, while parsing
** schema information, the Lookaside.bEnabled flag is cleared so that
** lookaside allocations are not used to construct the schema objects.
*/
struct Lookaside {
  u16 sz;                 /* Size of each buffer in bytes */
  u8 bEnabled;            /* False to disable new lookaside allocations */
  u8 bMalloced;           /* True if pStart obtained from sqlite3_malloc() */
  int nOut;               /* Number of buffers currently checked out */
  int mxOut;              /* Highwater mark for nOut */
  LookasideSlot *pFree;   /* List of available buffers */
  void *pStart;           /* First byte of available memory space */
  void *pEnd;             /* First byte past end of available space */
};
struct LookasideSlot {
  LookasideSlot *pNext;    /* Next buffer in the list of free buffers */
};

/* Forward declarations of structures. */
typedef struct Hash Hash;
typedef struct HashElem HashElem;

/* A complete hash table is an instance of the following structure.
** The internals of this structure are intended to be opaque -- client
** code should not attempt to access or modify the fields of this structure
** directly.  Change this structure only by using the routines below.
** However, some of the "procedures" and "functions" for modifying and
** accessing this structure are really macros, so we can't really make
** this structure opaque.
**
** All elements of the hash table are on a single doubly-linked list.
** Hash.first points to the head of this list.
**
** There are Hash.htsize buckets.  Each bucket points to a spot in
** the global doubly-linked list.  The contents of the bucket are the
** element pointed to plus the next _ht.count-1 elements in the list.
**
** Hash.htsize and Hash.ht may be zero.  In that case lookup is done
** by a linear search of the global list.  For small tables, the 
** Hash.ht table is never allocated because if there are few elements
** in the table, it is faster to do a linear search than to manage
** the hash table.
*/
struct Hash {
  unsigned int htsize;      /* Number of buckets in the hash table */
  unsigned int count;       /* Number of entries in this table */
  HashElem *first;          /* The first element of the array */
  struct _ht {              /* the hash table */
    int count;                 /* Number of entries with this hash */
    HashElem *chain;           /* Pointer to first entry with this hash */
  } *ht;
};

/* Each element in the hash table is an instance of the following 
** structure.  All elements are stored on a single doubly-linked list.
**
** Again, this structure is intended to be opaque, but it can't really
** be opaque because it is used by macros.
*/
struct HashElem {
  HashElem *next, *prev;       /* Next and previous elements in the table */
  void *data;                  /* Data associated with this element */
  const char *pKey; int nKey;  /* Key associated with this element */
};

/*
** Each SQL function is defined by an instance of the following
** structure.  A pointer to this structure is stored in the sqlite.aFunc
** hash table.  When multiple functions have the same name, the hash table
** points to a linked list of these structures.
*/
struct FuncDef {
  i16 nArg;            /* Number of arguments.  -1 means unlimited */
  u8 iPrefEnc;         /* Preferred text encoding (SQLITE_UTF8, 16LE, 16BE) */
  u8 flags;            /* Some combination of SQLITE_FUNC_* */
  void *pUserData;     /* User data parameter */
  FuncDef *pNext;      /* Next function with same name */
  void (*xFunc)(sqlite3_context*,int,sqlite3_value**); /* Regular function */
  void (*xStep)(sqlite3_context*,int,sqlite3_value**); /* Aggregate step */
  void (*xFinalize)(sqlite3_context*);                /* Aggregate finalizer */
  char *zName;         /* SQL name of the function. */
  FuncDef *pHash;      /* Next with a different name but the same hash */
};


/*
** An instance of the following structure is used to store the busy-handler
** callback for a given sqlite handle. 
**
** The sqlite.busyHandler member of the sqlite struct contains the busy
** callback for the database handle. Each pager opened via the sqlite
** handle is passed a pointer to sqlite.busyHandler. The busy-handler
** callback is currently invoked only from within pager.c.
*/
typedef struct BusyHandler BusyHandler;
struct BusyHandler {
  int (*xFunc)(void *,int);  /* The busy callback */
  void *pArg;                /* First arg to busy callback */
  int nBusy;                 /* Incremented with each busy call */
};

struct sqlite3 {
  sqlite3_vfs *pVfs;            /* OS Interface */
  int nDb;                      /* Number of backends currently in use */
  Db *aDb;                      /* All backends */
  int flags;                    /* Miscellaneous flags. See below */
  int openFlags;                /* Flags passed to sqlite3_vfs.xOpen() */
  int errCode;                  /* Most recent error code (SQLITE_*) */
  int errMask;                  /* & result codes with this before returning */
  u8 autoCommit;                /* The auto-commit flag. */
  u8 temp_store;                /* 1: file 2: memory 0: default */
  u8 mallocFailed;              /* True if we have seen a malloc failure */
  u8 dfltLockMode;              /* Default locking-mode for attached dbs */
  u8 dfltJournalMode;           /* Default journal mode for attached dbs */
  signed char nextAutovac;      /* Autovac setting after VACUUM if >=0 */
  u8 suppressErr;               /* Do not issue error messages if true */
  int nextPagesize;             /* Pagesize after VACUUM if >0 */
  int nTable;                   /* Number of tables in the database */
  CollSeq *pDfltColl;           /* The default collating sequence (BINARY) */
  i64 lastRowid;                /* ROWID of most recent insert (see above) */
  u32 magic;                    /* Magic number for detect library misuse */
  int nChange;                  /* Value returned by sqlite3_changes() */
  int nTotalChange;             /* Value returned by sqlite3_total_changes() */
  sqlite3_mutex *mutex;         /* Connection mutex */
  int aLimit[SQLITE_N_LIMIT];   /* Limits */
  struct sqlite3InitInfo {      /* Information used during initialization */
    int iDb;                    /* When back is being initialized */
    int newTnum;                /* Rootpage of table being initialized */
    u8 busy;                    /* TRUE if currently initializing */
    u8 orphanTrigger;           /* Last statement is orphaned TEMP trigger */
  } init;
  int nExtension;               /* Number of loaded extensions */
  void **aExtension;            /* Array of shared library handles */
  struct Vdbe *pVdbe;           /* List of active virtual machines */
  int activeVdbeCnt;            /* Number of VDBEs currently executing */
  int writeVdbeCnt;             /* Number of active VDBEs that are writing */
  void (*xTrace)(void*,const char*);        /* Trace function */
  void *pTraceArg;                          /* Argument to the trace function */
  void (*xProfile)(void*,const char*,u64);  /* Profiling function */
  void *pProfileArg;                        /* Argument to profile function */
  void *pCommitArg;                 /* Argument to xCommitCallback() */   
  int (*xCommitCallback)(void*);    /* Invoked at every commit. */
  void *pRollbackArg;               /* Argument to xRollbackCallback() */   
  void (*xRollbackCallback)(void*); /* Invoked at every commit. */
  void *pUpdateArg;
  void (*xUpdateCallback)(void*,int, const char*,const char*,sqlite_int64);
  void(*xCollNeeded)(void*,sqlite3*,int eTextRep,const char*);
  void(*xCollNeeded16)(void*,sqlite3*,int eTextRep,const void*);
  void *pCollNeededArg;
  sqlite3_value *pErr;          /* Most recent error message */
  char *zErrMsg;                /* Most recent error message (UTF-8 encoded) */
  char *zErrMsg16;              /* Most recent error message (UTF-16 encoded) */
  union {
    volatile int isInterrupted; /* True if sqlite3_interrupt has been called */
    double notUsed1;            /* Spacer */
  } u1;
  Lookaside lookaside;          /* Lookaside malloc configuration */
#ifndef SQLITE_OMIT_AUTHORIZATION
  int (*xAuth)(void*,int,const char*,const char*,const char*,const char*);
                                /* Access authorization function */
  void *pAuthArg;               /* 1st argument to the access auth function */
#endif
#ifndef SQLITE_OMIT_PROGRESS_CALLBACK
  int (*xProgress)(void *);     /* The progress callback */
  void *pProgressArg;           /* Argument to the progress callback */
  int nProgressOps;             /* Number of opcodes for progress callback */
#endif
#ifndef SQLITE_OMIT_VIRTUALTABLE
  Hash aModule;                 /* populated by sqlite3_create_module() */
  Table *pVTab;                 /* vtab with active Connect/Create method */
  VTable **aVTrans;             /* Virtual tables with open transactions */
  int nVTrans;                  /* Allocated size of aVTrans */
  VTable *pDisconnect;    /* Disconnect these in next sqlite3_prepare() */
#endif
  FuncDefHash aFunc;            /* Hash table of connection functions */
  Hash aCollSeq;                /* All collating sequences */
  BusyHandler busyHandler;      /* Busy callback */
  int busyTimeout;              /* Busy handler timeout, in msec */
  Db aDbStatic[2];              /* Static space for the 2 default backends */
  Savepoint *pSavepoint;        /* List of active savepoints */
  int nSavepoint;               /* Number of non-transaction savepoints */
  int nStatement;               /* Number of nested statement-transactions  */
  u8 isTransactionSavepoint;    /* True if the outermost savepoint is a TS */
  i64 nDeferredCons;            /* Net deferred constraints this transaction. */

#ifdef SQLITE_ENABLE_UNLOCK_NOTIFY
  /* The following variables are all protected by the STATIC_MASTER 
  ** mutex, not by sqlite3.mutex. They are used by code in notify.c. 
  **
  ** When X.pUnlockConnection==Y, that means that X is waiting for Y to
  ** unlock so that it can proceed.
  **
  ** When X.pBlockingConnection==Y, that means that something that X tried
  ** tried to do recently failed with an SQLITE_LOCKED error due to locks
  ** held by Y.
  */
  sqlite3 *pBlockingConnection; /* Connection that caused SQLITE_LOCKED */
  sqlite3 *pUnlockConnection;           /* Connection to watch for unlock */
  void *pUnlockArg;                     /* Argument to xUnlockNotify */
  void (*xUnlockNotify)(void **, int);  /* Unlock notify callback */
  sqlite3 *pNextBlocked;        /* Next in list of all blocked connections */
#endif
};

/*
** An instance of this object represents a single database file.
** 
** A single database file can be in use as the same time by two
** or more database connections.  When two or more connections are
** sharing the same database file, each connection has it own
** private Btree object for the file and each of those Btrees points
** to this one BtShared object.  BtShared.nRef is the number of
** connections currently sharing this database file.
**
** Fields in this structure are accessed under the BtShared.mutex
** mutex, except for nRef and pNext which are accessed under the
** global SQLITE_MUTEX_STATIC_MASTER mutex.  The pPager field
** may not be modified once it is initially set as long as nRef>0.
** The pSchema field may be set once under BtShared.mutex and
** thereafter is unchanged as long as nRef>0.
**
** isPending:
**
**   If a BtShared client fails to obtain a write-lock on a database
**   table (because there exists one or more read-locks on the table),
**   the shared-cache enters 'pending-lock' state and isPending is
**   set to true.
**
**   The shared-cache leaves the 'pending lock' state when either of
**   the following occur:
**
**     1) The current writer (BtShared.pWriter) concludes its transaction, OR
**     2) The number of locks held by other connections drops to zero.
**
**   while in the 'pending-lock' state, no connection may start a new
**   transaction.
**
**   This feature is included to help prevent writer-starvation.
*/
struct BtShared {
  Pager *pPager;        /* The page cache */
  sqlite3 *db;          /* Database connection currently using this Btree */
  BtCursor *pCursor;    /* A list of all open cursors */
  MemPage *pPage1;      /* First page of the database */
  u8 readOnly;          /* True if the underlying file is readonly */
  u8 pageSizeFixed;     /* True if the page size can no longer be changed */
#ifndef SQLITE_OMIT_AUTOVACUUM
  u8 autoVacuum;        /* True if auto-vacuum is enabled */
  u8 incrVacuum;        /* True if incr-vacuum is enabled */
#endif
  u16 pageSize;         /* Total number of bytes on a page */
  u16 usableSize;       /* Number of usable bytes on each page */
  u16 maxLocal;         /* Maximum local payload in non-LEAFDATA tables */
  u16 minLocal;         /* Minimum local payload in non-LEAFDATA tables */
  u16 maxLeaf;          /* Maximum local payload in a LEAFDATA table */
  u16 minLeaf;          /* Minimum local payload in a LEAFDATA table */
  u8 inTransaction;     /* Transaction state */
  int nTransaction;     /* Number of open transactions (read + write) */
  void *pSchema;        /* Pointer to space allocated by sqlite3BtreeSchema() */
  void (*xFreeSchema)(void*);  /* Destructor for BtShared.pSchema */
  sqlite3_mutex *mutex; /* Non-recursive mutex required to access this struct */
  Bitvec *pHasContent;  /* Set of pages moved to free-list this transaction */
#ifndef SQLITE_OMIT_SHARED_CACHE
  int nRef;             /* Number of references to this structure */
  BtShared *pNext;      /* Next on a list of sharable BtShared structs */
  BtLock *pLock;        /* List of locks held on this shared-btree struct */
  Btree *pWriter;       /* Btree with currently open write transaction */
  u8 isExclusive;       /* True if pWriter has an EXCLUSIVE lock on the db */
  u8 isPending;         /* If waiting for read-locks to clear */
#endif
  u8 *pTmpSpace;        /* BtShared.pageSize bytes of space for tmp use */
};

/*
** A linked list of the following structures is stored at BtShared.pLock.
** Locks are added (or upgraded from READ_LOCK to WRITE_LOCK) when a cursor 
** is opened on the table with root page BtShared.iTable. Locks are removed
** from this list when a transaction is committed or rolled back, or when
** a btree handle is closed.
*/
struct BtLock {
  Btree *pBtree;        /* Btree handle holding this lock */
  Pgno iTable;          /* Root page of table */
  u8 eLock;             /* READ_LOCK or WRITE_LOCK */
  BtLock *pNext;        /* Next in BtShared.pLock list */
};


/* A Btree handle
**
** A database connection contains a pointer to an instance of
** this object for every database file that it has open.  This structure
** is opaque to the database connection.  The database connection cannot
** see the internals of this structure and only deals with pointers to
** this structure.
**
** For some database files, the same underlying database cache might be 
** shared between multiple connections.  In that case, each connection
** has it own instance of this object.  But each instance of this object
** points to the same BtShared object.  The database cache and the
** schema associated with the database file are all contained within
** the BtShared object.
**
** All fields in this structure are accessed under sqlite3.mutex.
** The pBt pointer itself may not be changed while there exists cursors 
** in the referenced BtShared that point back to this Btree since those
** cursors have to do go through this Btree to find their BtShared and
** they often do so without holding sqlite3.mutex.
*/
struct Btree {
  sqlite3 *db;       /* The database connection holding this btree */
  BtShared *pBt;     /* Sharable content of this btree */
  u8 inTrans;        /* TRANS_NONE, TRANS_READ or TRANS_WRITE */
  u8 sharable;       /* True if we can share pBt with another db */
  u8 locked;         /* True if db currently has pBt locked */
  int wantToLock;    /* Number of nested calls to sqlite3BtreeEnter() */
  int nBackup;       /* Number of backup operations reading this btree */
  Btree *pNext;      /* List of other sharable Btrees from the same db */
  Btree *pPrev;      /* Back pointer of the same list */
#ifndef SQLITE_OMIT_SHARED_CACHE
  BtLock lock;       /* Object used to lock page 1 */
#endif
};

/*
** An instance of the following structure is allocated for each active
** savepoint and statement transaction in the system. All such structures
** are stored in the Pager.aSavepoint[] array, which is allocated and
** resized using sqlite3Realloc().
**
** When a savepoint is created, the PagerSavepoint.iHdrOffset field is
** set to 0. If a journal-header is written into the main journal while
** the savepoint is active, then iHdrOffset is set to the byte offset 
** immediately following the last journal record written into the main
** journal before the journal-header. This is required during savepoint
** rollback (see pagerPlaybackSavepoint()).
*/
typedef struct PagerSavepoint PagerSavepoint;
struct PagerSavepoint {
  i64 iOffset;                 /* Starting offset in main journal */
  i64 iHdrOffset;              /* See above */
  Bitvec *pInSavepoint;        /* Set of pages in this savepoint */
  Pgno nOrig;                  /* Original number of pages in file */
  Pgno iSubRec;                /* Index of first record in sub-journal */
};

/*
** A complete page cache is an instance of this structure.
*/
struct PCache {
  PgHdr *pDirty, *pDirtyTail;         /* List of dirty pages in LRU order */
  PgHdr *pSynced;                     /* Last synced page in dirty page list */
  int nRef;                           /* Number of referenced pages */
  int nMax;                           /* Configured cache size */
  int szPage;                         /* Size of every page in this cache */
  int szExtra;                        /* Size of extra space for each page */
  int bPurgeable;                     /* True if pages are on backing store */
  int (*xStress)(void*,PgHdr*);       /* Call to try make a page clean */
  void *pStress;                      /* Argument to xStress */
  sqlite3_pcache *pCache;             /* Pluggable cache module */
  PgHdr *pPage1;                      /* Reference to page 1 */
};



struct Pager {
  sqlite3_vfs *pVfs;          /* OS functions to use for IO */
  u8 exclusiveMode;           /* Boolean. True if locking_mode==EXCLUSIVE */
  u8 journalMode;             /* On of the PAGER_JOURNALMODE_* values */
  u8 useJournal;              /* Use a rollback journal on this file */
  u8 noReadlock;              /* Do not bother to obtain readlocks */
  u8 noSync;                  /* Do not sync the journal if true */
  u8 fullSync;                /* Do extra syncs of the journal for robustness */
  u8 sync_flags;              /* One of SYNC_NORMAL or SYNC_FULL */
  u8 tempFile;                /* zFilename is a temporary file */
  u8 readOnly;                /* True for a read-only database */
  u8 memDb;                   /* True to inhibit all file I/O */

  /* The following block contains those class members that are dynamically
  ** modified during normal operations. The other variables in this structure
  ** are either constant throughout the lifetime of the pager, or else
  ** used to store configuration parameters that affect the way the pager 
  ** operates.
  **
  ** The 'state' variable is described in more detail along with the
  ** descriptions of the values it may take - PAGER_UNLOCK etc. Many of the
  ** other variables in this block are described in the comment directly 
  ** above this class definition.
  */
  u8 state;                   /* PAGER_UNLOCK, _SHARED, _RESERVED, etc. */
  u8 dbModified;              /* True if there are any changes to the Db */
  u8 needSync;                /* True if an fsync() is needed on the journal */
  u8 journalStarted;          /* True if header of journal is synced */
  u8 changeCountDone;         /* Set after incrementing the change-counter */
  u8 setMaster;               /* True if a m-j name has been written to jrnl */
  u8 doNotSync;               /* Boolean. While true, do not spill the cache */
  u8 dbSizeValid;             /* Set when dbSize is correct */
  u8 subjInMemory;            /* True to use in-memory sub-journals */
  Pgno dbSize;                /* Number of pages in the database */
  Pgno dbOrigSize;            /* dbSize before the current transaction */
  Pgno dbFileSize;            /* Number of pages in the database file */
  int errCode;                /* One of several kinds of errors */
  int nRec;                   /* Pages journalled since last j-header written */
  u32 cksumInit;              /* Quasi-random value added to every checksum */
  u32 nSubRec;                /* Number of records written to sub-journal */
  Bitvec *pInJournal;         /* One bit for each page in the database file */
  sqlite3_file *fd;           /* File descriptor for database */
  sqlite3_file *jfd;          /* File descriptor for main journal */
  sqlite3_file *sjfd;         /* File descriptor for sub-journal */
  i64 journalOff;             /* Current write offset in the journal file */
  i64 journalHdr;             /* Byte offset to previous journal header */
  PagerSavepoint *aSavepoint; /* Array of active savepoints */
  int nSavepoint;             /* Number of elements in aSavepoint[] */
  char dbFileVers[16];        /* Changes whenever database file changes */
  u32 sectorSize;             /* Assumed sector size during rollback */

  u16 nExtra;                 /* Add this many bytes to each in-memory page */
  i16 nReserve;               /* Number of unused bytes at end of each page */
  u32 vfsFlags;               /* Flags for sqlite3_vfs.xOpen() */
  int pageSize;               /* Number of bytes in a page */
  Pgno mxPgno;                /* Maximum allowed size of the database */
  char *zFilename;            /* Name of the database file */
  char *zJournal;             /* Name of the journal file */
  int (*xBusyHandler)(void*); /* Function to call when busy */
  void *pBusyHandlerArg;      /* Context argument for xBusyHandler */
#ifdef SQLITE_TEST
  int nHit, nMiss;            /* Cache hits and missing */
  int nRead, nWrite;          /* Database pages read/written */
#endif
  void (*xReiniter)(DbPage*); /* Call this routine when reloading pages */
#ifdef SQLITE_HAS_CODEC
  void *(*xCodec)(void*,void*,Pgno,int); /* Routine for en/decoding data */
  void (*xCodecSizeChng)(void*,int,int); /* Notify of page size changes */
  void (*xCodecFree)(void*);             /* Destructor for the codec */
  void *pCodec;               /* First argument to xCodec... methods */
#endif
  char *pTmpSpace;            /* Pager.pageSize bytes of space for tmp use */
  i64 journalSizeLimit;       /* Size limit for persistent journal files */
  PCache *pPCache;            /* Pointer to page cache object */
  sqlite3_backup *pBackup;    /* Pointer to list of ongoing backup processes */
};

/*
** CAPI3REF: Return The Filename For A Database Connection
**
** ^The sqlite3_db_filename(D,N) interface returns a pointer to a filename
** associated with database N of connection D.  ^The main database file
** has the name "main".  If there is no attached database N on the database
** connection D, or if database N is a temporary or in-memory database, then
** a NULL pointer is returned.
**
** ^The filename returned by this function is the output of the
** xFullPathname method of the [VFS].  ^In other words, the filename
** will be an absolute pathname, even if the filename used
** to open the database originally was a URI or relative pathname.
*/
SQLITE_API const char *sqlite3_db_filename(sqlite3 *db, const char *zDbName);

/*
** Internal function prototypes
*/
#define sqlite3StrICmp sqlite3_stricmp
#define sqlite3StrNICmp sqlite3_strnicmp

/* An array to map all upper-case characters into their corresponding
** lower-case character. 
**
** SQLite only considers US-ASCII (or EBCDIC) characters.  We do not
** handle case conversions for the UTF character set since the tables
** involved are nearly as big or bigger than SQLite itself.
*/
const unsigned char sqlite3UpperToLower[] = {
#ifdef SQLITE_ASCII
      0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17,
     18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
     36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53,
     54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 97, 98, 99,100,101,102,103,
    104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,
    122, 91, 92, 93, 94, 95, 96, 97, 98, 99,100,101,102,103,104,105,106,107,
    108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,
    126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,
    144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,
    162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,
    180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,
    198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,
    216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,
    234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,
    252,253,254,255
#endif
#ifdef SQLITE_EBCDIC
      0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, /* 0x */
     16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, /* 1x */
     32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, /* 2x */
     48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, /* 3x */
     64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, /* 4x */
     80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, /* 5x */
     96, 97, 66, 67, 68, 69, 70, 71, 72, 73,106,107,108,109,110,111, /* 6x */
    112, 81, 82, 83, 84, 85, 86, 87, 88, 89,122,123,124,125,126,127, /* 7x */
    128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143, /* 8x */
    144,145,146,147,148,149,150,151,152,153,154,155,156,157,156,159, /* 9x */
    160,161,162,163,164,165,166,167,168,169,170,171,140,141,142,175, /* Ax */
    176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191, /* Bx */
    192,129,130,131,132,133,134,135,136,137,202,203,204,205,206,207, /* Cx */
    208,145,146,147,148,149,150,151,152,153,218,219,220,221,222,223, /* Dx */
    224,225,162,163,164,165,166,167,168,169,232,203,204,205,206,207, /* Ex */
    239,240,241,242,243,244,245,246,247,248,249,219,220,221,222,255, /* Fx */
#endif
};


/* Convenient short-hand */
#define UpperToLower sqlite3UpperToLower

/*
** Some systems have stricmp().  Others have strcasecmp().  Because
** there is no consistency, we will define our own.
**
** IMPLEMENTATION-OF: R-30243-02494 The sqlite3_stricmp() and
** sqlite3_strnicmp() APIs allow applications and extensions to compare
** the contents of two buffers containing UTF-8 strings in a
** case-independent fashion, using the same definition of "case
** independence" that SQLite uses internally when comparing identifiers.
*/
SQLITE_API int sqlite3_stricmp(const char *zLeft, const char *zRight){
  register unsigned char *a, *b;
  a = (unsigned char *)zLeft;
  b = (unsigned char *)zRight;
  while( *a!=0 && UpperToLower[*a]==UpperToLower[*b]){ a++; b++; }
  return UpperToLower[*a] - UpperToLower[*b];
}
SQLITE_API int sqlite3_strnicmp(const char *zLeft, const char *zRight, int N){
  register unsigned char *a, *b;
  a = (unsigned char *)zLeft;
  b = (unsigned char *)zRight;
  while( N-- > 0 && *a!=0 && UpperToLower[*a]==UpperToLower[*b]){ a++; b++; }
  return N<0 ? 0 : UpperToLower[*a] - UpperToLower[*b];
}

/*
** Return the full pathname of the database file.
**
** Except, if the pager is in-memory only, then return an empty string if
** nullIfMemDb is true.  This routine is called with nullIfMemDb==1 when
** used to report the filename to the user, for compatibility with legacy
** behavior.  But when the Btree needs to know the filename for matching to
** shared cache, it uses nullIfMemDb==0 so that in-memory databases can
** participate in shared-cache.
*/
const char *sqlite3PagerFilename(Pager *pPager, int nullIfMemDb){
  return (nullIfMemDb && pPager->memDb) ? "" : pPager->zFilename;
}


/*
** Return the full pathname of the underlying database file.  Return
** an empty string if the database is in-memory or a TEMP database.
**
** The pager filename is invariant as long as the pager is
** open so it is safe to access without the BtShared mutex.
*/
const char *sqlite3BtreeGetFilename(Btree *p){
  assert( p->pBt->pPager!=0 );
  return sqlite3PagerFilename(p->pBt->pPager, 1);
}


/*
** Return the Btree pointer identified by zDbName.  Return NULL if not found.
*/
Btree *sqlite3DbNameToBtree(sqlite3 *db, const char *zDbName){
  int i;
  for(i=0; i<db->nDb; i++){
    if( db->aDb[i].pBt
     && (zDbName==0 || sqlite3StrICmp(zDbName, db->aDb[i].zName)==0)
    ){
      return db->aDb[i].pBt;
    }
  }
  return 0;
}


/*
** Return the filename of the database associated with a database
** connection.
*/
SQLITE_API const char *sqlite3_db_filename(sqlite3 *db, const char *zDbName){
  Btree *pBt = sqlite3DbNameToBtree(db, zDbName);
  return pBt ? sqlite3BtreeGetFilename(pBt) : 0;
}


#endif
