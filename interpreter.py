import subprocess
import time
from typing import Generator

from factory import connections, socketio


EXE_PATH = './interpreter/bin/breeze'
INPUT_MESSAGE = "~<INPUT MESSAGE>~"


def pass_input(process: subprocess.Popen, data: str) -> None:
    process.stdin.write(data + '\n')
    process.stdin.flush()


def run_code(code: str, token: str) -> Generator[str, None, bool]:
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
        
        if output:
            yield output

        return_statuc_code = process.poll()
        flag = output != '' or process.poll() is None
    
    yield None
    if token in connections:
        del connections[token]
    yield return_statuc_code == 0


def execute_code(code, execution_token):
    time.sleep(0.1)
    generator = run_code(code, execution_token)
    for out in generator:
        if out:
            
            if (out.startswith(INPUT_MESSAGE)):
                socketio.emit("input-request", out.replace(INPUT_MESSAGE, ""))
            
            else:
                socketio.emit("output", out)
        
        else:
            success = next(generator)
            if success:
                socketio.emit("end-program", "Program completed :)")
            
            else:
                socketio.emit("end-program", "Error occurred :(")
