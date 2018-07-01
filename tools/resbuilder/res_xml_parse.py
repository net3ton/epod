import sys
import gsi_xml as x
import os.path


class Atlas:
  name = None
  lang = None
  tiles = None
  format = "rgba4444"
  width = 1024
  height = 1024

class Font:
  name = None
  file = None
  format = "rgba8888"

class Sound:
  name = None
  path = None
  file = None
  looped = 0
  group = 0     # [sound, music]


def parse_pack(x_pack, res_dir):
  atlas = Atlas()
  atlas.name = str(x.attr(x_pack, "name"))
  atlas.tiles = []
  
  if x.has_attr(x_pack, "lang"):
    atlas.lang = str(x.attr(x_pack, "lang"))
  
  if x.has_attr(x_pack, "width"):
    atlas.width = int(x.attr(x_pack, "width"))
  if x.has_attr(x_pack, "height"):
    atlas.height = int(x.attr(x_pack, "height"))
  if x.has_attr(x_pack, "format"):
    atlas.format = str(x.attr(x_pack, "format"))
  
  for x_image in x.elements(x_pack, "image"):
    tileName = str(x.attr(x_image, "name"))
    tileFile = os.path.join(res_dir, str(x.attr(x_image, "file")))
    
    assert os.path.exists(tileFile), "image '%s' doesn't exist" % tileFile
    atlas.tiles.append((tileName, tileFile))
  
  for x_dir in x.elements(x_pack, "dir"):
    dirPath = os.path.join(res_dir, str(x.attr(x_dir, "path")))
    assert os.path.exists(dirPath), "dir '%s' doesn't exist" % dirPath
    
    items = os.listdir(dirPath)
    for item in items:
      tileFile = os.path.join(dirPath, item)
      if os.path.isfile(tileFile):
        tileName, ext = os.path.splitext(item)
        if ext.lower() in [".png"]:
          atlas.tiles.append((tileName, tileFile))
  
  return atlas


def parse_font(x_font, res_dir):
  font = Font()
  font.name = str(x.attr(x_font, "name"))
  font.file = os.path.join(res_dir, str(x.attr(x_font, "file")))
  
  if x.has_attr(x_font, "format"):
    font.format = str(x.attr(x_font, "format"))
  
  return font


def parse_sound(x_sound, res_dir, grp):
  sound = Sound()
  sound.name = str(x.attr(x_sound, "name"))
  sound.path = str(x.attr(x_sound, "file"))
  sound.file = os.path.join(res_dir, sound.path)
  sound.group = grp
  
  # for music 'looped' is true by default
  if grp == 1:
    sound.looped = 1
  
  if x.has_attr(x_sound, "looped"):
    if str(x.attr(x_sound, "looped")) == "1":
      sound.looped = 1
  
  return sound


def parse(xml_filename, res_dir):
  assert os.path.isdir(res_dir), "directory '%s' doesn't exist" % res_dir
  
  x_res = x.element(x.parse_file(xml_filename), "resources")
  
  x_packs = x.elements(x_res, "atlas")
  res_packs = map(lambda e : parse_pack(e, res_dir), x_packs)
  print "  atlases: %d" % len(res_packs)
  
  x_fonts = x.elements(x_res, "font")
  res_fonts = map(lambda e : parse_font(e, res_dir), x_fonts)
  print "  fonts: %d" % len(res_fonts)
  
  x_sounds = x.elements(x_res, "sound")
  res_sounds = map(lambda e : parse_sound(e, res_dir, 0), x_sounds)
  print "  sounds: %d" % len(res_sounds)
  x_music = x.elements(x_res, "music")
  res_music = map(lambda e : parse_sound(e, res_dir, 1), x_music)
  print "  musics: %d" % len(res_music)
  res_sounds.extend(res_music)
  
  return (res_packs, res_fonts, res_sounds)
