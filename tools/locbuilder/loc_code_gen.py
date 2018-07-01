#-*- coding: utf-8 -*-

from __future__ import with_statement

import sys
import gsi_sys
import os.path


#----- interface template -----------------------------------------------------

template_iface = """/*AUTOGENERATED CODE, DO NOT EDIT*/

#ifndef _EPOD_AUTOGENERATED_LOC_HEADER_
#define _EPOD_AUTOGENERATED_LOC_HEADER_

namespace loc
{

LOC_EXTERN_STRINGS

void init(const char *loc);

} // namespace loc

#endif // _EPOD_AUTOGENERATED_LOC_HEADER_
"""

template_EXTERN_STRING = "extern const char *STR_ID;\n"

#----- main implementation template -------------------------------------------

template_main_impl = """/*AUTOGENERATED CODE, DO NOT EDIT*/

LOC_CPP_INCLUDES

namespace loc
{

#define LOC_STR(name, loc) \\
LOC_STR_BY_INDEX

int get_loc_id(const char *loc)
{
LOC_INDEX_CHECKS
  return -1;
}

LOC_NULL_STRINGS

void init(const char *loc)
{
  int locId = get_loc_id(loc);
  if (locId == -1)
    return;

LOC_ASSIGN_STRINGS
}

} // namespace loc
"""

template_CPP_INCLUDE = "#include \"loc_LOC_ID.cpp\"\n"
template_STR_BY_INDEX = "  (loc == LOC_IND) ? loc_LOC_ID::name : \\\n"
template_STR_BY_INDEX_END = "  loc_LOC_ID::name;\n"
template_INDEX_CHECK = "  if (!strcmp(loc, \"LOC_ID\"))\n    return LOC_IND;\n"
template_NULL_STRING = "const char *STR_ID = 0;\n"
template_ASSIGN_STRING = "  STR_ID = LOC_STR(STR_ID, locId);\n"

#----- loc implementation template --------------------------------------------

template_loc_impl = """/*AUTOGENERATED CODE, DO NOT EDIT*/

namespace loc_LOC_ID
{

LOC_STRINGS
} // namespace loc_LOC_ID
"""

template_STRING = "const char *STR_ID = \"STR_TEXT\";\n"

#------------------------------------------------------------------------------

# comparing new-data and file-data
def cmp_file_data(data, file_name):
  if os.path.exists(file_name):
    with open(file_name, 'r') as file:
      if file.read() == data:
        return True

  return False

#------------------------------------------------------------------------------

def make_loc_imple_file(loc_info, output_src_dir, loc_ind):  
  (locs, str_lines) = loc_info
  loc = locs[loc_ind]
  
  output_loc_cpp = os.path.join(output_src_dir, "loc_%s.cpp" % loc)
  
  lines = ""
  for key, vals in str_lines:
    lines += template_STRING.replace('STR_ID', key).replace('STR_TEXT', vals[loc_ind])
  
  data_cpp = template_loc_impl.replace('LOC_ID', loc).replace('LOC_STRINGS', lines)
  
  # cpp loc file
  with open(output_loc_cpp, 'w') as file_cpp:
    print >> file_cpp, data_cpp,
  

def gen_source_code(loc_info, output_src_dir):
  output_loc_h = os.path.join(output_src_dir, "loc.h")
  output_loc_cpp = os.path.join(output_src_dir, "loc.cpp")
  gsi_sys.create_path_to_file(output_loc_h)
  
  data_h = template_iface
  data_cpp = template_main_impl
  
  (locs, str_lines) = loc_info
  
  # locs enum
  cpp_includes = ""
  str_by_indexes = ""
  index_checks = ""
  for ind in xrange(len(locs)):
    make_loc_imple_file(loc_info, output_src_dir, ind)
    
    loc = locs[ind]
    cpp_includes += template_CPP_INCLUDE.replace('LOC_ID', loc)
    index_checks += template_INDEX_CHECK.replace('LOC_IND', str(ind)).replace('LOC_ID', loc)
    
    if (ind < len(locs) - 1):
      str_by_indexes += template_STR_BY_INDEX.replace('LOC_IND', str(ind)).replace('LOC_ID', loc)
    else:
      str_by_indexes += template_STR_BY_INDEX_END.replace('LOC_ID', loc)
  
  # strings enum
  extern_strings = ""
  null_strings = ""
  assign_strings = ""
  for key, vals in str_lines:
    extern_strings += template_EXTERN_STRING.replace('STR_ID', key)
    null_strings += template_NULL_STRING.replace('STR_ID', key)
    assign_strings += template_ASSIGN_STRING.replace('STR_ID', key)
  
  data_h = data_h.replace('LOC_EXTERN_STRINGS', extern_strings)
  
  data_cpp = data_cpp.replace('LOC_CPP_INCLUDES', cpp_includes)
  data_cpp = data_cpp.replace('LOC_STR_BY_INDEX', str_by_indexes)
  data_cpp = data_cpp.replace('LOC_INDEX_CHECKS', index_checks)
  data_cpp = data_cpp.replace('LOC_NULL_STRINGS', null_strings)
  data_cpp = data_cpp.replace('LOC_ASSIGN_STRINGS', assign_strings)
  
  # hpp file
  if not cmp_file_data(data_h, output_loc_h):
    print "!header was changed!"
    with open(output_loc_h, 'w') as file_h:
      print >> file_h, data_h,
  
  # cpp main file
  with open(output_loc_cpp, 'w') as file_cpp:
    print >> file_cpp, data_cpp,
  