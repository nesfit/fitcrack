<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <div>
    <v-layout row wrap>
      <v-flex xs6 class="border">
        <v-card-title class="pb-0"><h2>Select dictionary<span class="required primary--text"> *</span></h2></v-card-title>
      </v-flex>
      <v-flex xs6>
        <v-card-title class="pb-0"><h2>Type mask<span class="required primary--text"> *</span></h2></v-card-title>
      </v-flex>
      <v-flex xs6 class="border">
        <dict-selector v-model="dictionaries" @input="checkValid"></dict-selector>
      </v-flex>
      <v-flex xs6>
        <v-expansion-panel class="elevation-0 pt-2">
          <mask-single :openForever="true" v-model="mask" @input="checkValid"></mask-single>
        </v-expansion-panel>
      </v-flex>

      <v-flex xs6 class="border">
        <v-card-title class="pb-0"><h2>Type rule</h2></v-card-title>
      </v-flex>
      <v-flex xs6>
        <v-card-title class="pb-0"><h2>Type rule</h2></v-card-title>
      </v-flex>

      <v-flex xs6 class="border px-2">
        <v-text-field
          outline
          single-line
          placeholder="Rule left"
          v-model="ruleLeft"
          @input="checkValid"
        ></v-text-field>
      </v-flex>
      <v-flex xs6  class="px-2">
        <v-text-field
          outline
          single-line
          placeholder="Rule right"
          v-model="ruleRight"
          @input="checkValid"
        ></v-text-field>
      </v-flex>
    </v-layout>

  </div>
</template>

<script>
  import dictSelector from '@/components/selector/dictionarySelector'
  import maskSingle from '@/components/mask/maskSingle'
  export default {
    name: "hybridMaskWordlist",
    props: {
      value: {
        type: [Boolean, Object],
        default: null
      },
    },
    watch:{
      value: function(){

      }
    },
    components: {
      'mask-single': maskSingle,
      'dict-selector': dictSelector
    },
    methods: {
      dictSelected: function (id, dictNubmer) {
        this.selectedDict1Id = id
        this.checkValid();
      },
      addCharToMask: function (char) {
        this.mask += char
        this.checkValid()
      },
      checkValid: function () {
        if (this.mask !== '' && this.dictionaries.length > 0) {
          this.$emit('input', {
            'attack_mode': this.attackId,
            'attack_name': this.attackName,
            'rule_left': this.ruleLeft,
            'rule_right': this.ruleRight,
            'left_dictionaries': this.dictionaries,
            'mask': this.mask
          })
          return true
        }
        return false
      },
    },
    data: function () {
      return {
        attackId: 6,
        attackName: 'Hybrid wordlist+mask',
        dictionaries: [],
        valid: false,
        mask: '',
        ruleLeft: '',
        ruleRight: '',
        maskRules: [
          v => /^(\?[ludhHsab]|[ -~])+$/.test(v) || 'Not valid mask'
        ]
      }
    }
  }
</script>

<style scoped>
  .border {
    border-right: 2px solid rgba(0,0,0,.12)
  }

  .required {
    font-weight: bold;
    color: #000;
  }

  .maskBtn {
    min-width: 0px;
    max-width: 40px;
    text-transform: none;
  }

  .maskBtns {
    text-align: center;
  }

</style>

<style>
  .selected {
    background: rgba(37, 157, 173, 0.85) !important;
    color: white;
  }

  .selected a {
    color: white;
  }

  .clickable {
    cursor: pointer;
  }
</style>
