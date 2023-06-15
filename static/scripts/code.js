// Define custom mode for breeze
CodeMirror.defineSimpleMode("pseudo", {
    start: [
        // Rules for highlighting pseudo code
        { regex: /"(?:[^\\]|\\.)*?(?:"|$)/, token: "string" },
        { regex: /[0-9]+/, token: "number" },
        { regex: /(?:no|yes)\b/, token: "atom" },
        { regex: /(?:out|in|to-number|to-text)\b/, token: "def" },
        { regex: /(?:set|to|when|do|otherwise|loop|times)\b/, token: "keyword" },
    ],
    meta: {
        lineComment: "//",
    },
});

function AdjustHeight(element) {
    element.style.height = "auto";
    element.style.height = element.scrollHeight + "px";
}

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

