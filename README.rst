===============================
PyCouchbase
===============================

.. image:: https://img.shields.io/travis/ardydedase/pycouchbase.svg
        :target: https://travis-ci.org/ardydedase/pycouchbase

.. image:: https://img.shields.io/pypi/v/pycouchbase.svg
        :target: https://pypi.python.org/pypi/pycouchbase


Why add another layer to the SDK?
------------------------------------

* Inconsistent data types in Couchbase documents can be a real pain.
* Better management of multiple Couchbase connections.
* Readability of Couchbase documents in the Python source code while still being able to use Couchbase SDK's operations.

Features
---------------

* This was originally forked from and inspired by couchbasekit_.
* Validate Couchbase documents.
* Represent Couchbase documents as Python objects.
* Easily manage multiple Couchbase connections.
* Supports `Couchbase Python SDK 2.0`_ operations.
* There are data retrieval operations that are already included in couchbasekit but I haven't thoroughly tested it with PyCouchbase.

.. _couchbasekit: https://github.com/kirpit/couchbasekit
.. _Couchbase Python SDK 2.0: http://docs.couchbase.com/developer/python-2.0/introduction.html

Installation
---------------

At the command line::

    $ easy_install pycouchbase

Or, if you have virtualenvwrapper installed::

    $ mkvirtualenv pycouchbase
    $ pip install pycouchbase

Getting Started
---------------

Let us go through a simple example.

Import everything we need:

.. code:: python

    import datetime

    from pycouchbase import Connection
    from pycouchbase import Document, register_view
    from pycouchbase.fields import EmailField, ChoiceField


Declare the Document class:

.. code:: python

    # You can define your own field/data type
    
    class Gender(ChoiceField):
        CHOICES = {
            'M': 'Male',
            'F': 'Female',
        }
    
    @register_view('dev_authors')
    class Author(Document):
        __bucket_name__ = 'couchbasekit_samples'
        __key_field__ = 'slug'  # optional
        doc_type = 'author'
        structure = {
            'slug': unicode,
            'first_name': unicode,
            'last_name': unicode,
            'gender': Gender,
            'email': EmailField,
            'has_book': bool,
            'age': int,
            'birthday': datetime.date,
            'created_at': datetime.datetime,
        }
        default_values = {  # optional
            'has_book': False,
            # don't worry about the timezone info!
            # it's auto assigned as to UTC, so all you have to do is:
            'created_at': datetime.datetime.utcnow,
        }
        required_fields = (  # optional
            'slug',
            'first_name',
            'last_name',
            'email',
        )
    

Validate and save your document:

.. code:: python
    
    local_connection = Connection.auth(server='localhost')
    author = Author()
    bucket = author.get_bucket(local_connection)
    
    author.update({
        'slug': u'douglas_adams',
        'first_name': u'Douglas',
        'last_name': u'Adams',
        'gender': Gender('M'),
        'email': EmailField('dna@example.com'),
    })
    
    # Try to validate before saving
    try:
        author.validate()
        try:
            rvs = bucket.insert(author.slug, author.encode())
        except KeyExistsError as why:
            print(why)  
    except Author.StructureError as why:
        # when the data structure is invalid
        print(why)

Save multiple documents:

.. code:: python

    local_connection = Connection.auth(server='localhost')
    author = Author()

    list_data = [{
        'slug': u'douglas_adams',
        'first_name': u'Douglas',
        'last_name': u'Adams',
        'gender': Gender('M'),
        'email': EmailField('dna@example.com'),
    }, {
        'slug': u'isaac_asimov',
        'first_name': u'Isaac',
        'last_name': u'Asimov',
        'gender': Gender('M'),
        'email': EmailField('dna@example.com'),
    }]

    try:
        bucket = author.get_bucket(local_connection)
        updated_authors = {}

        for d in list_data:
            author.update(d)
            try:
                # validate!
                author.validate()
                updated_authors.update({
                    d['slug']: author.encode()
                })
            except author.StructureError as why:
                print(why)

        # save multiple data
        rvs = bucket.upsert_multi(updated_authors)
    except CouchbaseNetworkError as why:
        print(why)

Manage multiple connections:

.. code:: python
    
    connection_1 = Connection.auth(server='server_1')
    connection_2 = Connection.auth(server='server_2')

    # where doc_1 and doc_2 are document objects
    bucket_1 = doc_1.get_bucket(connection_1)
    bucket_2 = doc_2.get_bucket(connection_2)

Bucket objects can support any `Couchbase Python SDK 2.0`_ operations:

.. code:: python

    bucket_1.get('key_or_id')
    bucket_1.insert('key_or_id', value)

More about Couchbase SDK's supported operations here: http://docs.couchbase.com/developer/python-2.0/introduction.html