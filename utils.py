from . import connections


def remove_token_prefix(token):
    prefix = "execution_token="
    
    if token.startswith(prefix):
        return token[len(prefix):]
    
    else:
        return ""

def predicate(func):
    
    def decorator(data):
        plain_token = data.get("token")
        
        if not plain_token:
            print("Token auth error")
            return
        
        process = connections.get(remove_token_prefix(plain_token))
        return func(process, data["payload"])

    return decorator