#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <libxml2/parser.h>

#include "util/at_xmlparser.h"

void process_table(xmlNodePtr node, void *param_in, void *param_out) {
    xmlNodePtr cur_node = NULL;
    for (cur_node = node; cur_node; cur_node = cur_node->next) {
        if (strcasecmp(cur_node->name, "a") == 0) {  
            xmlChar *href = xmlGetProp(cur_node, "href");
            if (strcasestr(href, "www.ebi.ac.uk/interpro/entry")) {
                printf("href: %s, content: %s\n", href, cur_node->children->content);
            }
            xmlFree(href);
        }
        process_table(cur_node->children, param_in, param_out);
    }
}

int main(int argc, char *argv[]) {
    at_xmlparser_t *parser = htmlparser_create_for_file("/home/herengao/workspace-cxx/test/DIP-445.html", "UTF-8");
    if (parser) {
        xmlparser_register_node_callback(parser, "table", process_table, NULL, NULL);
        xmlparser_parse(parser);
        xmlparser_destroy(&parser);
    }
}

