from factory import create_app, socketio
from log import log

app = create_app()
app.logger.addHandler(log)
socketio.run(app)