# coding: utf-8

import sys
import yaml
import json
import codecs
from jsonschema import validate

schema_file = sys.argv[1]
json_file = sys.argv[2]

with codecs.open (schema_file, 'r', 'utf-8') as schema_yaml:
    schema = yaml.load(schema_yaml.read())
with codecs.open (json_file, 'r', 'utf-8') as target:
    validate(json.load(target), schema)
