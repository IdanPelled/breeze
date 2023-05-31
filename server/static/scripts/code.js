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