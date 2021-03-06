# -*- coding: utf-8 -*-

__author__ = 'Ardy Dedase'
__email__ = 'ardy.dedase@gmail.com'
__version__ = '0.1.0'

try:
	import couchbase
except ImportError:
	import couchbase_ffi
	import couchbase

from .utils import *
from .connection import Connection
from .document import Document
from .viewsync import register_view

__all__ = (
    Connection,
    Document,
    register_view
)