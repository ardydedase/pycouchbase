#! /usr/bin/env python
import datetime
from pycouchbase import Document, Connection
from pycouchbase.fields import EmailField, PasswordField

Connection.auth('pycouchbase_samples', 'pycouchbase')

class User(Document):
    __bucket_name__ = 'pycouchbase_samples'
    doc_type = 'user'
    structure = {
        'username': unicode,
        'email': EmailField,
        'password': PasswordField,
        'logins': {
            # datetime: ip
            datetime.datetime: unicode,
        },
    }