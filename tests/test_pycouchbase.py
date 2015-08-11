#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
test_pycouchbase
----------------------------------

Tests for `pycouchbase` module.
"""

import unittest
import datetime

from pycouchbase import Connection
from pycouchbase import Document, register_view
from pycouchbase.fields import EmailField, ChoiceField

from couchbase.exceptions import CouchbaseNetworkError, CouchbaseTransientError

from example.samples.publisher import Publisher
from example.samples.book import Book


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
        'publisher': Publisher,  # kind of foreign key
        'books': [Book],  # 1-to-many, or many-to-many? some-to-some.. :)
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


class TestPyCouchbase(unittest.TestCase):

    author = {}

    def setUp(self):
        Connection.auth(server='localhost')
        self.author = Author()

    def test_is_new_record(self):
        self.assertTrue(self.author.is_new_record)

    def test_missing_key(self):
        try:
            self.author.validate()
        except Author.StructureError as why:
            expected_why = "Key field 'slug' is defined but not provided."
            self.assertEqual(str(why), expected_why)

    def test_missing_required(self):
        self.author.slug = u'douglas_adams'

        try:
            self.author.validate()
        except Author.StructureError as why:
            expected_why = "Required field for 'first_name' is missing."
            self.assertEqual(str(why), expected_why)

    def test_successful_validation(self):
        self.assertTrue(isinstance(self.author, dict))
        self.author.update({
            'slug': u'douglas_adams',
            'first_name': u'Douglas',
            'last_name': u'Adams',
            'gender': Gender('M'),
            'email': EmailField('dna@example.com'),
        })

        self.assertTrue(self.author.validate())

    def test_failed_validation(self):
        self.assertTrue(isinstance(self.author, dict))
        self.author.update({
            'slug': u'douglas_adams',
            'first_name': u'Douglas',
            'last_name': u'Adams',
            'gender': 1,
            'email': EmailField('dna@example.com'),
        })

        try:
            self.author.validate()
        except Author.StructureError as why:
            expected_why = "'gender' does not fit the required structure, expected Gender but 'int' is given."
            self.assertEqual(str(why), expected_why)

    def test_successful_save(self):
        self.author.update({
            'slug': u'douglas_adams',
            'first_name': u'Douglas',
            'last_name': u'Adams',
            'gender': Gender('M'),
            'email': EmailField('dna@example.com'),
        })

        try:
            self.author.save()
            self.assertEqual(self.author.id, u'douglas_adams')
            self.assertEqual(self.author.doc_id, u'author_douglas_adams')
            self.assertEqual(self.author.birthday, None)
        except (CouchbaseNetworkError, CouchbaseTransientError) as why:
            print(str(why))

        try:
            self.author.not_existent_field
        except AttributeError as why:
            # test passed
            pass
        except e:
            self.fail('Unexpected exception thrown:', e)
        else:
            self.fail('ExpectedException not thrown')

    def test_successful_save_multi(self):

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

        for d in list_data:
            self.author.update(d)
            self.author.add_multi(self.author)

        self.author.save_multi()

    def test_existing_record(self):
        author = Author('douglas_adams')
        self.assertFalse(author.is_new_record)
        view = author.view('by_fullname')
        self.assertTrue(isinstance(view, list))

    def tearDown(self):
        pass
