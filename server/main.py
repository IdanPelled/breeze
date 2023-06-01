from routes import routes_app
from websocket import app, socketio


app.register_blueprint(routes_app)

if __name__ == '__main__':
    socketio.run(app)
