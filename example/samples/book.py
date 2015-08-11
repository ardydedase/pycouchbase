#! /usr/bin/env python
import datetime
from pycouchbase import Document, Connection

Connection.auth('pycouchbase_samples', 'pycouchbase')

class Book(Document):
    __bucket_name__ = 'pycouchbase_samples'
    doc_type = 'book'
    structure = {
        'title': unicode,
        'published_at': datetime.date,
        'pictures': list,
        'tags': [unicode],
        'category': {
            u'History': bool,
            u'Sci-Fiction': bool,
            u'Cooking': {
                u'Turkish': bool,
                u'Italian': bool,
                u'Fast Food': bool,
                u'Dessert': bool,
            },
        },
    }