#! /usr/bin/env python
"""
couchbasekit.viewsync
~~~~~~~~~~~~~~~~~~~~~

:website: http://github.com/kirpit/couchbasekit
:copyright: Copyright 2013, Roy Enjoy <kirpit *at* gmail.com>, see AUTHORS.txt.
:license: MIT, see LICENSE.txt for details.
"""
import os
import shutil
from couchbase.exceptions import BucketNotFoundError
from .connection import Connection 
from .document import Document

def register_view(design_doc, full_set=True):
    """Model document decorator to register its design document view::

        @register_view('dev_books')
        class Book(Document):
            __bucket_name__ = 'mybucket'
            doc_type = 'book'
            structure = {
                # snip snip
            }

    :param design_doc: The name of the design document.
    :type design_doc: basestring
    :param full_set: Attach full_set param to development views.
    :type full_set: bool
    """
    def _injector(doc):
        if not isinstance(doc, type) or not issubclass(doc, Document):
            raise TypeError("Class must be a cbwrapper 'Document' subclass.")
        doc.__view_name__ = design_doc
        doc.full_set = full_set
        ViewSync._documents.add(doc)
        return doc
    return _injector


class ViewSync(object):
    """This is an experimental helper to download, upload and synchronize your
    couchbase views (both map and reduce JavaScript functions) in an organized
    way.

    Unfortunately, it's quite impossible to synchronize these views since
    couchbase doesn't provide any information about when a specific view was
    created and modified. So we can't know if previously downloaded .js file or
    the current one at couchbase server should be replaced..

    This class also works in a singleton pattern so all its methods are
    ``@classmethod`` that you don't need to create an instance at all.

    In order to use this tool, you have to set VIEW_PATH attribute of the class
    to the directory wherever you want to keep downloaded JavaScript files. It is
    better to keep that directory under version controlled folder, as they can
    also become your view backups::

        ViewSync.VIEW_PATH = '/path/to/your/js/view/backups'
    """
    VIEWS_PATH = None
    _documents = set()

    @classmethod
    def _check_folder(cls):
        if not isinstance(cls.VIEWS_PATH, basestring):
            raise RuntimeError("ViewSync.VIEWS_PATH must be set")
        if not os.path.exists(cls.VIEWS_PATH) or not os.path.isdir(cls.VIEWS_PATH):
            raise RuntimeError(
                "Directory must created first; '%s'" % cls.VIEWS_PATH
            )

    @classmethod
    def download(cls):
        """Downloads all the views from server for the registered model
        documents into the defined :attr:`VIEW_PATHS` directory.

        This method **removes** previous views directory if exist.
        """
        cls._check_folder()
        os.chdir(cls.VIEWS_PATH)
        # iterate documents
        for doc in cls._documents:
            design_doc = doc().view()
            if design_doc is None:
                continue
            bucket_name = design_doc.bucket.name
            # iterate viewtypes (i.e. spatial and views)
            for view_type, views in design_doc.ddoc.iteritems():
                save_dir = '%s/%s/%s' % (bucket_name, design_doc.name, view_type)
                try:
                    # remove and recreate the dir
                    shutil.rmtree(save_dir, ignore_errors=True)
                    os.makedirs(save_dir)
                except OSError:
                    pass
                for name, view in views.iteritems():
                    if isinstance(view, unicode) and view_type=='spatial':
                        spatial_file = '%s/%s.spatial.js' % (save_dir, name)
                        with open(spatial_file, 'w') as f:
                            f.write(view)
                        print 'Downloaded: %s' % spatial_file
                    if isinstance(view, dict) and 'map' in view:
                        map_file = '%s/%s.map.js' % (save_dir, name)
                        with open(map_file, 'w') as f:
                            f.write(view['map'])
                        print 'Downloaded: %s' % map_file
                    if isinstance(view, dict) and 'reduce' in view:
                        reduce_file = '%s/%s.reduce.js' % (save_dir, name)
                        with open(reduce_file, 'w') as f:
                            f.write(view['reduce'])
                        print 'Downloaded: %s' % reduce_file
        pass

    @classmethod
    def upload(cls):
        """Uploads all the local views from :attr:`VIEW_PATHS` directory
        to CouchBase server

        This method **over-writes** all the server-side views with the same
        named ones coming from :attr:`VIEW_PATHS` folder.
        """
        cls._check_folder()
        os.chdir(cls.VIEWS_PATH)
        buckets = dict()
        # iterate local folders
        for bucket_name in os.listdir(cls.VIEWS_PATH):
            if not os.path.isdir(bucket_name):
                continue
            # get bucket object
            if bucket_name not in buckets:
                try:
                    bucket = Connection.bucket(bucket_name)
                except BucketNotFoundError as why:
                    print "[WARNING] %s" % str(why)
                    continue
                else:
                    buckets[bucket_name] = bucket
            else:
                bucket = buckets[bucket_name]
            # go through design docs
            for ddoc_name in os.listdir(bucket_name):
                views_path = '%s/%s/views' % (bucket_name, ddoc_name)
                spatial_path = '%s/%s/spatial' % (bucket_name, ddoc_name)
                if not (os.path.exists(views_path) and os.path.isdir(views_path)) and \
                   not (os.path.exists(spatial_path) and os.path.isdir(spatial_path)):
                    continue
                # initialize design doc
                new_ddoc = {
                    'views': {},
                    'spatial': {},
                }
                # map and reduces
                if os.path.exists(views_path) and os.path.isdir(views_path):
                    for filename in os.listdir(views_path):
                        if not os.path.isfile('%s/%s' % (views_path, filename)) or \
                           not filename.endswith(('.map.js', '.reduce.js')):
                            continue
                        view_name, view_type, js = filename.rsplit('.', 2)
                        if view_name not in new_ddoc['views']:
                            new_ddoc['views'][view_name] = {}
                        with open('%s/%s' % (views_path, filename), 'r') as f:
                            new_ddoc['views'][view_name][view_type] = f.read()
                # spatial views
                if os.path.exists(spatial_path) and os.path.isdir(spatial_path):
                    for filename in os.listdir(spatial_path):
                        if not os.path.isfile('%s/%s' % (spatial_path, filename)) or \
                           not filename.endswith('.spatial.js'):
                            continue
                        view_name = filename.rsplit('.', 2)[0]
                        with open('%s/%s' % (spatial_path, filename), 'r') as f:
                            new_ddoc['spatial'][view_name] = f.read()
                bucket['_design/%s' % ddoc_name] = new_ddoc
                print 'Uploaded design document: %s' % ddoc_name
        pass

    @classmethod
    def sync(cls):
        """Not implemented yet."""
        raise NotImplementedError()
