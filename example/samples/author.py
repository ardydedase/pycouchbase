#! /usr/bin/env python
import datetime
from pycouchbase import Document, Connection
from pycouchbase.fields import EmailField, ChoiceField
from example.samples.publisher import Publisher
from example.samples.book import Book

Connection.auth('pycouchbase_samples', 'pycouchbase')

class Gender(ChoiceField):
    CHOICES = {
        'M': 'Male',
        'F': 'Female',
    }


class Author(Document):
    __bucket_name__ = 'pycouchbase_samples'
    __key_field__ = 'slug' # optional
    doc_type = 'author'
    structure = {
        'slug': unicode,
        'first_name': unicode,
        'last_name': unicode,
        'gender': Gender,
        'email': EmailField,
        'publisher': Publisher, # kind of foreign key
        'books': [Book], # 1-to-many, or many-to-many? some-to-some.. :)
        'has_book': bool,
        'age': int,
        'birthday': datetime.date,
        'created_at': datetime.datetime,
    }
    # optional
    default_values = {
        'has_book': False,
        # don't worry about the timezone info!
        # it's auto assigned as to UTC, so all you have to do is:
        'created_at': datetime.datetime.utcnow,
    }
    # optional
    required_fields = (
        'slug',
        'first_name',
        'last_name',
        'email',
    )