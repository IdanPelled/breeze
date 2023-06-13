var socket;

const send = (eventName, data) => {
    const dataWithToken = {
      token: document.cookie,
      payload: data
    };
    
    socket.emit(eventName, dataWithToken);
};
  

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
    socket = io();
    
    socket.on('output', (output) => {
        AddOutput(output);
    });

    socket.on('end-program', (output) => {
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

function HandleKeyPress(event, element) {
    // check for entry press
    if (event.keyCode === 13 && !event.shiftKey) {
        if (socket && socket.connected) {
            console.info("!@#", socket)
            event.preventDefault();
            console.info(element.value);
            send("input", element.value);
            AddOutput(element.value);
        }
        else {
            AddOutput(
                "You can send input only when the the program is"
                + "listenign for input. for more information read"
                + "about the `in` Value function."
            );
        }

        element.value = "";
        
    }
}