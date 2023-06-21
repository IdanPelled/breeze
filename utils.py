import hashlib
from logging import getLogger
from flask import request

from factory import connections

log = getLogger(__name__)

def remove_token_prefix(token):
    """
    Removes the prefix 'execution_token=' from the given token string.

    Args:
        token (str): The token string.

    Returns:
        str: The token string without the prefix, or an empty string if the prefix is not present.
    """

    prefix = "execution_token="
    
    if token.startswith(prefix):
        return token[len(prefix):]
    
    else:
        log.info("Auth Error: invalid token - missing prifix")
        return ""


def predicate(func):
    """
    Decorator function that performs authentication and executes the decorated function.

    Args:
        func (function): The function to be decorated.

    Returns:
        function: The decorated function.
    """
    
    def decorator(data):
        plain_token = data.get("token")
        if not plain_token:
            log.info("Auth Error: missing token")
            return

        token = remove_token_prefix(plain_token)
        if (token != hash_ip(request.remote_addr)):
            log.info("Auth Error: invalid token - potential of session hijacking")
            return

        process = connections.get(token)
        if not process:
            log.info("Auth Error: invalid token - unregistered token")
            return
        
        return func(process, data["payload"])

    return decorator


def hash_ip(ip: str) -> str:
    """Hashes the given IP address using SHA256 algorithm.

    Args:
        ip (str): The IP address to hash.

    Returns:
        str: The hashed IP address in hexadecimal format.
    """
    
    sha256_hash = hashlib.sha256()
    sha256_hash.update(ip.encode('utf-8'))
    return sha256_hash.hexdigest()
