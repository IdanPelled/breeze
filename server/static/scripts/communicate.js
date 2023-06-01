
const socket = io.connect();

socket.on('output', function(output) {
    AddOutput(output);
});


function SendCode() {

    var code = codeEditor.getValue();
    var url = window.location.origin + "/execute";

    fetch(url, {
        method: 'POST',
        body: JSON.stringify({code: code}),
        headers: {
            'Content-Type': 'application/json'
        },
    })
    .then(response => response.text())
    .then(data => AddOutput(data))
    .catch(error => {
        console.error('Error:', error);
    });
}

function ClearOutput() {
    var element = document.getElementById("output");
    element.innerHTML = "";
}

function AddOutput(content) {
    var paragraph = document.createElement("p");
    paragraph.textContent = content;
    document.getElementById("output").appendChild(paragraph);
}