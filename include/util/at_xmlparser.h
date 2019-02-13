#ifndef AT_XMLPARSER_H_
#define AT_XMLPARSER_H_

#include <string.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>

#include "util/at_string.h"
#include "util/at_vector.h"

#define DEFAULT_NODE_CALLBACK_CAP 1
#define NODE_CALLBACK_INCREASE_STEP 2
#define DEFAULT_NODE_HANDLER_CAP 5
#define NODE_HANDLER_INCREASE_STEP 5

typedef void (*node_process_func)(xmlNodePtr, void *, void *);
typedef void (*nodeset_process_func)(xmlNodeSetPtr, void *, void *);

typedef struct {
    node_process_func func;
    void *param_in;
    void *param_out;
} at_nodecallback_t;

typedef struct {
    at_string_t *node;
    at_vector_t *callbacks;
} at_nodehandler_t;

typedef struct {
    xmlDocPtr doc;
    xmlXPathContextPtr xpath_ctx;
    at_vector_t *handlers;
} at_xmlparser_t;

#ifdef __cplusplus
extern "C" {
#endif

at_xmlparser_t *xmlparser_create_for_file(const char *, const char *);

at_xmlparser_t *xmlparser_create_for_str(const char *, const char *);

at_xmlparser_t *htmlparser_create_for_file(const char*, const char *);

at_xmlparser_t *htmlparser_create_for_str(const char *, const char *);

void xmlparser_register_node_callback(at_xmlparser_t *, char *, node_process_func, void *, void *);

void xmlparser_parse(at_xmlparser_t *);

int xmlparser_eval_xpath_express(at_xmlparser_t *, const char *, nodeset_process_func, void *, void *);

void xmlparser_destroy(at_xmlparser_t **);

#ifdef __cplusplus
}
#endif

#endif /*AT_XMLPARSER_H_*/

