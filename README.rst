===============================
PyCouchbase
===============================

.. image:: https://img.shields.io/travis/ardydedase/pycouchbase.svg
        :target: https://travis-ci.org/ardydedase/pycouchbase

.. image:: https://img.shields.io/pypi/v/pycouchbase.svg
        :target: https://pypi.python.org/pypi/pycouchbase


PyCouchbase

* This is a fork of <a target="_blank" href="https://github.com/kirpit/couchbasekit">couchbasekit</a> but made it work with Couchbase Python SDK 2.0.
* For Couchbase document validation and readability in Python code.
* Free software: BSD license
* Supports <a href="http://docs.couchbase.com/developer/python-2.0/introduction.html" target="_blank">Couchbase Python SDK 2.0</a>.


Getting Started
---------------

Let us go through a simple example.

Import everything we need:

	import datetime

	from pycouchbase import Connection
	from pycouchbase import Document, register_view
	from pycouchbase.fields import EmailField, ChoiceField
	
Declare the Document class:

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

Validate, save and retrieve your document:

	author = Author()
	
	author.update({
		'slug': u'douglas_adams',
		'first_name': u'Douglas',
		'last_name': u'Adams',
		'gender': Gender('M'),
		'email': EmailField('dna@example.com'),
	})
	
	# Try to validate
	try:
		author.validate()
	except Author.StructureError as why:
		print(why)
	
	# save
	author.save()
