#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
test_pycouchbase
----------------------------------

Tests for `pycouchbase` module.
"""

import unittest
import datetime

from pprint import pprint
from pycouchbase.utils import *
from pycouchbase import Connection
from pycouchbase import Document, register_view
from pycouchbase.fields import EmailField, ChoiceField

from couchbase.exceptions import CouchbaseNetworkError, CouchbaseTransientError, KeyExistsError

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
        'slug': str,
        'first_name': str,
        'last_name': str,
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
    local_connection = None

    def setUp(self):
        self.local_connection = Connection.auth(server='localhost')
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
        self.author.slug = 'douglas_adams'

        try:
            self.author.validate()
        except Author.StructureError as why:
            expected_why = "Required field for 'first_name' is missing."
            self.assertEqual(str(why), expected_why)

    def test_successful_validation(self):
        self.assertTrue(isinstance(self.author, dict))
        self.author.update({
            'slug': 'douglas_adams',
            'first_name': 'Douglas',
            'last_name': 'Adams',
            'gender': Gender('M'),
            'email': EmailField('dna@example.com'),
        })

        self.assertTrue(self.author.validate())

    def test_failed_validation(self):
        self.assertTrue(isinstance(self.author, dict))
        self.author.update({
            'slug': 'douglas_adams',
            'first_name': 'Douglas',
            'last_name': 'Adams',
            'gender': 1,
            'email': EmailField('dna@example.com'),
        })

        try:
            self.author.validate()
        except Author.StructureError as why:
            expected_why = "'gender' does not fit the required structure, expected Gender but 'int' is given."
            self.assertEqual(str(why), expected_why)

    def test_successful_save(self):
        author = Author()
        try:
            bucket = author.get_bucket(self.local_connection)
            author.update({
                'slug': 'douglas_adams',
                'first_name': 'Douglas',
                'last_name': 'Adams',
                'gender': Gender('M'),
                'email': EmailField('dna@example.com'),
            })

            try:
                author.validate()
                try:
                    rvs = bucket.insert("%s-1" % author.slug, author.encode())
                    print("rvs:")
                    print(rvs)
                except KeyExistsError as why:
                    print(why)
            except author.StructureError as why:
                print(why)

            try:
                author.not_existent_field
            except AttributeError as why:
                # test passed
                pass
            except e:
                self.fail('Unexpected exception thrown:', e)
            else:
                self.fail('ExpectedException not thrown')
        except CouchbaseNetworkError as why:
            print(why)


    def test_successful_save_multi(self):
        list_data = [{
            'slug': 'douglas_adams',
            'first_name': 'Douglas',
            'last_name': 'Adams',
            'gender': Gender('M'),
            'email': EmailField('dna@example.com'),
        }, {
            'slug': 'isaac_asimov',
            'first_name': 'Isaac',
            'last_name': 'Asimov',
            'gender': Gender('M'),
            'email': EmailField('dna@example.com'),
        }]

        author = Author()

        try:
            bucket = author.get_bucket(self.local_connection)
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

    def test_existing_record(self):
        try:
            author = Author('douglas_adams')
            bucket = author.get_bucket(self.local_connection)
            d = bucket.get(author.id)
            author.update(d.value)
            self.assertEqual(author.slug, 'douglas_adams')
        except CouchbaseNetworkError as why:
            print(why)

    def tearDown(self):
        pass
