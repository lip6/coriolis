#!/bin/env python3
import docutils.utils

from docutils.parsers.rst import Parser
from docutils.frontend import OptionParser
from docutils.nodes import NodeVisitor


def parse_rst(text: str) -> docutils.nodes.document:
    parser = Parser()
    components = (Parser,)
    settings = OptionParser(components=components).get_default_values()
    document = docutils.utils.new_document('<rst-doc>', settings=settings)
    parser.parse(text, document)
    return document


class MyVisitor(NodeVisitor):

    def visit_literal_block(self, node: docutils.nodes.reference) -> None:
        """Called for "reference" nodes."""
#        print(f'\n{node}\n')
        for child in node:
            print(child.astext(), end='')
        print('\n')

    def unknown_visit(self, node: docutils.nodes.Node) -> None:
        """Called for all other node types."""
        pass


with open("README.rst") as f:
    s = f.read()
    doc = parse_rst(s)
    visitor = MyVisitor(doc)
    doc.walk(visitor)
