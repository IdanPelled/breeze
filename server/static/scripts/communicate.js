function SendCode() {

    var code = codeEditor.getValue();
    var url = window.location.origin + "/execute";

    fetch(url, {
        method: 'POST',
        body: JSON.stringify({code: code}),
        headers: {
            'Content-Type': 'application/json'
        },
    });
    const socket = io();

    socket.on('output', (output) => {
        AddOutput(output);
    });

    socket.on('end-proggram', (output) => {
        AddOutput(output);
        DeleteCookie("execution_token");
        socket.close();
    });
}

function AddOutput(content) {
    var paragraph = document.createElement("p");
    const out = document.getElementById("output")
    
    paragraph.textContent = content;
    out.appendChild(paragraph);
    out.scrollTop = out.scrollHeight;
}


function DeleteCookie(cookieName) {
    document.cookie = cookieName + "=; expires=Thu, 01 Jan 1970 00:00:00 UTC; path=/;";
}