// Define custom mode for breeze
CodeMirror.defineSimpleMode("pseudo", {
    start: [
        // Rules for highlighting pseudo code
        { regex: /"(?:[^\\]|\\.)*?(?:"|$)/, token: "string" },
        { regex: /[0-9]+/, token: "number" },
        { regex: /\/\/.*/, token: "comment" }, // Match comments starting with "//"
        { regex: /(?:no|yes)\b/, token: "atom" },
        { regex: /(?:out|in|to-number|to-text)\b/, token: "def" },
        { regex: /(?:set|to|when|do|otherwise|loop|times)\b/, token: "keyword" },
        { regex: /[><=+\-*/]/, token: "operator" },
        { regex: /[\{\}\(\)]/, token: "punctuation" },
        { regex: /[^\s]+/, token: null }, // Match any remaining characters as null token
    ],
    meta: {
        lineComment: "//",
    },
});

/**
 * Adjusts the height of the element to fit its content.
 * @param {HTMLElement} element - The element to adjust the height of.
 * @returns {void}
 */
function AdjustHeight(element) {
    element.style.height = "auto";
    element.style.height = element.scrollHeight + "px";
}

// Initialize CodeMirror with the custom mode
var codeEditor = CodeMirror.fromTextArea(document.getElementById("code"), {
    mode: "pseudo",
    lineNumbers: true,
});

/**
 * Clears the output element.
 * @returns {void}
 */
function ClearOutput() {
    var element = document.getElementById("output");
    element.innerHTML = "";
}

/**
 * Sets focus on the input element.
 * @returns {void}
 */
function FocusOnInput() {
    const input = document.getElementById('input');
    input.focus();
}

/**
 * Adjusts the height of the element to fit its content.
 * @param {HTMLElement} element - The element to adjust the height of.
 * @returns {void}
 */
function AdjustHeight(element) {
    element.style.height = "auto";
    element.style.height = element.scrollHeight + "px";
}

/**
 * Downloads the code as a file.
 * @returns {void}
 */
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

/**
 * Opens a file and sets its content in the code editor.
 * @param {Event} event - The file input change event.
 * @returns {void}
 */
function openFile(event) {
    var file = event.target.files[0];
    var reader = new FileReader();

    reader.onload = function(e) {
        var content = e.target.result;
        codeEditor.setValue(content);
    };

    reader.readAsText(file);
}

