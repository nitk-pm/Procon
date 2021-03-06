
from flask import (
    Flask,
    render_template,
    send_file,
    abort
)

app = Flask(__name__)


@app.route('/')
def index():
    return render_template('index.html')


@app.route('/admin')
def admin():
    return render_template('admin.html')


@app.route('/download/<path>')
def DownloadFile(path=None):
    if path is None:
        abort(400)
    try:
        return send_file('{}/{}'.format('data', path))
    except Exception as e:
        abort(400)


if __name__ == '__main__':
    app.run(host='0.0.0.0')
