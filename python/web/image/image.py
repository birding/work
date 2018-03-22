# encoding=utf-8

from flask import Flask,redirect,url_for,render_template,Response
from image_database import ImageDB
import os
from flask import send_file
from flask import make_response
from flask import request
import random

app = Flask(__name__)

app.config.from_object('config')
print app.config['DIR_FILE']
print app.config['PICS_FILE']
print app.config['IP_ADDRESS']
print app.config['WEB_PORT']

image_db = ImageDB()
image_db.DIR_FILE = app.config['DIR_FILE']
image_db.PICS_FILE = app.config['PICS_FILE']
image_db.gen_pics_config_file()


@app.route('/')
def hello_world():
    print "hello world"
    # return redirect(url_for('show_images', idx=0))
    # return redirect(url_for('hello',idx=0))
    # return redirect(url_for('get_local_img'))
    return redirect(url_for('show_images_local', idx=0))


@app.route('/hello2/<int:idx>/')
def hello(idx):
    return 'Hello %d' % (idx + 1)


@app.route('/get_local_img/')
def get_local_img():
    print "aa"
    local_img = request.args.get('file')
    print "text %s" % local_img
    # return "text %s" % local_img
    # file_path = 'F:/pictures/2017/IMG_20170128_153405.jpg'
    # file_path = 'F:/pictures/2017/111.jpg'
    if os.path.exists(local_img):
        file_path = local_img
    else:
        file_path = 'static/images/pic_oops.jpg'
    if os.path.exists(file_path):
        print " file path %s" % file_path
        # resp = make_response(send_file(file_path, mimetype='image/jpg'))
        resp = make_response(send_file(file_path))
        return resp
    else:
        print " wrong file path %s" % file_path
        return "no file"
    return "final"


@app.route('/images_local/<int:idx>/')
def show_images_local(idx):
    print "cc"
    if idx == 0:
        cur_file = 'static/images/pic_loading.jpg'
    elif idx == -1:
        cur_file = 'static/images/pic_oops.jpg'
    else:
        if app.config['RANDOM_SET'] == 1:
            idx = random.randint(1, image_db.total_pics)

        line, cur_file=image_db.read_uri(idx)
        print "uri file %s" % cur_file
        if line != idx:
            idx = line;

    cur_file = cur_file.replace('\\', '/')
    print "cur_file %s" % cur_file
    next_url = url_for('show_images_local',idx=idx+1)

    pic = 'http://%s:%d/get_local_img/?file=%s' % (app.config['IP_ADDRESS'], app.config['WEB_PORT'], cur_file)
    print "pic %s" % pic
    title_file =  os.path.basename(cur_file)
    return render_template("base.html", pic=pic, next_url=next_url, title_file=title_file)


@app.route('/images/<int:idx>/')
def show_images(idx):
    print "bb"
    if idx == 0:
        cur_file = 'static/images/pic_loading.jpg'
    elif idx == -1:
        cur_file = 'static/images/pic_oops.jpg'
    else:
        line, cur_file=image_db.read_uri(idx)
        print "uri file %s" % cur_file
        if line != idx:
            idx = line;
            #cur_file = 'static/images/pic_oops.jpg'

    cur_file = cur_file[7:].replace('\\', '/')
    print "cur_file %s" % cur_file
    pic = url_for('static', filename=cur_file)
    next_url=url_for('show_images',idx=idx+1)
    print "pci %s" % pic
    return render_template("base.html", pic=pic, next_url=next_url)


if __name__ == '__main__':
    # app.run(debug=True)
    # app.run(host=app.config['IP_ADDRESS'], port=app.config['WEB_PORT'])
    app.run(host=app.config['IP_ADDRESS'], port=app.config['WEB_PORT'], debug=True)
