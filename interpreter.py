from logging import getLogger
import subprocess
import time
import threading
from queue import Queue
import io

from factory import connections, socketio


EXE_PATH = './interpreter/bin/breeze'
INPUT_MESSAGE = "~<INPUT MESSAGE>~"

log = getLogger(__name__)

def read_output(proc: subprocess.Popen, messages: Queue, terminate_flag: threading.Event) -> None:
    """
    Reads the output from the subprocess and puts each line into the messages queue.

    Args:
        proc (subprocess.Popen): The subprocess object.
        messages (Queue): The queue to store the output messages.
        terminate_flag (threading.Event): a terminate event signal
    """

    with io.TextIOWrapper(proc.stdout, encoding='utf-8', line_buffering=True) as stdout_wrapper:
        for line in iter(stdout_wrapper.readline, ''):
            
            if terminate_flag.is_set():
                return None
            
            messages.put(line.rstrip())


def read_error(proc: subprocess.Popen, errors: Queue, terminate_flag: threading.Event) -> None:
    """
    Reads the error from the subprocess and puts each line into the errors queue.

    Args:
        proc (subprocess.Popen): The subprocess object.
        errors (Queue): The queue to store the error errors.
        terminate_flag (threading.Event): a terminate event signal
    """

    with io.TextIOWrapper(proc.stderr, encoding='utf-8', line_buffering=True) as stderr_wrapper:
        for line in iter(stderr_wrapper.readline, ''):
            
            if terminate_flag.is_set():
                return None
            
            errors.put(line.rstrip())


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


def handle_end(proc: subprocess.Popen, errors: Queue):
    """
    Handles the end of the subprocess execution.

    Args:
        proc (subprocess.Popen): The subprocess object.
        error (Queue): The error queue
    """

    if proc.returncode == 0:
        socketio.emit("end-program", "success")
    
    else:
        if not errors.empty():
            socketio.emit("error", errors.get(block=False))
        
        else:
            log.error("Missing error description for status code", proc.returncode)
        
        socketio.emit("end-program", "error")



def execute_code(code: str, token: str, terminate_flag: threading.Event) -> None:
    """
    Executes the provided code using a subprocess.

    Args:
        code (str): The code to be executed.
        token (str): A token to identify the execution.
        terminate_flag (threading.Event): a terminate event signal

    """

    proc = subprocess.Popen(
        [EXE_PATH, code],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        stdin=subprocess.PIPE,
    )

    connections.update({token: proc})
    
    messages = Queue()
    errors = Queue()

    output_thread = threading.Thread(target=read_output, args=(proc, messages, terminate_flag))
    error_thread = threading.Thread(target=read_error, args=(proc, errors, terminate_flag))
    output_thread.start()
    error_thread.start()
    
    time.sleep(0.5)
    while errors.empty() and (proc.poll() is None or not messages.empty()):
        
        if not messages.empty():
            handle_message(messages.get(block=False))
        
        time.sleep(0.05)


    if token in connections:
        del connections[token]

    handle_end(proc, errors)
    terminate_flag.set()
    output_thread.join()
    error_thread.join()


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
