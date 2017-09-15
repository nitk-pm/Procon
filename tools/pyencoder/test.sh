HERE=$(dirname $0)
FORMAT_HOME=$HERE/../../format

cat $HERE/sample.txt | python3 $HERE/encoder.py
python3 $FORMAT_HOME/validate.py $FORMAT_HOME/frame-schema.yml $HERE/frame.json
python3 $FORMAT_HOME/validate.py $FORMAT_HOME/piece-schema.yml $HERE/piece.json
