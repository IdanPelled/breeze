# Build the interpreter
cd ./interpreter
make
cd ..

# Run the server
cd ./server
pip install -r requirements.txt
export FLASK_APP=main.py
flask run -p 9090