import subprocess
from typing import Generator


def pass_input(process: subprocess.Popen, data: str) -> None:
    process.stdin.write((data + '\n').encode())
    process.stdin.flush()


def read_output(process: subprocess.Popen) -> str:
    return process.stdout.readline().decode()


def run_code(code: str) -> Generator[str, None, bool]:
    process = subprocess.Popen(
        ['../interpreter/bin/breeze', code],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    )
    
    return_statuc_code = process.poll()
    while return_statuc_code is None:
        out = read_output(process)

        if out:
            yield out

        return_statuc_code = process.poll()
        print(return_statuc_code)
    
    yield None
    print("status code ", return_statuc_code)
    yield return_statuc_code == 0