#ifndef H_RPMCLI
#define	H_RPMCLI

/** \ingroup rpmcli rpmbuild
 * \file lib/rpmcli.h
 */

#include "rpmlib.h"
#include "rpmurl.h"
#include "rpmmacro.h"

/** \ingroup rpmcli
 * Should version 3 packages be produced?
 */
/*@-redecl@*/
extern int _noDirTokens;
/*@=redecl@*/

#ifdef __cplusplus
extern "C" {
#endif

/* ==================================================================== */
/** \name RPMBT */
/*@{*/

/** \ingroup rpmcli
 * Describe build command line request.
 */
struct rpmBuildArguments_s {
    int buildAmount;		/*!< Bit(s) to control operation. */
/*@null@*/ const char * buildRootOverride; /*!< from --buildroot */
/*@null@*/ char * targets;	/*!< Target platform(s), comma separated. */
/*@observer@*/
    const char * passPhrase;	/*!< Pass phrase. */
/*@only@*/ /*@null@*/
    const char * cookie;	/*!< NULL for binary, ??? for source, rpm's */
    int force;			/*!< from --force */
    int noBuild;		/*!< from --nobuild */
    int noDeps;			/*!< from --nodeps */
    int noLang;			/*!< from --nolang */
    int shortCircuit;		/*!< from --short-circuit */
    int sign;			/*!< from --sign */
    int useCatalog;		/*!< from --usecatalog */
    char buildMode;		/*!< Build mode (one of "btBC") */
    char buildChar;		/*!< Build stage (one of "abcilps ") */
/*@observer@*/ /*@null@*/ const char * rootdir;
};

/** \ingroup rpmcli
 */
typedef	struct rpmBuildArguments_s *	BTA_t;

/** \ingroup rpmcli
 */
/*@unchecked@*/
extern struct rpmBuildArguments_s	rpmBTArgs;

/** \ingroup rpmcli
 */
/*@unchecked@*/
extern struct poptOption		rpmBuildPoptTable[];

/*@}*/
/* ==================================================================== */
/** \name RPMQV */
/*@{*/

/** \ingroup rpmcli
 * Bit(s) to control rpmQuery() operation, stored in qva_flags.
 * @todo Merge rpmQueryFlags, rpmVerifyFlags, and rpmVerifyAttrs?.
 */
typedef enum rpmQueryFlags_e {
    QUERY_FOR_DEFAULT	= 0,		/*!< */
/*@-enummemuse@*/
    QUERY_MD5		= (1 << 0),	/*!< from --nomd5 */
    QUERY_SIZE		= (1 << 1),	/*!< from --nosize */
    QUERY_LINKTO	= (1 << 2),	/*!< from --nolink */
    QUERY_USER		= (1 << 3),	/*!< from --nouser) */
    QUERY_GROUP		= (1 << 4),	/*!< from --nogroup) */
    QUERY_MTIME		= (1 << 5),	/*!< from --nomtime) */
    QUERY_MODE		= (1 << 6),	/*!< from --nomode) */
    QUERY_RDEV		= (1 << 7),	/*!< from --nodev */
	/* bits 8-15 unused, reserved for rpmVerifyAttrs */
    QUERY_FILES		= (1 << 16),	/*!< verify: from --nofiles */
    QUERY_DEPS		= (1 << 17),	/*!< verify: from --nodeps */
    QUERY_SCRIPT	= (1 << 18),	/*!< verify: from --noscripts */
    QUERY_DIGEST	= (1 << 19),	/*!< verify: from --nodigest */
/*@=enummemuse@*/
	/* bits 20-22 unused */
    QUERY_FOR_LIST	= (1 << 23),	/*!< query:  from --list */
    QUERY_FOR_STATE	= (1 << 24),	/*!< query:  from --state */
    QUERY_FOR_DOCS	= (1 << 25),	/*!< query:  from --docfiles */
    QUERY_FOR_CONFIG	= (1 << 26),	/*!< query:  from --configfiles */
    QUERY_FOR_DUMPFILES	= (1 << 27),	/*!< query:  from --dump */
} rpmQueryFlags;

/** \ingroup rpmcli
 * Bit(s) to control rpmVerify() operation, stored in qva_flags.
 * @todo Merge rpmQueryFlags, rpmVerifyFlags, and rpmVerifyAttrs values?.
 */
typedef enum rpmVerifyFlags_e {
/*@-enummemuse@*/
    VERIFY_DEFAULT	= 0,		/*!< */
/*@=enummemuse@*/
    VERIFY_MD5		= (1 << 0),	/*!< from --nomd5 */
    VERIFY_SIZE		= (1 << 1),	/*!< from --nosize */
    VERIFY_LINKTO	= (1 << 2),	/*!< from --nolinkto */
    VERIFY_USER		= (1 << 3),	/*!< from --nouser */
    VERIFY_GROUP	= (1 << 4),	/*!< from --nogroup */
    VERIFY_MTIME	= (1 << 5),	/*!< from --nomtime */
    VERIFY_MODE		= (1 << 6),	/*!< from --nomode */
    VERIFY_RDEV		= (1 << 7),	/*!< from --nodev */
	/* bits 8-15 unused, reserved for rpmVerifyAttrs */
    VERIFY_FILES	= (1 << 16),	/*!< verify: from --nofiles */
    VERIFY_DEPS		= (1 << 17),	/*!< verify: from --nodeps */
    VERIFY_SCRIPT	= (1 << 18),	/*!< verify: from --noscripts */
    VERIFY_DIGEST	= (1 << 19),	/*!< verify: from --nodigest */
	/* bits 20-22 unused */
/*@-enummemuse@*/
    VERIFY_FOR_LIST	= (1 << 23),	/*!< query:  from --list */
    VERIFY_FOR_STATE	= (1 << 24),	/*!< query:  from --state */
    VERIFY_FOR_DOCS	= (1 << 25),	/*!< query:  from --docfiles */
    VERIFY_FOR_CONFIG	= (1 << 26),	/*!< query:  from --configfiles */
    VERIFY_FOR_DUMPFILES= (1 << 27),	/*!< query:  from --dump */
/*@=enummemuse@*/
	/* bits 28-30 used in rpmVerifyAttrs */
} rpmVerifyFlags;

#define	VERIFY_ATTRS	\
  ( VERIFY_MD5 | VERIFY_SIZE | VERIFY_LINKTO | VERIFY_USER | VERIFY_GROUP | \
    VERIFY_MTIME | VERIFY_MODE | VERIFY_RDEV )
#define	VERIFY_ALL	\
  ( VERIFY_ATTRS | VERIFY_FILES | VERIFY_DEPS | VERIFY_SCRIPT | VERIFY_DIGEST )

/** \ingroup rpmcli
 * @param qva		parsed query/verify options
 * @param ts		transaction set
 * @param db		rpm database
 * @param h		header to use for query/verify
 */
typedef	int (*QVF_t) (QVA_t qva, rpmTransactionSet ts, Header h)
	/*@globals fileSystem@*/
	/*@modifies qva, ts, fileSystem @*/;

/** \ingroup rpmcli
 * Display query/verify information for each header in iterator.
 *
 * This routine uses:
 *	- qva->qva_mi		rpm database iterator
 *	- qva->qva_showPackage	query/verify display routine
 *
 * @param qva		parsed query/verify options
 * @param ts		transaction set
 * @return		result of last non-zero showPackage() return
 */
int showMatches(QVA_t qva, rpmTransactionSet ts)
	/*@globals fileSystem@*/
	/*@modifies qva, fileSystem @*/;

/** \ingroup rpmcli
 * Display list of tags that can be used in --queryformat.
 * @param fp	file handle to use for display
 */
void rpmDisplayQueryTags(FILE * fp)
	/*@globals fileSystem@*/
	/*@modifies *fp, fileSystem @*/;

/** \ingroup rpmcli
 * Common query/verify source interface, called once for each CLI arg.
 *
 * This routine uses:
 *	- qva->qva_mi		rpm database iterator
 *	- qva->qva_showPackage	query/verify display routine
 *
 * @param qva		parsed query/verify options
 * @param ts		transaction set
 * @param arg		name of source to query/verify
 * @return		showPackage() result, 1 if rpmdbInitIterator() is NULL
 */
int rpmQueryVerify(QVA_t qva, rpmTransactionSet ts, const char * arg)
	/*@globals rpmGlobalMacroContext,
		fileSystem, internalState @*/
	/*@modifies qva, ts, rpmGlobalMacroContext,
		fileSystem, internalState @*/;

/** \ingroup rpmcli
 * Display results of package query.
 * @todo Devise a meaningful return code.
 * @param qva		parsed query/verify options
 * @param ts		transaction set
 * @param h		header to use for query
 * @return		0 always
 */
int showQueryPackage(QVA_t qva, rpmTransactionSet ts, Header h)
	/*@modifies ts @*/;

/** \ingroup rpmcli
 * Display package information.
 * @todo hack: RPMQV_ALL can pass char ** arglist = NULL, not char * arg. Union?
 * @param qva		parsed query/verify options
 * @param argv		query argument(s) (or NULL)
 * @return		0 on success, else no. of failures
 */
int rpmcliQuery(QVA_t qva, /*@null@*/ const char ** argv)
	/*@globals rpmGlobalMacroContext,
		fileSystem, internalState @*/
	/*@modifies qva, rpmGlobalMacroContext,
		fileSystem, internalState @*/;

/** \ingroup rpmcli
 * Display results of package verify.
 * @param qva		parsed query/verify options
 * @param ts		transaction set
 * @param h		header to use for verify
 * @return		result of last non-zero verify return
 */
int showVerifyPackage(QVA_t qva, rpmTransactionSet ts, Header h)
	/*@globals rpmGlobalMacroContext,
		fileSystem, internalState @*/
	/*@modifies ts, h, rpmGlobalMacroContext,
		fileSystem, internalState @*/;

/**
 * Check original header digest.
 * @todo Make digest check part of rpmdb iterator.
 * @todo Wire transaction set here, python bindings prevent.
 * @param h		header
 * @return		0 on success (or unavailable), 1 on digest mismatch
 */
int rpmVerifyDigest(Header h)
	/*@modifies nothing @*/;

/** \ingroup rpmcli
 * Verify package install.
 * @todo hack: RPMQV_ALL can pass char ** arglist = NULL, not char * arg. Union?
 * @param qva		parsed query/verify options
 * @param argv		verify argument(s) (or NULL)
 * @return		0 on success, else no. of failures
 */
int rpmcliVerify(QVA_t qva, /*@null@*/ const char ** argv)
	/*@globals rpmGlobalMacroContext,
		fileSystem, internalState @*/
	/*@modifies qva, rpmGlobalMacroContext,
		fileSystem, internalState @*/;

/** \ingroup rpmcli
 * Describe query/verify command line request.
 */
struct rpmQVArguments_s {
    rpmQVSources qva_source;	/*!< Identify CLI arg type. */
    int 	qva_sourceCount;/*!< Exclusive option check (>1 is error). */
    rpmQueryFlags qva_flags;	/*!< Bit(s) to control operation. */
    rpmfileAttrs qva_fflags;	/*!< Bit(s) to filter on attribute. */
/*@only@*/ /*@null@*/
    rpmdbMatchIterator qva_mi;	/*!< Match iterator on selected headers. */
/*@null@*/
    QVF_t qva_showPackage;	/*!< Function to display iterator matches. */
/*@unused@*/ int qva_verbose;	/*!< (unused) */
/*@only@*/ /*@null@*/
    const char * qva_queryFormat;/*!< Format for headerSprintf(). */
/*@observer@*/ /*@null@*/
    const char * qva_prefix;	/*!< Path to top of install tree. */
    char	qva_mode;	/*!< 'q' is query, 'v' is verify mode. */
    char	qva_char;	/*!< (unused) always ' ' */
};

/** \ingroup rpmcli
 */
/*@unchecked@*/
extern struct rpmQVArguments_s rpmQVArgs;

/** \ingroup rpmcli
 */
/*@unchecked@*/
extern struct poptOption rpmQVSourcePoptTable[];

/** \ingroup rpmcli
 */
/*@unchecked@*/
extern int specedit;

/** \ingroup rpmcli
 */
/*@unchecked@*/
extern struct poptOption rpmQueryPoptTable[];

/** \ingroup rpmcli
 */
/*@unchecked@*/
extern struct poptOption rpmVerifyPoptTable[];

/*@}*/
/* ==================================================================== */
/** \name RPMEIU */
/*@{*/
/* --- install/upgrade/erase modes */

/*@unchecked@*/
extern int packagesTotal;

/** \ingroup rpmcli
 * The rpm CLI generic transaction callback.
 * @deprecated Transaction callback arguments need to change, so don't rely on
 * this routine in the rpmcli API.
 *
 * @param arg		per-callback private data (e.g. an rpm header)
 * @param what		callback identifier
 * @param amount	per-callback progress info
 * @param total		per-callback progress info
 * @param pkgkey	opaque header key (e.g. file name or PyObject)
 * @param data		private data (e.g. rpmInstallInterfaceFlags)
 * @return		per-callback data (e.g. an opened FD_t)
 */
/*@null@*/ void * rpmShowProgress(/*@null@*/ const void * arg,
		const rpmCallbackType what,
		const unsigned long amount,
		const unsigned long total,
		/*@null@*/ const void * pkgKey,
		/*@null@*/ void * data)
	/*@globals fileSystem, internalState @*/
	/*@modifies fileSystem, internalState @*/;

/** \ingroup rpmcli
 * Install/upgrade/freshen binary rpm package.
 * @param rootdir	path to top of install tree
 * @param fileArgv	array of package file names (NULL terminated)
 * @param transFlags	bits to control rpmRunTransactions()
 * @param interfaceFlags bits to control rpmInstall()
 * @param probFilter 	bits to filter problem types
 * @param relocations	package file relocations
 * @return		0 on success
 */
int rpmInstall(/*@null@*/ const char * rootdir,
		/*@null@*/ const char ** fileArgv,
		rpmtransFlags transFlags, 
		rpmInstallInterfaceFlags interfaceFlags,
		rpmprobFilterFlags probFilter,
		/*@null@*/ rpmRelocation * relocations)
	/*@globals packagesTotal, rpmGlobalMacroContext,
		fileSystem, internalState@*/
	/*@modifies *relocations, packagesTotal, rpmGlobalMacroContext,
		fileSystem, internalState @*/;

/** \ingroup rpmcli
 * Install source rpm package.
 * @param rootdir	path to top of install tree
 * @param arg		source rpm file name
 * @retval specFile	address of (installed) spec file name
 * @retval cookie
 * @return		0 on success
 */
int rpmInstallSource(const char * rootdir, const char * arg,
		/*@null@*/ /*@out@*/ const char ** specFile,
		/*@null@*/ /*@out@*/ const char ** cookie)
	/*@globals rpmGlobalMacroContext,
		fileSystem, internalState@*/
	/*@modifies *specFile, *cookie, rpmGlobalMacroContext,
		fileSystem, internalState @*/;

/** \ingroup rpmcli
 * Erase binary rpm package.
 * @param rootdir	path to top of install tree
 * @param argv		array of package file names (NULL terminated)
 * @param transFlags	bits to control rpmRunTransactions()
 * @param interfaceFlags bits to control rpmInstall()
 * @return		0 on success
 */
int rpmErase(/*@null@*/ const char * rootdir, /*@null@*/ const char ** argv,
		rpmtransFlags transFlags, 
		rpmEraseInterfaceFlags interfaceFlags)
	/*@globals rpmGlobalMacroContext,
		fileSystem, internalState @*/
	/*@modifies rpmGlobalMacroContext,
		fileSystem, internalState @*/;

/** \ingroup rpmcli
 * Describe database command line requests.
 */
struct rpmInstallArguments_s {
    rpmtransFlags transFlags;
    rpmprobFilterFlags probFilter;
    rpmInstallInterfaceFlags installInterfaceFlags;
    rpmEraseInterfaceFlags eraseInterfaceFlags;
/*@only@*/ /*@null@*/ rpmRelocation * relocations;
    int numRelocations;
    int noDeps;
    int incldocs;
/*@null@*/ const char * prefix;
/*@observer@*/ /*@null@*/ const char * rootdir;
    int_32 rbtid;		/*!< from --rollback */
};

/**
 * A rollback transaction id element.
 */
/*@-fielduse@*/
typedef /*@abstract@*/ struct IDT_s {
    unsigned int instance;	/*!< installed package transaction id. */
/*@owned@*/ /*@null@*/ const char * key; /*! removed package file name. */
    Header h;			/*!< removed package header. */
    union {
	int_32 i32;		/*!< install/remove transaction id */
    } val;
} * IDT;
/*@=fielduse@*/

/**
 * A rollback transaction id index.
 */
typedef /*@abstract@*/ struct IDTindex_s {
    int delta;			/*!< no. elements to realloc as a chunk. */
    int size;			/*!< size of id index element. */
    int alloced;		/*!< current number of elements allocated. */
    int nidt;			/*!< current number of elements initialized. */
/*@only@*/ /*@null@*/ IDT idt;	/*!< id index elements. */
} * IDTX;

/**
 * Destroy id index.
 * @param idtx		id index
 * @return		NULL always
 */
/*@null@*/ IDTX IDTXfree(/*@only@*/ /*@null@*/ IDTX idtx)
	/*@modifies idtx @*/;

/**
 * Create id index.
 * @return		new id index
 */
/*@only@*/ IDTX IDTXnew(void)
	/*@*/;

/**
 * Insure that index has room for "need" elements.
 * @param idtx		id index
 * @param need		additional no. of elements needed
 * @return 		id index (with room for "need" elements)
 */
/*@only@*/ /*@null@*/ IDTX IDTXgrow(/*@only@*/ /*@null@*/ IDTX idtx, int need)
	/*@modifies idtx @*/;

/**
 * Sort tag (instance,value) pairs.
 * @param idtx		id index
 * @return 		id index
 */
/*@only@*/ /*@null@*/ IDTX IDTXsort(/*@only@*/ /*@null@*/ IDTX idtx)
	/*@modifies idtx @*/;

/**
 * Load tag (instance,value) pairs from rpm databse, and return sorted id index.
 * @param db		rpm database
 * @param tag		rpm tag
 * @return 		id index
 */
/*@only@*/ /*@null@*/ IDTX IDTXload(rpmdb db, rpmTag tag)
	/*@globals fileSystem@*/
	/*@modifies db, fileSystem @*/;

/**
 * Load tag (instance,value) pairs from packages, and return sorted id index.
 * @param db		glob expression
 * @param tag		rpm tag
 * @return 		id index
 */
/*@only@*/ /*@null@*/ IDTX IDTXglob(const char * globstr, rpmTag tag)
	/*@globals fileSystem@*/
	/*@modifies fileSystem @*/;


/** \ingroup rpmcli
 * Rollback transactions, erasing new, reinstalling old, package(s).
 * @return		0 on success
 */
int rpmRollback(struct rpmInstallArguments_s * ia,
		/*@null@*/ const char ** argv)
	/*@globals rpmGlobalMacroContext,
		fileSystem@*/
	/*@modifies rpmGlobalMacroContext,
		fileSystem @*/;

/** \ingroup rpmcli
 */
/*@unchecked@*/
extern struct rpmInstallArguments_s rpmIArgs;

/** \ingroup rpmcli
 */
/*@unchecked@*/
extern struct poptOption rpmInstallPoptTable[];

/*@}*/
/* ==================================================================== */
/** \name RPMDB */
/*@{*/
/* --- database modes */

/** \ingroup rpmcli
 * Describe database command line requests.
 */
struct rpmDatabaseArguments_s {
    int init;			/*!< from --initdb */
    int rebuild;		/*!< from --rebuilddb */
    int verify;			/*!< from --verifydb */
};

/** \ingroup rpmcli
 */
/*@unchecked@*/
extern struct rpmDatabaseArguments_s rpmDBArgs;

/** \ingroup rpmcli
 */
/*@unchecked@*/
extern struct poptOption rpmDatabasePoptTable[];

/*@}*/
/* ==================================================================== */
/** \name RPMK */
/*@{*/

/** \ingroup rpmcli
 * Bit(s) to control rpmCheckSig() operation.
 */
typedef enum rpmCheckSigFlags_e {
/*@-enummemuse@*/
    CHECKSIG_NONE	= 0,	/*!< Don't check any signatures. */
/*@=enummemuse@*/
    CHECKSIG_PGP	= (1 << 0),	/*!< if not --nopgp */
    CHECKSIG_MD5	= (1 << 1),	/*!< if not --nomd5 */
    CHECKSIG_GPG	= (1 << 2)	/*!< if not --nogpg */
} rpmCheckSigFlags;
#define	CHECKSIG_ALL	(CHECKSIG_PGP|CHECKSIG_MD5|CHECKSIG_GPG)

/** \ingroup rpmcli
 * Bit(s) to control rpmReSign() operation.
 */
typedef enum rpmSignFlags_e {
    RPMSIGN_NONE		= 0,
    RPMSIGN_CHK_SIGNATURE	= 1,	/*!< from --checksig */
    RPMSIGN_NEW_SIGNATURE	= 2,	/*!< from --resign */
    RPMSIGN_ADD_SIGNATURE	= 3,	/*!< from --addsign */
    RPMSIGN_IMPORT_PUBKEY	= 4,	/*!< from --import */
} rpmSignFlags;

/** \ingroup rpmcli
 * Describe signature command line request.
 */
struct rpmSignArguments_s {
    rpmSignFlags addSign;	/*!< from --checksig/--resign/--addsign */
    rpmCheckSigFlags checksigFlags;	/*!< bits to control --checksig */
    int sign;			/*!< Is a passphrase needed? */
/*@observer@*/
    const char * passPhrase;	/*!< Pass phrase. */
};

/** \ingroup rpmcli
 */
/*@unchecked@*/
extern struct rpmSignArguments_s rpmKArgs;

/** \ingroup rpmcli
 */
/*@unchecked@*/
extern struct poptOption rpmSignPoptTable[];

/** \ingroup rpmcli
 * Create/Modify/Check elements from signature header.
 * @param ka		mode flags and parameters
 * @param argv		array of package file names (NULL terminated)
 * @return		0 on success
 */
int rpmcliSign(struct rpmSignArguments_s * ka, /*@null@*/ const char ** argv)
	/*@globals rpmGlobalMacroContext,
		fileSystem, internalState @*/
	/*@modifies rpmGlobalMacroContext,
		fileSystem, internalState @*/;

/*@}*/

#ifdef __cplusplus
}
#endif

#endif	/* H_RPMCLI */
