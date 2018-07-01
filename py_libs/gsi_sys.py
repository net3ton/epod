import shutil
import os.path

def create_path_to_file(dst):
    dst_dir = os.path.dirname(dst)
    if (dst_dir and not os.path.exists(dst_dir)):
        os.makedirs(dst_dir)


def copy_file(src, dst):
    if not os.path.exists(src):
        raise Exception, "source file '%s' not found" % src
    create_path_to_file(dst)
    shutil.copy(src, dst)


def spawn(exe, args):
    if not os.path.exists(exe):
        raise Exception, "can't find executable '%s'" % exe    
    if 0 != os.spawnv(os.P_WAIT, exe, [exe] + args):
        raise Exception, "'%s' returns error code" % exe


def get_file_data(filename):
    f = open(filename, "rb")
    try:
        return f.read()
    finally:
        f.close()


def set_file_data(filename, data):
    f = open(filename, "wb")
    try:
        f.write(data)
        f.flush()
    finally:
        f.close()


def __deltree_skipdirs_callback(function, path, excinfo):
    if (function == os.rmdir and os.path.isdir(path)):
        return
    raise Exception("can't delete '%s' ('%s')" % (path, excinfo))

def deltree_skipdirs(path):
    #Deleting folder tree, allows blocked directories to be non deleted and raise no error.
    if (not os.path.isdir(path)):
        return
    shutil.rmtree(path, False, __deltree_skipdirs_callback)        


def copy_tree(from_path, to_path, dir_filter = None, file_filter = None, delete = True):
    #Supports dir_filter/file_filter.
    #May delete or not delete destination folder (delete = True)
    #Doesn't depend on different shutil.copytree details in python 2.4/2.5
    #Doesn't copy empty folders now, don't neccesery now. May be fixed in future.
    
    if not os.path.exists(from_path):
        raise RuntimeError("can't find '%s' " % from_path)
    
    #todo: don't delete top-level. allow non-strict deletion of blocked folders?
    if (delete):
        deltree_skipdirs(to_path)
    
    for dir, subdirs, files in os.walk(from_path):
        if dir_filter:
            subdirs[:] = filter(dir_filter, subdirs)
            
        assert dir.startswith(from_path)
        copy_to_current_dir = os.path.join(to_path, dir[len(from_path) + 1:]) # extract "subdir" from string "from_path/subdir"
        
        for file in files:
            if not file_filter or file_filter(file):
                copy_file(
                    os.path.join(dir, file),
                    os.path.join(copy_to_current_dir, file))        


import zlib
import struct

def compress_add_header(data):
    return struct.pack("i", len(data)) + zlib.compress(data, 9)
