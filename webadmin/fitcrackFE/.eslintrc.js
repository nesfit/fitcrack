module.exports = {
    "env": {
        "browser": true,
        "es6": true
    },
    "extends": 'plugin:vue/recommended',
    "parserOptions": {
        "sourceType": "module"
    },
    plugins: [
      'vue',
      'vuetify'
    ],
    "rules": {
      /*
        "indent": [
            "error",
            2
        ],
        "linebreak-style": [
            "error",
            "unix"
        ],
        "quotes": [
            "error",
            "single"
        ],
        "semi": [
            "error",
            "never"
        ],
        */
        'vuetify/no-deprecated-classes': 'error',
        'vuetify/grid-unknown-attributes': 'error',
        'vuetify/no-legacy-grid': 'error'
    }
};