import subprocess
from typing import Generator

from . import running, socketio

EXE_PATH = '../interpreter/bin/breeze'

def pass_input(process: subprocess.Popen, data: str) -> None:
    process.stdin.write((data + '\n').encode())
    process.stdin.flush()


def read_output(process: subprocess.Popen) -> str:
    return process.stdout.readline().decode()


def run_code(code: str, token: str) -> Generator[str, None, bool]:
    process = subprocess.Popen(
        [EXE_PATH, code],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    )
    
    running.update({token: process.pid})
    return_statuc_code = process.poll()
    while return_statuc_code is None:
        out = read_output(process)

        if out:
            yield out

        return_statuc_code = process.poll()
    
    yield None
    del running[token]
    yield return_statuc_code == 0

def execute_code(code, execution_token):
    print("executing code")
    generator = run_code(code, execution_token)
    for out in generator:
        if out:
            socketio.emit("output", out)
        
        else:
            success = next(generator)
            if success:
                socketio.emit("end-proggram", "Proggram completed :)")
            else:
                socketio.emit("end-proggram", "Error occurred :(")