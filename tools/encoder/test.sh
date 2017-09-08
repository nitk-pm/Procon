omake
cat ../problems/sample.txt | ./encoder
python ../format/validate.py ../format/piece-schema.yml ./piece.json
python ../format/validate.py ../format/frame-schema.yml ./frame.json
