import os
import zipfile
import secrets
import threading
from flask import request, render_template, Blueprint, make_response, send_file

from breeze.interpreter import execute_code


routes_app = Blueprint('routes', __name__)


@routes_app.route('/editor', methods=['GET'])
def editor():
    return render_template("editor.html")


@routes_app.route('/', methods=['GET'])
def home():
    return render_template("index.html")


@routes_app.route('/execute', methods=['POST'])
def execute():
    code = request.json.get("code")

    execution_token = hex(secrets.randbelow(2 ** 1000))
    execution_thread = threading.Thread(
        target=execute_code, args=(code, execution_token)
    )
    execution_thread.start()
    
    response = make_response()
    response.set_cookie('execution_token', execution_token)
    return response



@routes_app.route('/download', methods=['GET'])
def download_breeze():
    folder_path = './interpreter/release/breeze.zip'

    return send_file(folder_path, as_attachment=True)