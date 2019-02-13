#include <string.h>
#include <stdlib.h>
#include <libxml/HTMLparser.h>

#include "util/at_xmlparser.h"

static at_xmlparser_t *create_xmlparser(xmlDocPtr doc) {
    at_xmlparser_t *parser = (at_xmlparser_t *)malloc(sizeof(at_xmlparser_t));
    parser->doc = doc;
    parser->xpath_ctx = NULL;
    parser->handlers = vector_new(DEFAULT_NODE_HANDLER_CAP, sizeof(at_nodehandler_t), NULL);
    return parser;
}

/**
* create xmlparser to parse xml file <xmlFile>
**/
at_xmlparser_t *xmlparser_create_for_file(const char *xml_file, const char *encoding) {
    xmlDocPtr doc = xmlReadFile(xml_file, encoding, XML_PARSE_NOBLANKS | XML_PARSE_COMPACT);
    if (doc) {
        return create_xmlparser(doc);
    }
    return NULL;
}

at_xmlparser_t *htmlparser_create_for_file(const char *html_file, const char *encoding) {
    xmlDocPtr doc = htmlReadFile(html_file, encoding, XML_PARSE_NOBLANKS | XML_PARSE_COMPACT);
    if (doc) {
        return create_xmlparser(doc);
    }
    return NULL;
}

/**
* create xmlparser to parse xml <xml>
**/
at_xmlparser_t *xmlparser_creata_for_str(const char *xml, const char *encoding) {
    xmlDocPtr doc = xmlReadMemory(xml, strlen(xml), NULL, encoding, XML_PARSE_NOBLANKS | XML_PARSE_COMPACT);
    if (doc) {
        return create_xmlparser(doc);
    }
    return NULL;
}

at_xmlparser_t *htmlparser_create_for_str(const char *html, const char *encoding) {
    xmlDocPtr doc = htmlReadMemory(html, strlen(html), NULL, encoding, XML_PARSE_NOBLANKS | XML_PARSE_COMPACT);
    if (doc) {
        return create_xmlparser(doc);
    }
    return NULL;
}

static at_nodehandler_t *get_handler(at_xmlparser_t *parser, const char *node) {
    int i = 0, handler_num = vector_size(parser->handlers);
    for (i = 0; i < handler_num; i++) {
        at_nodehandler_t *target = (at_nodehandler_t *)vector_get(parser->handlers, i);
        if (strcmp(string_cstr(target->node), node) == 0) {
            return target;
        }
    }
    return NULL;
}

/**
* register callback to process node specified by <node>
* @node node path in xml, for example, xml is <root><a><a1></a1></a><b></b></root>, and you want to 
*       register callback to process al node, the node should liek /root/a/a1/
* @func callback
* @param_in input parameter to <func>, if no, set it be NULL
* #param_out output parameter to <func>, if no, set it be NULL
**/
void xmlparser_register_node_callback(at_xmlparser_t *parser, char *node, node_process_func func, void *param_in, 
    void *param_out) {
    at_nodehandler_t *handler = get_handler(parser, node);
    if (!handler) {
        handler = (at_nodehandler_t *)vector_prefetch(parser->handlers);
        handler->node = string_new(node);
        handler->callbacks = vector_new(DEFAULT_NODE_CALLBACK_CAP, sizeof(at_nodecallback_t), NULL);
    }
    at_nodecallback_t *callback = (at_nodecallback_t *)vector_prefetch(handler->callbacks);
    callback->func = func;
    callback->param_in = param_in;
    callback->param_out = param_out;
}

static void travel(at_xmlparser_t *parser, xmlNodePtr start_node, const char *cur_path) {
    xmlNodePtr cur_node = NULL;
    for (cur_node = start_node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            at_string_t *node_path = string_blank(100);
            string_append(node_path, cur_path, strlen(cur_path));
            string_append(node_path, cur_node->name, strlen(cur_node->name));
            at_nodehandler_t *handler = get_handler(parser, string_cstr(node_path));
            if (!handler) {
                handler = get_handler(parser, cur_node->name);
            }
            if (handler) {
                int i = 0, callback_num = vector_size(handler->callbacks);
                for (i = 0; i < callback_num; i++) {
                    at_nodecallback_t *callback = (at_nodecallback_t *)vector_get(handler->callbacks, i);
                    callback->func(cur_node, callback->param_in, callback->param_out);
                }
            } else {
                string_append(node_path, "/", 1);
                travel(parser, cur_node->children, string_cstr(node_path));
            }
            string_free(&node_path);
        }
    }
}

/**
* start to parse xml, active the callbacks registered before to process nodes
**/
void xmlparser_parse(at_xmlparser_t *parser) {
    xmlNodePtr xmlRoot = xmlDocGetRootElement(parser->doc);
    travel(parser, xmlRoot, "/"); 
}

int xmlparser_eval_xpath_express(at_xmlparser_t *parser, const char *xpath, nodeset_process_func func, void *param_in,
    void *param_out) {
    if (!parser->xpath_ctx) {
        parser->xpath_ctx = xmlXPathNewContext(parser->doc);
        if (!parser->xpath_ctx) {
            return -1;
        }
    }
    xmlXPathObjectPtr xpath_obj = xmlXPathEvalExpression(xpath, parser->xpath_ctx);
    if (!xpath_obj) {
        return -2;
    }
    func(xpath_obj->nodesetval, param_in, param_out);
    xmlXPathFreeObject(xpath_obj);
    return 0;
}

void xmlparser_destroy(at_xmlparser_t **parser_ptr) {
    at_xmlparser_t *parser = *parser_ptr;
    if (parser) {
        if (parser->xpath_ctx) {
            xmlXPathFreeContext(parser->xpath_ctx);
        }
        xmlFreeDoc(parser->doc);
        xmlCleanupParser();
        int i = 0, handler_num = vector_size(parser->handlers);
        for (i = 0; i < handler_num; i++) {
            at_nodehandler_t *handler = (at_nodehandler_t *)vector_get(parser->handlers, i);
            vector_free(&handler->callbacks);
        }
        vector_free(&parser->handlers);
        free(parser);
        *parser_ptr = NULL;
    }
}


