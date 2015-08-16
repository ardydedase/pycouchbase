#! /usr/bin/env python
import datetime
from pycouchbase import Document, Connection
from pycouchbase.utils import *
from example.samples.book import Book

Connection.auth('pycouchbase_samples', 'pycouchbase')

class Publisher(Document):
    __bucket_name__ = 'pycouchbase_samples'
    __key_field__ = 'slug' # optional
    doc_type = 'publisher'
    structure = {
        'slug': unicode,
        'name': unicode,
        'phone': unicode,
        'address': unicode,
        'established_year': int,
        'created_at': datetime.datetime,
        'publishes': {
            datetime.date: [Book],
        }
    }
    # optional
    default_values = {
        # don't worry about the timezone info!
        # it's auto assigned as to UTC, so all you have to do is:
        'created_at': datetime.datetime.utcnow,
    }
    # optional
    required_fields = (
        'slug',
        'name',
    )