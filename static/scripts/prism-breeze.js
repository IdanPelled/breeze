Prism.languages.breeze = {
    'comment': {
        pattern: /\/\/.*/,
        alias: 'comment'
    },
    'literal-text': {
        pattern: /"(?:\\.|[^"])*"/,
        alias: 'literal-text'
    },
    'literal-number': {
        pattern: /-?\d+/,
        alias: 'literal-number'
    },
    'literal-bool': {
        pattern: /no|yes/,
        alias: 'literal-bool'
    },
    'built-in': {
        pattern: /out|in|to-number|to-text/,
        alias: 'builtin'
    },
    'keyword': {
        pattern: /set|to|when|do|otherwise|loop|times|while/,
        alias: 'keyword'
    },
    'identifier': {
        pattern: /[a-zA-Z][a-zA-Z0-9_-]*/,
        alias: 'variable'
    },
    'operator': {
        pattern: /[><=+\-*\/]/,
        alias: 'operator'
    },
    'punctuation': {
        pattern: /[\{\}\(\)]/,
        alias: 'punctuation'
    }
};
  