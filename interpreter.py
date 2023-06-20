import select
import subprocess
import time
from typing import Generator, Tuple

from factory import connections, socketio


EXE_PATH = './interpreter/bin/breeze'
INPUT_MESSAGE = "~<INPUT MESSAGE>~"


def pass_input(process: subprocess.Popen, data: str) -> None:
    """
    Passes the input data to the subprocess.

    Args:
        process (subprocess.Popen): The subprocess to pass the input to.
        data (str): The input data to be passed.

    Returns:
        None
    """

    process.stdin.write(data + '\n')
    process.stdin.flush()


def run_code(code: str, token: str) -> Generator[Tuple[str, str], None, bool]:
    """
    Runs the code using the interpreter subprocess.

    Args:
        code (str): The code to be executed.
        token (str): The token associated with the code execution.

    Yields:
        Tuple[str, str]: The output and error messages yielded during the code execution.
        bool: The final success status of the code execution.
    """

    process = subprocess.Popen(
        [EXE_PATH, code],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        universal_newlines=True,
    )

    connections.update({token: process})

    while process.poll() is None:
        # Check if there is output available to read
        readable, _, _ = select.select([process.stdout, process.stderr], [], [], 0)

        for stream in readable:
            if stream is process.stdout:
                output = stream.readline()
                yield output, None

            elif stream is process.stderr:
                error = stream.readline()
                yield None, error
        

    # Read any remaining output after process completion
    output = process.stdout.read()
    error = process.stderr.read()
    
    if output or error:
        yield output, error
    
    yield None, None
    
    if token in connections:
        del connections[token]
    
    yield process.returncode == 0


def handle_out(message: str):
    """
    Handles the output message received from the interpreter subprocess.

    Args:
        message (str): The output message.

    Returns:
        None
    """

    if (message.startswith(INPUT_MESSAGE)):
        socketio.emit("input-request", message.replace(INPUT_MESSAGE, ""))
    
    else:
        socketio.emit("output", message)


def handle_error(message: str):
    """
    Handles the error message received from the interpreter subprocess.

    Args:
        message (str): The error message.

    Returns:
        None
    """

    socketio.emit("error", message)


def handle_end_program(success: bool):
    """
    Handles the end of the program execution.

    Args:
        success (bool): The success status of the program execution.

    Returns:
        None
    """

    if success:
        socketio.emit("end-program", "success")
    
    else:
        socketio.emit("end-program", "error")


def execute_code(code, execution_token):
    """
    Executes the code and sends the output and error messages via SocketIO.

    Args:
        code (str): The code to be executed.
        execution_token (str): The token associated with the code execution.

    Returns:
        None
    """
    
    time.sleep(0.5)
    generator = run_code(code, execution_token)
    for out, error in generator:
        
        if out or error:
            if out:
                handle_out(out)

            if error:
                handle_error(error)
            
        else:
            return handle_end_program(next(generator))
