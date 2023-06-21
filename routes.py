import secrets
import threading
from flask import request, render_template, Blueprint, make_response, send_file

from interpreter import execute_code
from utils import hash_ip


routes_app = Blueprint('routes', __name__)


@routes_app.route('/editor', methods=['GET'])
def editor():
    """
    Renders the editor.html template.

    Returns:
        The rendered editor.html template.
    """

    return render_template("editor.html")


@routes_app.route('/documentation', methods=['GET'])
def documentation():
    """
    Renders the documentation.html template.

    Returns:
        The rendered documentation.html template.
    """

    return render_template("documentation.html")


@routes_app.route('/', methods=['GET'])
def index():
    """
    Renders the index.html template.

    Returns:
        The rendered index.html template.
    """

    return render_template("index.html")


@routes_app.route('/execute', methods=['POST'])
def execute():
    """
    Executes the provided code asynchronously and returns an HTTP response.

    Returns:
        The HTTP response indicating the status of the code execution.
    """
    
    code = request.json.get("code")
    response = make_response()

    if request.cookies.get('execution_token') is None:
       
        execution_token = hash_ip(request.remote_addr)
        execution_thread = threading.Thread(
            target=execute_code,
            args=(code, execution_token)
        )
        execution_thread.start()

        response.set_cookie('execution_token', execution_token)
    
    else:
        # Too Many Requests Status Code
        response.status_code = 429
    
    return response


@routes_app.route('/download', methods=['GET'])
def download_breeze():
    """
    Downloads the breeze.zip file.

    Returns:
        The breeze.zip file as an attachment for download.
    """

    folder_path = './interpreter/release/breeze.zip'
    return send_file(folder_path, as_attachment=True)
