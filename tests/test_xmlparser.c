#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>

#include "util/at_xmlparser.h"

void process_interactor(xmlNodePtr node, void *param_in, void *param_out) {
    printf("%s\n", node->name);
}

int main(int argc, char *argv[]) {
    at_xmlparser_t *parser = xmlparser_create_for_file("/home/herengao/data/ppi/dipsimple.xml", "UTF-8");
    if (parser) {
        xmlparser_register_node_callback(parser, "/entrySet/entry/interactorList/interactor", 
            process_interactor, NULL, NULL);
        xmlparser_parse(parser);
        xmlparser_destroy(&parser);
    }
    return 0;
}


