from flask import Flask
import logging


class SocketIOFilter(logging.Filter):
    def filter(self, record):
        # Filter out SocketIO logs
        return "/socket.io/?EIO" not in record.getMessage()


def setup_logger(app: Flask):
    logging.basicConfig(level=logging.INFO)
    log = logging.StreamHandler()
    log.setLevel(logging.INFO)

    formatter = logging.Formatter('%(asctime)s - %(levelname)s - %(message)s [%(module)s:%(funcName)s:%(lineno)d]')
    log.setFormatter(formatter)

    # Create and configure the logger
    logger = logging.getLogger(__name__)
    logger.addHandler(log)

    # Add the werkzeug_log handler to the Flask app logger
    werkzeug_log = logging.getLogger('werkzeug')
    werkzeug_log.addFilter(SocketIOFilter())
    werkzeug_log.addHandler(log)
    
    app.logger.addHandler(logger)
    app.logger.addHandler(werkzeug_log)

    return logger
