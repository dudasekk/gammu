/* drivers for sql service
 *
 * MySQL (C) 2004 by Marcin Wiacek 
 * PostgreSQL (C) 2006 by Andrea Riciputi
 * DBI (C) 2009 by Michal Čihař 
 *
 */

#ifdef WIN32
#  include <winsock2.h>
#endif

#ifdef HAVE_MYSQL_MYSQL_H
#include <mysql.h>
#include <mysqld_error.h>
#endif

#ifdef HAVE_POSTGRESQL_LIBPQ_FE_H
#  include <libpq-fe.h>
#endif

#ifdef LIBDBI_FOUND
#include <dbi/dbi.h>
#endif

#ifdef HAVE_SHM
#include <sys/types.h>
#endif


typedef union {
#ifdef LIBDBI_FOUND
	dbi_result dbi;
#endif
#ifdef HAVE_MYSQL_MYSQL_H
	struct __mysql {
		MYSQL_RES *res;
		MYSQL_ROW row;
		MYSQL * con;	
	} my;
#endif
#ifdef HAVE_POSTGRESQL_LIBPQ_FE_H
	struct __pg {
		PGresult *res;
		int iter;
	} pg;
#endif
} SQL_result;

typedef union __sql_conn {
#ifdef LIBDBI_FOUND
	dbi_conn dbi; /* dbi driver */
#endif
#ifdef HAVE_MYSQL_MYSQL_H
	MYSQL my; /* mysql driver */
#endif
#ifdef HAVE_POSTGRESQL_LIBPQ_FE_H
	PGconn *pg; /* pgsql driver */
#endif
} SQL_conn;

typedef enum {
	SQL_OK,
	SQL_TIMEOUT,
	SQL_FAIL,
	SQL_LOCKED
} SQL_Error;

typedef enum {
	SQL_TYPE_NONE,
	SQL_TYPE_INT,
	SQL_TYPE_STRING
} SQL_Type;

typedef union {
	const char *s;
	int i;
} SQL_Val;

typedef struct {
	SQL_Type type;
	SQL_Val v;
} SQL_Var;

/* incomplete declaration - cyclic occurence of GSM_SMSDConfig */
struct GSM_SMSDConfig;


struct GSM_SMSDdbobj {
	const char *DriverName;
	SQL_Error (* Connect)(GSM_SMSDConfig *);
	SQL_Error (* Query)(GSM_SMSDConfig *, const char *, SQL_result *res);
	void (* Free)(SQL_conn *); /* = close() */
	void (* FreeResult)(SQL_result);
	int (* NextRow)(SQL_result *);
	unsigned long long (* SeqID)(SQL_conn *conn, const char *);
	unsigned long (* AffectedRows)(SQL_result);
	unsigned long (* NumRows)(SQL_result);
	const char * (* GetString)(SQL_result, unsigned int);
	long long (* GetNumber)(SQL_result, unsigned int);
	time_t (* GetDate)(SQL_result, unsigned int);
	gboolean (* GetBool)(SQL_result, unsigned int);
	char * (* QuoteString)(SQL_conn *conn, const char *);
	const char *error;
	SQL_conn conn;
};

#ifdef HAVE_POSTGRESQL_LIBPQ_FE_H
extern struct GSM_SMSDdbobj SMSDPgSQL;
#endif

#ifdef HAVE_MYSQL_MYSQL_H
extern struct GSM_SMSDdbobj SMSDMySQL;
#endif

#ifdef LIBDBI_FOUND
extern struct GSM_SMSDdbobj SMSDDBI;
#endif

/* How should editor hadle tabs in this file? Add editor commands here.
 * vim: noexpandtab sw=8 ts=8 sts=8:
 */
