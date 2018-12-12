<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
    <v-expansion-panel-content
      lazy
      expand-icon="edit"
      @input="focus"
      class="expaPanel"
      :hide-actions="true"
      :readonly="openForever"
      :value="openForever"
    >
      <div slot="header">
        <v-layout row wrap>
          <v-flex>
            <div  class="width100 fakeInput" :class="{validationError: validateError}" v-html="parsedHTML"></div>
          </v-flex>

        </v-layout>

      </div>
      <div>
        <v-layout row >
          <v-layout row justify-center wrap>
            <v-tooltip top v-for="(item, key) in represenArray" :key="key">
              <v-btn slot="activator"
                     color="primary"
                     class="maskBtn py-0 mx-1"
                     outline
                     @click="addSymbol(key)"
              >{{item.represent}}</v-btn>
              <span>{{item.chars}}</span>
            </v-tooltip>
            <v-tooltip top v-for="(item, index) in customCharsets">
              <v-btn slot="activator"
                     color="primary"
                     class="maskBtn py-0 mx-1"
                     outline
                     @click="addSymbol(index + 1)"
              >{{item.name}}</v-btn>
              <span>{{item.name}}</span>
            </v-tooltip>
          </v-layout>
        </v-layout>
        <v-text-field
          ref="maskInput"
          autofocus
          label="Mask"
          outline
          class="primary--text px-2 mx-4"
          single-line
          :rules="maskRules"
          :value="value"
          :error="validateError"
          @input="update"

        ></v-text-field>
      </div>
    </v-expansion-panel-content>
</template>

<script>
  export default {
    name: "maskSingle",
    props: {
      value: {
        type: String,
        default: ''
      },
      openForever: {
        type: Boolean,
        default: false
      },
      customCharsets: {
        type: [Array, null],
        default: null
      }
    },
    computed: {
      parsedHTML() {
        var nextCharSymbol = false
        var parsed = ''
        if (this.value === '') {
          if (this.openForever) {
            return '<span class="grey--text">Write mask down.</span>'
          } else {
            return '<span class="grey--text">Empty mask. Click to edit.</span>'
          }
        }
        for (var i = 0; i < this.value.length; i++) {
          if (nextCharSymbol) {
            if (this.value[i] === '?') {
              parsed += '?'
              nextCharSymbol = false
              continue
            }
            var symbol = this.represenArray[this.value[i]]
            if (symbol === undefined) {
              if (this.customCharsets !== null && this.customCharsets[parseInt(this.value[i]) - 1] ) {
                symbol = {
                  represent: this.customCharsets[parseInt(this.value[i]) - 1].name
                }
              } else {
                this.validateError = true
                return '<span class="grey--text">Not valid mask.</span>'
              }
            }
            parsed += '<span class="symbol primary--text">' + symbol.represent + '</span>'
            nextCharSymbol = false
            continue
          }
          if (this.value[i] === '?') {
            nextCharSymbol = true
          } else {
            parsed += this.value[i]
            nextCharSymbol = false
          }
        }
        this.validateError = false
        return parsed
      }
    },
    methods: {
      addSymbol: function (symbol) {
        this.$emit('input', this.value + '?' + symbol)
      },
      update: function (e) {
        this.$emit('input', e)
      },
      focus: function (e) {
        if (e) {
          this.$nextTick(() => {
            this.$refs.maskInput.focus()
          })
        }
      }
    },
    data() {
      return {
        validateError: false,
        rawValue: '',
        maskRules: [
          v => /^(\?[ludhHsab\?]|[ -~])*$/.test(v) || ''
        ],
        editing: true,
        represenArray: {
          'l': {
            'chars': 'abcdefghijklmnopqrstuvwxyz',
            'represent': 'a-z'
          },
          'u': {
            'chars': 'ABCDEFGHIJKLMNOPQRSTUVWXYZ',
            'represent': 'A-Z'
          },
          'd': {
            'chars': '0123456789',
            'represent': '0-9'
          },
          'h': {
            'chars': '0123456789abcdef',
            'represent': '0-f'
          },
          'H': {
            'chars': '0123456789ABCDEF',
            'represent': '0-F'
          },
          's': {
            'chars': '«space»!"#$%&()*+,-./:;<=>?@[]^_`{|}~',
            'represent': 'special'
          },
          'a': {
            'chars': '?l?u?d?s',
            'represent': 'a-z,A-Z,0-9,special'
          },
          'b': {
            'chars': '0x00 - 0xff',
            'represent': 'ASCII'
          }
        }
      }
    },
  }
</script>
<style scoped>
  .title {
    padding: 8px;
    font-size: 14px;
    font-weight: 400;
  }

  .maskBtn {
    min-width: 0px;
    text-transform: none;
    padding: 0 10px;
  }

  .fakeInput {
    border: 2px solid rgba(0,0,0,.54);
    border-radius: 4px;
    padding: 6px 12px;
    margin: 0 10px;
  }

  .validationError {
    border-color: #ff5252;
  }


</style>
<style >
  .symbol {
    margin-left: 1px;
    margin-right: 1px;
    border: 1px solid currentColor;
    padding: 3px;
    border-radius: 4px;
    font-weight: 500;
  }
</style>
