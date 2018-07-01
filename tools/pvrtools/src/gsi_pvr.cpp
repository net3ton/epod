#include <Python.h>
#include "common.h"

// makes .pvr altas from income .png
// input:
// 1. list of tuples ((string)image id, (string)image path)
// 2. atlas width
// 3. atlas height
// 4. .pvr format
// 5. .pvr path
// output:
// list of tuples (tile info):
// 1. x
// 2. y
// 3. width
// 4. height
// 5. id (same as income image id)
// 6. atlas width
// 7. atlas height
// 8. atlas path
//

static PyObject *make_atlas(PyObject *self, PyObject *args)
{
  (void)self;

  PyObject *tilesObj;
  int width, height;
  const char *outformat, *outfile;
  
  if (!PyArg_ParseTuple(args, "Oiiss:make_atlas", &tilesObj, &width, &height, &outformat, &outfile))
    return NULL;

  if (!PyList_CheckExact(tilesObj))
  {
    PyErr_Format(PyExc_TypeError, "first parameter must be a list of tiles!");
    return NULL;
  }

  ttex::TileVector tiles;

  int tilesCount = (int)PyList_GET_SIZE(tilesObj);
  for (int i=0 ; i<tilesCount ; ++i)
  {
    PyObject *tileNamesObj = PyList_GET_ITEM(tilesObj, i);
    const char *tileId, *tileName;

    if (!PyArg_ParseTuple(tileNamesObj, "ss", &tileId, &tileName))
      return NULL;

    tiles.push_back(new ttex::TileInfo(tileName, tileId));
  }

  if (!ttex::make_atlas(tiles, width, height, outformat, outfile))
  {
    PyErr_Format(PyExc_Exception, ttex::get_error());

    for (size_t i=0 ; i<tiles.size() ; ++i)
      delete tiles[i];

    return NULL;
  }

  PyObject *outListObj = PyList_New((Py_ssize_t)tiles.size());
  
  for (size_t i=0 ; i<tiles.size() ; ++i)
  {
    const ttex::TileInfo *tile = tiles[i];

    PyObject *infoObj = Py_BuildValue("iiiiiiiisiis",
      tile->x, tile->y, tile->width, tile->height,
      tile->xoffset, tile->yoffset, tile->wtex, tile->htex,
      tile->id.c_str(), width, height, tile->texName.c_str());

    PyList_SetItem(outListObj, (Py_ssize_t)i, infoObj);

    delete tile;
  }

  return outListObj;
}


static PyObject *png_to_pvr(PyObject *self, PyObject *args)
{
  (void)self;

  const char *infile, *outformat, *outfile;
  
  if (!PyArg_ParseTuple(args, "sss:png_to_pvr", &infile, &outfile, &outformat))
    return NULL;

  int width, height;
  if (!ttex::png_to_pvr(infile, outformat, outfile, width, height))
  {
    PyErr_Format(PyExc_Exception, ttex::get_error());
    return NULL;
  }

  return PyBool_FromLong(1);
}


static PyMethodDef PvrMethods[] =
{
  {"make_atlas", make_atlas, METH_VARARGS, "Makes pvr atlas from png tile set."},
  {"png_to_pvr", png_to_pvr, METH_VARARGS, "Converts png to pvr."},
  {NULL, NULL, 0, NULL}
};


PyMODINIT_FUNC initgsi_pvr(void)
{
  (void) Py_InitModule("gsi_pvr", PvrMethods);
}
