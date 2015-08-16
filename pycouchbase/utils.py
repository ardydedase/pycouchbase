# -*- coding: utf-8 -*-

# Credits to: https://github.com/rfk/pyenchant/blob/master/enchant/utils.py

try:
    unicode = unicode
except NameError:
    str = str
    unicode = str
    bytes = bytes
    basestring = (str,bytes)
else:
    str = str
    unicode = unicode
    bytes = str
    basestring = basestring