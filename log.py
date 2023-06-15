import logging

class SocketIOFilter(logging.Filter):
    def filter(self, record):
        # Filter out SocketIO logs
        return "/socket.io/?EIO" not in record.getMessage()


log = logging.getLogger('werkzeug')
log.addFilter(SocketIOFilter())