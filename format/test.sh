HERE=$(dirname $0)

python3 $HERE/validate.py $HERE/frame-schema.yml $HERE/frame-sample.json
python3 $HERE/validate.py $HERE/piece-schema.yml $HERE/piece-sample.json
python3 $HERE/validate.py $HERE/output-schema.yml $HERE/output-sample.json
