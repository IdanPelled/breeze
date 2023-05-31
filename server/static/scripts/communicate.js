
const socket = io.connect();

socket.on('output', function(output) {
    add_output(output);
});


function SendCode() {

    var code = codeEditor.getValue();
    var url = window.location.origin + "/execute";

    clear_output();
    fetch(url, {
        method: 'POST',
        body: JSON.stringify({code: code}),
        headers: {
            'Content-Type': 'application/json'
        },
    })
    .then(response => response.text())
    .then(data => add_output(data))
    .catch(error => {
        console.error('Error:', error);
    });
}

function clear_output(content) {
    var element = document.getElementById("output");
    element.innerHTML = "";
}

function add_output(content) {
    var paragraph = document.createElement("p");
    paragraph.textContent = content;
    document.getElementById("output").appendChild(paragraph);
}