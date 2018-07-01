from __future__ import with_statement

import os
import os.path
import gsi_sys
import gsi_pvr


def build_atlases(atlas_info, output_dir_res):
  texAllSize = 0
  res = []
  
  # verification
  #############################################################################
  tilesError = False
  
  langs = []
  langTiles = []
  tilesUnique = []
  for atlas in atlas_info:
    # common tiles (unilang) - dup verify
    if not atlas.lang:
      for (tileName, tileFile) in atlas.tiles:
        if tileName in tilesUnique:
          tilesError = True
          print "  ! duplication '%s' (%s)" % (tileName, tileFile)
        else:
          tilesUnique.append(tileName)
      continue
    
    if not atlas.lang in langs:
      langs.append(atlas.lang)
    
    for (tileName, tileFile) in atlas.tiles:
      if not tileName in langTiles:
        langTiles.append(tileName)
  
  # full tiles list for earch lang
  tilesForLangs = []
  for lang in langs:
    tiles = []
    tiles.extend(langTiles)
    tilesForLangs.append(tiles)
  
  # del tiles from lang lists to verify lost tiles
  for atlas in atlas_info:
    if not atlas.lang:
      continue
    
    ind = langs.index(atlas.lang)
    masTiles = tilesForLangs[ind]
    
    for (tileName, tileFile) in atlas.tiles:
      if tileName in tilesUnique:
        tilesError = True
        print "  ! duplication '%s' in lang '%s' with common tile (%s)" % (tileName, atlas.lang, tileFile)
      
      if not tileName in masTiles:
        tilesError = True
        print "  ! duplication '%s' in lang '%s' (%s)" % (tileName, atlas.lang, tileFile)
      else:
        masTiles.remove(tileName)
  
  ind = 0
  for tiles in tilesForLangs:
    if len(tiles):
      tilesError = True
      print "  ! lost tiles in lang '%s':" % (langs[ind])
      for tile in tiles:
        print "  ! %s" % (tile)
    
    ind = ind + 1
  
  assert not tilesError, "some errors in tiles structure!"
  #############################################################################
  
  for atlas in atlas_info:
    atlasFilePath = os.path.join(output_dir_res, "tiles", atlas.name)
    gsi_sys.create_path_to_file(atlasFilePath)
    
    info = gsi_pvr.make_atlas(atlas.tiles, atlas.width, atlas.height, atlas.format, atlasFilePath)
    res.append((atlas.name, atlas.lang, info))

    # printing info
    texes = {}
    for (x, y, wr, hr, xo, yo, w, h, tileName, tw, th, texFilePath) in info:
      texFileName = os.path.basename(texFilePath)
      if not texes.has_key(texFileName):
        texSize = os.stat(texFilePath).st_size
        texAllSize += texSize
        print "  %s - %dx%d - %d" % (texFileName, tw, th, texSize)
        texes[texFileName] = True
  print "  ---------------------------------------"
  print "  size: %dKb" % (texAllSize / 1024)
  
  return res


# gets texture name from text .fnt file
def get_font_tex_name(font_file):
  with open(font_file, "r") as fin:
    fin.readline() # skip 1st line
    fin.readline() # skip 2nd line
    line = fin.readline()
    return line[line.find("\"") + 1:-2]
  
  assert 0, "wrong font .fnt format!"
  return ""


def build_fonts(font_info, output_dir_res):
  res = []
  
  for font in font_info:
    texName = get_font_tex_name(font.file)
    texPath = os.path.join(os.path.dirname(font.file), texName)
    
    texFileName, texFileExt = os.path.splitext(texName)
    texFontPath = os.path.join("fonts", texFileName + ".pvr")
    fntFontPath = os.path.join("fonts", os.path.basename(font.file))
    
    gsi_sys.copy_file(font.file, os.path.join(output_dir_res, fntFontPath))
    
    alreadyProccessed = False
    for name, fnt, path in res:
      if path == texFontPath:
        alreadyProccessed = True
        break
    
    if not alreadyProccessed:
      texOutPath = os.path.join(output_dir_res, texFontPath)
      gsi_sys.create_path_to_file(texOutPath)
      gsi_pvr.png_to_pvr(texPath, texOutPath, font.format)
    
    res.append((font.name, fntFontPath, texFontPath))
  
  return res;


def build_sounds(sound_info, output_dir_res):
  res = []
  
  for sound in sound_info:
    gsi_sys.copy_file(sound.file, os.path.join(output_dir_res, sound.path))
    res.append((sound.name, sound.path, sound.group, sound.looped))
  
  return res;
