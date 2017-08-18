import sys
import yaml
import json
from jsonschema import validate

schema_file = sys.argv[1]
json_file = sys.argv[2]

with open (schema_file, 'r') as schema_yaml:
    schema = yaml.load(schema_yaml.read())
with open (json_file, 'r') as target:
    validate(json.load(target), schema)