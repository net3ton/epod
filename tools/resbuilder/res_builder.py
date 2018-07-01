import sys
import res_xml_parse
import res_code_gen
import res_data

import os.path

# command line params:
# 1 - input_dir_res - input folder with resources and resource_info.xml scheme
# 2 - output_dir_res - output folder for resources
# 3 - output_dir_code - output folder for generated sources

# xml_config
# input_dir_res
# output_dir_res
# output_dir_source
# output_fnames

if __name__ == '__main__':
  assert len(sys.argv) >=3, "usage: res_builder.py input_dir_res output_dir_res output_dir_code"
  input_dir_res, output_dir_res, output_dir_code = sys.argv[1], sys.argv[2], sys.argv[3]
  
  print "parsing resource.xml..."
  xml_config = os.path.join(input_dir_res, "resource_info.xml")
  atlas_info, font_info, sound_info = res_xml_parse.parse(xml_config, input_dir_res)
  
  print "building atlases..."
  tiles = res_data.build_atlases(atlas_info, output_dir_res)
  
  print "building fonts..."
  fonts = res_data.build_fonts(font_info, output_dir_res)
  
  print "building sounds..."
  sounds = res_data.build_sounds(sound_info, output_dir_res)
  
  print "generating source code..."
  output_dir_source = output_dir_code
  output_fnames = "res_gen"
  res_code_gen.gen_source_code(tiles, fonts, sounds, output_dir_source, output_fnames)
  
  print "ok."
