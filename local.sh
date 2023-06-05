# Build the interpreter
cd ./interpreter
make
cd ..

# Run the server
cd ./server
pip install -r requirements.txt
export FLASK_APP=main.py
export FLASK_DEBUG=1 
export TEMPLATES_AUTO_RELOAD=1
flask run 