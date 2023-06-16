import subprocess
import time
from typing import Generator, Tuple

from factory import connections, socketio


EXE_PATH = './interpreter/bin/breeze'
INPUT_MESSAGE = "~<INPUT MESSAGE>~"


def pass_input(process: subprocess.Popen, data: str) -> None:
    process.stdin.write(data + '\n')
    process.stdin.flush()


def run_code(code: str, token: str) -> Generator[Tuple[str, str], None, bool]:
    process = subprocess.Popen(
        [EXE_PATH, code],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        universal_newlines=True,
    )

    connections.update({token: process})
    return_statuc_code = process.poll()
    flag = True

    while flag:
        output = process.stdout.readline()
        error = process.stderr.readline()
        
        if output or error:
            yield output, error

        return_statuc_code = process.poll()
        flag = output != '' or error != '' or process.poll() is None
    
    yield None, None
    if token in connections:
        del connections[token]
    yield return_statuc_code == 0


def handle_out(message: str):
    if (message.startswith(INPUT_MESSAGE)):
        socketio.emit("input-request", message.replace(INPUT_MESSAGE, ""))
    
    else:
        socketio.emit("output", message)


def handle_error(message: str):
    socketio.emit("error", message)


def handle_end_program(success: bool):
    if success:
        socketio.emit("end-program", "success")
    
    else:
        socketio.emit("end-program", "error")


def execute_code(code, execution_token):
    time.sleep(0.1)
    generator = run_code(code, execution_token)
    for out, error in generator:
        
        if out or error:
            if out:
                handle_out(out)

            if error:
                handle_error(error)
            
        else:
            handle_end_program(next(generator))
