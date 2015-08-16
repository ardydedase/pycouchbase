# -*- coding: utf-8 -*-

from couchbase.bucket import Bucket

class Connection(object):
    username = None
    password = None
    server = None
    connection = None
    _buckets = {}

    def __new__(cls, *args, **kwargs):
        raise RuntimeWarning('Connection class is not intended to create instances.')

    @classmethod
    def auth(cls, password=None, server='localhost', port='8091'):
        cls.password = password
        cls.connection = "couchbase://{server}".format(server=server)
        return cls.connection


    @classmethod
    def bucket(cls, bucket_name, connection=None):
        """Gives the bucket from couchbase server.

        :param bucket_name: Bucket name to fetch.
        :type bucket_name: str
        :returns: couchbase driver's Bucket object.
        :rtype: :class:`couchbase.client.Bucket`
        :raises: :exc:`RuntimeError` If the credentials wasn't set.
        """
        connection = cls.connection if connection == None else connection


        if bucket_name not in cls._buckets:
            connection = "{connection}/{bucket_name}".format(connection=connection, bucket_name=bucket_name)

            if cls.password:
                cls._buckets[connection] = Bucket(connection, password=cls.password)
            else:
                cls._buckets[connection] = Bucket(connection)

        return cls._buckets[connection]