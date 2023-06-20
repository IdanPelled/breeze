var socket;

/**
 * Sends data to the server using the specified event name.
 * @param {string} eventName - The name of the event to emit.
 * @param {any} data - The data to send along with the event.
 * @returns {void}
 */
const send = (eventName, data) => {
    const dataWithToken = {
        token: document.cookie,
        payload: data
    };

    socket.emit(eventName, dataWithToken);
};

/**
 * Enum for different output levels.
 */
const Level = {
    info: 'info',
    error: 'error',
    success: 'success'
};

/**
 * Object that contains functions to handle different log levels.
 */
const Print = {
    [Level.info]: (output) => {
        AddOutput(output, "black");
    },
    [Level.error]: (output) => {
        AddOutput(output, "red");
    },
    [Level.success]: (output) => {
        AddOutput(output, "green");
    }
};

/**
 * Sends the code to the server and sets up event listeners for output, input requests, program end, and errors.
 * @returns {void}
 */
function SendCode() {
    StartProgram();
    socket = io();

    socket.on('output', (output) => {
        console.info('output ' + output);
        Print[Level.info](output);
    });

    socket.on('input-request', (message) => {
        console.info('input-request ' + message);
        ShowInput(message);
    });

    socket.on('end-program', (output) => {
        console.info('end-program ' + output);
        EndProgram(output);
    });

    socket.on('error', (output) => {
        console.info('error ' + output);
        Print[Level.error](output);
    });

    console.info('connected');
}

/**
 * Starts the program by sending the code to the server.
 * @returns {void}
 */
function StartProgram() {
    var code = codeEditor.getValue();
    var url = window.location.origin + "/execute";

    fetch(url, {
        method: 'POST',
        body: JSON.stringify({ code: code }),
        headers: {
            'Content-Type': 'application/json'
        },
    });

    ToggleButton("run", true);
    ToggleButton("stop", false);
    ToggleVisibility("loading", "inline-block");
}

/**
 * Toggles the disabled state of a button.
 * @param {string} name - The name or ID of the button.
 * @param {boolean} state - The disabled state of the button.
 * @returns {void}
 */
function ToggleButton(name, state) {
    document.getElementById(name).disabled = state;
}

/**
 * Toggles the visibility of an element.
 * @param {string} name - The name or ID of the element.
 * @param {string} mode - The display mode to set (e.g., "none", "block", "inline-block").
 * @returns {void}
 */
function ToggleVisibility(name, mode) {
    document.getElementById(name).style.display = mode;
}

/**
 * Handles the program end event and prints a success message if the output is "success".
 * @param {string} output - The output received from the server.
 * @returns {void}
 */
function EndProgram(output) {
    if (output == "success") {
        Print[Level.success]("Program completed :)");
    }
    Stop();
}

/**
 * Stops the program execution by emitting a "stop" event to the server, deleting the execution token cookie, and closing the socket connection.
 * @returns {void}
 */
 function Stop() {
    socket.emit("stop", document.cookie);
    DeleteCookie("execution_token");

    ToggleButton("run", false);
    ToggleButton("stop", true);
    ToggleVisibility("loading", "none");
    ToggleVisibility("input-frame", "none");

    socket.close();
}

/**
 * Displays the input request message on the page.
 * @param {string} message - The input request message received from the server.
 * @returns {void}
 */
function ShowInput(message) {
    console.info(message);
    document.getElementById("input-msg").textContent = message;
    ToggleVisibility("input-frame", "block");
}

/**
 * Hides the input frame after receiving the user input and displays the input in the output section.
 * @param {string} input - The user input.
 * @returns {void}
 */
function HideInput(input) {
    const msg = document.getElementById("input-msg").textContent + ": " + input;

    document.getElementById("input-msg").textContent = "";
    ToggleVisibility("input-frame", "none");

    Print[Level.info](msg);
}

/**
 * Adds output content to the output section.
 * @param {string} content - The output content to be added.
 * @param {string} color - The color of the output text.
 * @returns {void}
 */
function AddOutput(content, color) {
    var paragraph = document.createElement("p");
    const out = document.getElementById("output");

    paragraph.textContent = content;
    paragraph.style.color = color;
    out.appendChild(paragraph);
    out.scrollTop = out.scrollHeight;
}

/**
 * Deletes a cookie by setting its expiration date to the past.
 * @param {string} cookieName - The name of the cookie to be deleted.
 * @returns {void}
 */
function DeleteCookie(cookieName) {
    document.cookie = cookieName + "=; expires=Thu, 01 Jan 1970 00:00:00 UTC; path=/;";
}

/**
 * Handles key press events, specifically the Enter key press, for the input element.
 * @param {object} event - The key press event object.
 * @param {object} element - The input element.
 * @returns {void}
 */
function HandleKeyPress(event, element) {
    // Check for Enter key press without Shift key
    if (event.keyCode === 13 && !event.shiftKey) {
        if (socket && socket.connected) {
            event.preventDefault();
            send("input-response", element.value);
            HideInput(element.value);
        }

        element.value = "";
    }
}
