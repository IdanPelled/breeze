import subprocess
import time
from typing import Generator, Tuple
import threading
from queue import Queue
import io

from factory import connections, socketio


EXE_PATH = './interpreter/bin/breeze'
INPUT_MESSAGE = "~<INPUT MESSAGE>~"


def read_output(proc: subprocess.Popen, messages: Queue) -> None:
    """
    Reads the output from the subprocess and puts each line into the messages queue.

    Args:
        proc (subprocess.Popen): The subprocess object.
        messages (Queue): The queue to store the output messages.
    """

    with io.TextIOWrapper(proc.stdout, encoding='utf-8', line_buffering=True) as stdout_wrapper:
        for line in iter(stdout_wrapper.readline, ''):
            messages.put(line.rstrip())


def handle_message(message: str):
    """
    Handles each message received from the subprocess.

    Args:
        message (str): The message received from the subprocess.
    """

    if message.startswith(INPUT_MESSAGE):
        input_prompt = message.replace(INPUT_MESSAGE, "")
        socketio.emit("input-request", input_prompt)
    
    else:
        socketio.emit("output", message)


def handle_end(proc: subprocess.Popen):
    """
    Handles the end of the subprocess execution.

    Args:
        proc (subprocess.Popen): The subprocess object.
    """

    if proc.returncode == 0:
        socketio.emit("end-program", "success")
    
    else:
        socketio.emit("error", proc.stderr.readline().decode('utf-8'))
        socketio.emit("end-program", "error")


def execute_code(code: str, token: str) -> None:
    """
    Executes the provided code using a subprocess.

    Args:
        code (str): The code to be executed.
        token (str): A token to identify the execution.

    """

    time.sleep(0.5)
    proc = subprocess.Popen(
        [EXE_PATH, code],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        stdin=subprocess.PIPE,
    )

    connections.update({token: proc})
    messages = Queue()

    output_thread = threading.Thread(target=read_output, args=(proc, messages))
    output_thread.start()

    while proc.poll() is None or not messages.empty():
        handle_message(messages.get())
        time.sleep(0.05)

    if token in connections:
        del connections[token]

    handle_end(proc)
    output_thread.join()


def pass_input(process: subprocess.Popen, data: str) -> None:
    """
    Passes input to the subprocess.

    Args:
        process (subprocess.Popen): The subprocess object.
        data (str): The input data to be passed.

    Note:
        The subprocess should be waiting for an input prompt.
    """
    process.stdin.write((data + '\n').encode('utf-8'))
    process.stdin.flush()
