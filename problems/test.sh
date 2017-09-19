HERE=$(dirname $0)
ENCODER_HOME=$HERE/../tools/pyencoder

for f in $HERE/*.txt; do
	cat $f | python3 $ENCODER_HOME/encoder.py
done
