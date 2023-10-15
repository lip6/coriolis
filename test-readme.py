#!/bin/env python3
import docutils.utils
import tempfile
import bashlex
import asyncio
import logging

from docutils.parsers.rst import Parser
from docutils.frontend import get_default_settings
from docutils.nodes import NodeVisitor

#logging.basicConfig(level=logging.DEBUG)

def parse_rst(text: str) -> docutils.nodes.document:
    parser = Parser()
    settings = get_default_settings(Parser)
    document = docutils.utils.new_document('<rst-doc>', settings=settings)
    #document.reporter.debug_flag = True
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
        logging.debug(f'node: {node.__dict__}, line: {node.line}')
        for sys in self.systems:
            if sys in node['classes'] or 'all' in node['classes']:
                self.commands[sys].append(node)

    def unknown_visit(self, node: docutils.nodes.Node) -> None:
        """Called for all other node types."""
        pass


commands = []
with open("README.rst") as f:
    s = f.read()
    doc = parse_rst(s)
    visitor = MyVisitor(doc)
    doc.walk(visitor)
    for node in visitor.commands['mac']:
        logging.debug(f"rawsource: {node.rawsource}, line: {node.line}")
        parts = bashlex.parse(node.rawsource)
        for n in parts:
            command = node.rawsource[n.pos[0]:n.pos[1]]
            commands.append((node.line,command))


import re

debug = False

async def run_commands(commands):
    stdout_barrier = re.compile("^============= RSTTESTER RETURN ([0-9]+) ============$")
    stderr_barrier  = re.compile("^============= RSTTESTER ERROR COMPLETE ============$")
    with tempfile.TemporaryDirectory() as d:
        process = await asyncio.create_subprocess_exec('/bin/bash',
                                stdin=asyncio.subprocess.PIPE,
                                stdout=asyncio.subprocess.PIPE,
                                stderr=asyncio.subprocess.PIPE,
                                cwd=d)

        def write(s):
            process.stdin.write(s.encode('utf-8'))
            logging.debug(f"wrote {s}")

        logging.info(f'Running:\n{commands}')
        for cmd in commands:
            logging.info(f"---- line: {cmd[0]}\n{cmd[1]}\n\n")
            write(cmd[1])
            write('\necho "============= RSTTESTER RETURN $? ============"\n')
            lines = []
            line = ""
            while stdout_barrier.match(line) is None:
                data = await process.stdout.readline()
                line = data.decode('utf-8').rstrip()
                logging.debug(f'> {line}')
                lines.append(line)

            logging.debug (stdout_barrier.match(line))
            exitcode = int(stdout_barrier.match(line).group(1))


            write('\necho "============= RSTTESTER ERROR COMPLETE ============" 1>&2\n')
            errlines = []
            line = ""
            while stderr_barrier.match(line) is None:
                data = await process.stderr.readline()
                line = data.decode('utf-8').rstrip()
                logging.debug(f'E {line}')
                errlines.append(line)

            if (exitcode > 0):
                logging.error(f"Command failed at line {cmd[0]}: \n{cmd[1]}\n")

            logging.info(f"---- returned {exitcode}")
            logging.info(f"---- stdout:\n{lines}")
            logging.info(f"---- stdout:\n{errlines}")

asyncio.run(run_commands(commands))

# with tempfile.TemporaryDirectory() as d:
#    out, err = process.communicate(commands.encode('utf-8'))
#    print(out.decode('utf-8'))
