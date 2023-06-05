// Define custom mode for breeze
CodeMirror.defineSimpleMode("pseudo", {
    start: [
        // Rules for highlighting pseudo code
        { regex: /(?:if|else)\b/, token: "keyword" },
        { regex: /"(?:[^\\]|\\.)*?(?:"|$)/, token: "string" },
        { regex: /[0-9]+/, token: "number" },
    ],
    meta: {
        lineComment: "//",
    },
});

// Initialize CodeMirror with the custom mode
var codeEditor = CodeMirror.fromTextArea(document.getElementById("code"), {
    mode: "pseudo",
    lineNumbers: true,
});

function ClearOutput() {
    var element = document.getElementById("output");
    element.innerHTML = "";
}

function FocusOnInput() {
    const input = document.getElementById('input');
    input.focus();
}

function AdjustHeight(element) {
    element.style.height = "auto";
    element.style.height = element.scrollHeight + "px";
}

function HandleKeyPress(event, element) {
    // check for entry press
    if (event.keyCode === 13 && !event.shiftKey) {
        event.preventDefault();
        socket.emit("input", element.value);
        AddOutput(element.value);
        element.value = "";
    }
}