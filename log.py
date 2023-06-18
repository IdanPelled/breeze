from flask import Flask
import logging


class SocketIOFilter(logging.Filter):
    """
    Custom logging filter to exclude SocketIO logs.

    Methods:
        filter(record): Filters out SocketIO logs from the given log record.

    Attributes:
        None
    """

    def filter(self, record):
        """
        Filters out SocketIO logs from the given log record.

        Args:
            record (LogRecord): The log record to be filtered.

        Returns:
            bool: True if the log record should be included, False if it should be excluded.
        """

        return "/socket.io/?EIO" not in record.getMessage()


def setup_logger(app: Flask):
    """
    Sets up the logger configuration for the Flask app.

    Args:
        app (Flask): The Flask application instance.

    Returns:
        logging.Logger: The configured logger instance.
    """

    logging.basicConfig(level=logging.INFO)
    log = logging.StreamHandler()
    log.setLevel(logging.INFO)

    formatter = logging.Formatter('%(asctime)s - %(levelname)s - %(message)s [%(module)s:%(funcName)s:%(lineno)d]')
    log.setFormatter(formatter)

    logger = logging.getLogger(__name__)
    logger.addHandler(log)

    werkzeug_log = logging.getLogger('werkzeug')
    werkzeug_log.addFilter(SocketIOFilter())
    werkzeug_log.addHandler(log)
    
    app.logger.addHandler(logger)
    app.logger.addHandler(werkzeug_log)

    return logger
