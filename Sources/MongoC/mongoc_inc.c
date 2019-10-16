/*
 A set of detailed examples on how to use mongoc is located here: https://github.com/mongodb/mongo-c-driver/tree/master/src/libmongoc/examples
 Additional example on parsing and working with BSON are located here: https://github.com/mongodb/mongo-c-driver/tree/master/src/libbson/examples
*/

#include "mongoc_inc.h"

bson_error_t error;

mongoc_client_t *client = NULL;
mongoc_database_t *database = NULL;
mongoc_collection_t *collection = NULL;
mongoc_client_session_t *session = NULL;
mongoc_read_concern_t *read_concern = NULL;
mongoc_transaction_opt_t *default_txn_opts = NULL, *txn_opts = NULL;
mongoc_session_opt_t *session_opts = NULL;
bson_t *insert_opts = NULL;

mongoc_uri_t *uri;
const char *uri_string = "mongodb://localhost:27017/?appname=ctserver";

bool createDatabaseAndSession() {
    puts("createDatabase");
    
    mongoc_init();
    printf("A mongoc version used is: %s\n", mongoc_get_version());
    
    uri = mongoc_uri_new_with_error(uri_string, &error);
    
    client = mongoc_client_new_from_uri(uri);
    if (!client)
        return EXIT_FAILURE;
    
    mongoc_client_set_error_api(client, 2);
    database = mongoc_client_get_database(client, "TESTAR");
    
    if (!(collection = mongoc_database_create_collection(database, "ctcollection", NULL, &error))) {
        /* code 48 is collection exists, not an error in our case */
        if (error.code == 48) // The error.message contains the error message
            collection = mongoc_database_get_collection(database, "ctcollection");
        else
            return EXIT_FAILURE;
    }
    
    // Initiating a transaction
    default_txn_opts = mongoc_transaction_opts_new();
    read_concern = mongoc_read_concern_new();
    mongoc_read_concern_set_level(read_concern, "snapshot");
    mongoc_transaction_opts_set_read_concern(default_txn_opts, read_concern);
    
    session_opts = mongoc_session_opts_new();
    mongoc_session_opts_set_default_transaction_opts (session_opts, default_txn_opts);
    
    if (!(session = mongoc_client_start_session(client, session_opts, &error)))
        return EXIT_FAILURE;
    
    insert_opts = bson_new();
    if (!mongoc_client_session_append(session, insert_opts, &error))
        return EXIT_FAILURE;
    
    return EXIT_SUCCESS;
}

// In the current demo the amount of retries is not defined, it will be incapsulated into a layer with configurable retry parameters
bool writeDataToCollection(MongoObjData* mongoObjData1) {
retry:
    
    puts("writeDataToCollection");
    
    // Initiating a transaction
    if (!mongoc_client_session_start_transaction(session, txn_opts, &error))
        return EXIT_FAILURE;
    
    bson_t reply = BSON_INITIALIZER;
    bson_destroy(&reply);
    bson_t *doc = BCON_NEW("_id", BCON_INT32(92));
    /*
     Please note that starting in MongoDB 4.0, a MongoDB replica set is required to use transactions:
        https://stackoverflow.com/questions/51238986/pymongo-transaction-errortransaction-numbers-are-only-allowed-on-a-replica-set
        https://sudonull.com/posts/5771-How-to-already-feel-transactions-in-MongoDB (a docker based solution to that condition)
     */
    bool result = mongoc_collection_insert_one(collection, doc, insert_opts, &reply, &error);
    bson_destroy(doc);
    
    if (!result) {
        mongoc_client_session_abort_transaction(session, NULL);
        
        if (mongoc_error_has_label(&reply, "TransientTransactionError")) // TransientTransactionError means a retriable transaction error
            goto retry;
        else
            return EXIT_FAILURE;
    }
    
    char *reply_json = bson_as_json(&reply, NULL);
    puts(reply_json);
    
    // We need to use explicit destruction of bson/mongoc based object here and further
    bson_free(reply_json);
    bson_destroy(&reply);

retry_commit:
    // Commiting a transaction
    if (mongoc_client_session_commit_transaction (session, &reply, &error))
        return EXIT_SUCCESS;
        
    if (mongoc_error_has_label (&reply, "TransientTransactionError"))
        goto retry; // The one requiring to retry the whole transaction
    else if (mongoc_error_has_label (&reply, "UnknownTransactionCommitResult"))
        goto retry_commit; // The one requiring to retry the commit stage
    else // Unrecoverable error
        return EXIT_FAILURE;
    
    return EXIT_SUCCESS;
}

bool writeDataToCollectionNoTransaction(MongoObjData* mongoObjData2) {
    puts("writeDataToCollectionNoTransaction");
    
    bson_oid_t oid;
    bson_t * doc = bson_new();
    bson_oid_init (&oid, NULL);
    BSON_APPEND_OID (doc, "_id", &oid);
    BSON_APPEND_UTF8 (doc, "hello", "world");
    
    /*
     An example of a complex BSON document creation:
 
        doc = BCON_NEW ("foo",
                            "{",
                                "int", BCON_INT32 (1),
                                "array",
                                    "[",
                                        BCON_INT32 (100),
                                        "{",
                                            "sub", BCON_UTF8 ("value"),
                                        "}",
                                    "]",
                            "}");
     
         Creates the following document:
     
         { "foo" : { "int" : 1, "array" : [ 100, { "sub" : "value" } ] } }
     */
    
    if (!mongoc_collection_insert_one (
                                       collection, doc, NULL, NULL, &error)) {
        fprintf (stderr, "%s\n", error.message);
    }
    
    bson_destroy (doc);
}

bool closeDatabase() {
    puts("closeDatabase");
    
    bson_destroy (insert_opts);
    mongoc_read_concern_destroy (read_concern);
    mongoc_transaction_opts_destroy (txn_opts);
    mongoc_client_session_destroy (session);
    mongoc_collection_destroy (collection);
    mongoc_database_destroy (database);
    mongoc_client_destroy (client);
    mongoc_uri_destroy (uri);
    
    mongoc_cleanup ();
    
    return EXIT_SUCCESS;
}
