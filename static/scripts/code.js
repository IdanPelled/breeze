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

function downloadFile() {
    var code = codeEditor.getValue();
    var filename = 'code.brz';

    var blob = new Blob([code], { type: 'text/plain' });
    var url = URL.createObjectURL(blob);

    var link = document.createElement('a');
    link.href = url;
    link.download = filename;
    link.click();

    // Clean up the temporary URL
    URL.revokeObjectURL(url);
}

function openFile(event) {
    var file = event.target.files[0];
    var reader = new FileReader();

    reader.onload = function(e) {
        var content = e.target.result;
        codeEditor.setValue(content);
    };

    reader.readAsText(file);
}