from factory import connections

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
            print("Auth Error: missing token")
            return
        
        process = connections.get(remove_token_prefix(plain_token))
        if not process:
            print("Auth Error: invalid token")
            return
        
        return func(process, data["payload"])

    return decorator
