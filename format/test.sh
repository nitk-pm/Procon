HERE=$(dirname $0)

python3 $HERE/validate.py $HERE/geometry-schema.yml $HERE/geometry-sample.json
python3 $HERE/validate.py $HERE/output-schema.yml $HERE/output-sample.json
