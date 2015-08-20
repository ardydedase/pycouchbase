#!/usr/bin/env python
# -*- coding: utf-8 -*-


try:
    from setuptools import setup
except ImportError:
    from distutils.core import setup


with open('README.rst') as readme_file:
    readme = readme_file.read()

with open('HISTORY.rst') as history_file:
    history = history_file.read().replace('.. :changelog:', '')

requirements = [
    # TODO: put package requirements here
    'cffi==1.1.0',
    'couchbase==2.0',
    'python-dateutil==2.1',
    'jsonpickle==0.9.2'
]

test_requirements = [
    # TODO: put package test requirements here
    'cffi==1.1.0',
    'couchbase==2.0',
    'python-dateutil==2.1',
    'jsonpickle==0.9.2'
]

setup(
    name='pycouchbase',
    version='0.1.0b1',
    description="PyCouchbase",
    long_description=readme + '\n\n' + history,
    author="Ardy Dedase",
    author_email='ardy.dedase@gmail.com',
    url='https://github.com/ardydedase/pycouchbase',
    packages=[
        'pycouchbase',
    ],
    package_dir={'pycouchbase':
                 'pycouchbase'},
    include_package_data=True,
    install_requires=requirements,
    license="BSD",
    zip_safe=False,
    keywords='pycouchbase database couchbase python nosql couchbasekit sdk',
    classifiers=[
        'Development Status :: 4 - Beta',
        'Intended Audience :: Developers',
        'License :: OSI Approved :: BSD License',
        'Natural Language :: English',
        "Programming Language :: Python :: 2",
        'Programming Language :: Python :: 2.6',
        'Programming Language :: Python :: 2.7',
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: 3.3',
        'Programming Language :: Python :: 3.4',
    ],
    test_suite='tests',
    tests_require=test_requirements
)
