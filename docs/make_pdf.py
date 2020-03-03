#!/usr/bin/env python3

# A script to render the docs to PDF using pandoc & LaTeX
# I tried to use a Makefile, but failed miserably

import os
import platform
import re

# Prepare OS specific stuff
is_win = platform.system() == 'Windows'
sep = ';' if is_win else ':'
excl = '\\_' if is_win else '/_'

# Get paths where markdown files are referencing images from (e.g. everywhere)
resource_paths = [ x[0] for x in os.walk('.') if not excl in x[0] ]

# Get docs in correct order by reading TOC
files = []
with open('_sidebar.md') as toc:
  for line in toc:
    if re.search(r'<!--', line):
      continue
    md = re.search(r'\((.*)\)', line)
    if md:
      files.append(md.group(1))
      # if not os.path.exists(md.group(1)):
      #   f = open(md.group(1), 'w+')
      #   f.write('> TODO\n')
      #   f.close()

# Remove non-existent docs
existing_files = [ f for f in files if os.path.exists(f) ]

# Get args for pandoc
res_path_arg = sep.join(resource_paths)
input_arg = ' '.join(existing_files)

# Fire it up (leaving out README.md as it's useless)
os.system(f'pandoc --toc -V documentclass=report --resource-path={res_path_arg} -o manual.pdf {input_arg}')