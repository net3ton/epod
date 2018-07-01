#-*- coding: utf-8 -*-

from __future__ import with_statement

import sys
import loc_code_gen


def check_csv_string(str):
  str = str.replace('""', '\\"')
  
  inds = str.find('"')
  if (inds != -1):
    inde = str.rfind('"')
    assert inds != inde, "error CSV string format!"
    str = str[inds+1:inde]
  
  str = str.strip()
  return str


def split_csv_lines(inline):
  lines = []
  
  start = 0
  numk = 0
  for i in range(len(inline)):
    if (inline[i] == '"'):
      numk += 1;
    if (inline[i] == ';' and (numk % 2) == 0):
      lines.append(check_csv_string(inline[start:i]))
      start = i + 1
      numk = 0
  
  lines.append(check_csv_string(inline[start:]))
  return lines


def proccess_cvs(csv_path, loc_first_col):
  locs = []
  lines = []
  
  with open(csv_path, 'r') as file:
    headerLine = True
    for line in file.read().splitlines():
      values = split_csv_lines(line)
      
      if (headerLine):
        headerLine = False
        ind = loc_first_col
        while (ind < len(values)):
          locs.append(values[ind].lower())
          ind += 1
        assert len(locs) != 0, "CSV doesn't contain any locs!"
        continue
      
      key = values[0]
      if (key == ""):
        continue
      if (key.upper() != key):
        continue
      
      vals = []
      ind = loc_first_col
      while (ind < len(values)):
        text = values[ind];
        if (text == ""):
          print ("key = '" + key + "' is empty.")
          text = "???"
        vals.append(text)
        ind += 1
      
      assert len(locs) == len(vals), "Wrong number vals for locs!"
      lines.append((key, vals))
  
  print "  localizaions: %d -> %s" % (len(locs), "".join("%s " % loc for loc in locs))
  print "  strings: %d" % len(lines)
  return (locs, lines)


if __name__ == '__main__':
  assert len(sys.argv) >=4, "usage: loc_builder.py csv_path, cvs_row_start, output_src_dir"
  csv_path, cvs_row_start, output_src_dir = sys.argv[1], sys.argv[2], sys.argv[3]
  
  print "processing loc.csv..."
  loc_info = proccess_cvs(csv_path, int(cvs_row_start))
  
  print "generating source code..."
  loc_code_gen.gen_source_code(loc_info, output_src_dir)
  
  print "ok."
