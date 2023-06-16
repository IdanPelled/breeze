var socket;

const send = (eventName, data) => {
    const dataWithToken = {
      token: document.cookie,
      payload: data
    };
    
    socket.emit(eventName, dataWithToken);
};

const Level = {
    info: 'info',
    error: 'error',
    sucess: 'sucess'
  };

const Print = {
    [Level.info]: (output) => {
        AddOutput(output, "black")
    },
    [Level.error]: (output) => {
        AddOutput(output, "red")
    },
    [Level.sucess]: (output) => {
        AddOutput(output, "green")
    }
};

function SendCode() {
    StartProgram();
    socket = io();
    
    socket.on('output', (output) => {
        Print[Level.info](output);
    });

    socket.on('input-request', (message) => {
        ShowInput(message);
    });

    socket.on('end-program', (output) => {
        EndProgram(output);
    });

    socket.on('error', (output) => {
        Print[Level.error](output);
    });
}

function Connect()
{
    
}

function StartProgram()
{
    var code = codeEditor.getValue();
    var url = window.location.origin + "/execute";

    fetch(url, {
        method: 'POST',
        body: JSON.stringify({code: code}),
        headers: {
            'Content-Type': 'application/json'
        },
    });

    ToggleButton("run", true);
    ToggleButton("stop", false);
    ToggleVisability("loading", "inline-block");
}

function ToggleButton(name, state)
{
    document.getElementById(name).disabled = state;
}

function ToggleVisability(name, mode)
{
    document.getElementById(name).style.display = mode;
}

function EndProgram(output)
{
    if (output == "success")
        Print[Level.sucess]("Program completed :)");
    Stop();
}

function Stop() {
    socket.emit("stop", document.cookie);
    DeleteCookie("execution_token");

    ToggleButton("run", false);
    ToggleButton("stop", true);
    ToggleVisability("loading", "none");
    ToggleVisability("input-frame", "none");
    
    socket.close();
}

function ShowInput(message)
{
    console.info(message);
    document.getElementById("input-msg").textContent = message;
    ToggleVisability("input-frame", "block");
}

function HideInput(input)
{
    msg = (
        document.getElementById("input-msg").textContent
        + ": " + input
    )
    
    document.getElementById("input-msg").textContent = "";
    ToggleVisability("input-frame", "none");

    Print[Level.info](output);
}

function AddOutput(content, color)
{
    var paragraph = document.createElement("p");
    const out = document.getElementById("output")
    
    paragraph.textContent = content;
    paragraph.style.color = color;
    out.appendChild(paragraph);
    out.scrollTop = out.scrollHeight;
}

function DeleteCookie(cookieName)
{
    document.cookie = cookieName + "=; expires=Thu, 01 Jan 1970 00:00:00 UTC; path=/;";
}

function HandleKeyPress(event, element)
{
    // check for entry press
    if (event.keyCode === 13 && !event.shiftKey) {
        if (socket && socket.connected) {
            event.preventDefault();
            send("input-response", element.value);
            HideInput(element.value)
        }
        
        element.value = "";
    }
}