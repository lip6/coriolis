#!/bin/env python3
import docutils.utils
import subprocess
import tempfile

from docutils.parsers.rst import Parser
from docutils.frontend import get_default_settings
from docutils.nodes import NodeVisitor


def parse_rst(text: str) -> docutils.nodes.document:
    parser = Parser()
    settings = get_default_settings(Parser)
    document = docutils.utils.new_document('<rst-doc>', settings=settings)
    parser.parse(text, document)
    return document


class MyVisitor(NodeVisitor):
    def __init__(self, *args, **kwargs):
        super(MyVisitor, self).__init__(*args, **kwargs)
        self.commands = {}
        self.systems = ['mac', 'linux']
        for sys in self.systems:
            self.commands[sys] = []

    def visit_literal_block(self, node: docutils.nodes.reference) -> None:
        """Called for "reference" nodes."""
        for sys in self.systems:
            if sys in node['classes'] or 'all' in node['classes']:
                self.commands[sys].append(''.join(map(lambda n: n.astext(), node.children)))

    def unknown_visit(self, node: docutils.nodes.Node) -> None:
        """Called for all other node types."""
        pass


commands = ""
with open("README.rst") as f:
    s = f.read()
    doc = parse_rst(s)
    visitor = MyVisitor(doc)
    doc.walk(visitor)
    for command in visitor.commands['mac']:
        commands += command + '\n'

print(f'Running:\n{commands}')
with tempfile.TemporaryDirectory() as d:
    process = subprocess.Popen('/bin/zsh', stdin=subprocess.PIPE,
                               stdout=subprocess.PIPE, cwd=d)
    out, err = process.communicate(commands.encode('utf-8'))
    print(out.decode('utf-8'))
