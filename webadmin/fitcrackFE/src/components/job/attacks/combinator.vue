<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <div >
    <v-layout row wrap>
      <v-flex xs6 class="border">
        <v-card-title class="pb-0"><h2>Select left dictionary<span class="required primary--text"> *</span></h2></v-card-title>
      </v-flex>
      <v-flex xs6>
        <v-card-title class="pb-0"><h2>Select right dictionary<span class="required primary--text"> *</span></h2></v-card-title>
      </v-flex>
      <v-flex xs6 class="border">
        <dict-selector v-model="leftDictionaries" @input="checkValid"></dict-selector>
      </v-flex>
      <v-flex xs6>
        <dict-selector v-model="rightDictionaries" @input="checkValid"></dict-selector>
      </v-flex>

      <v-flex xs6 class="border">
        <v-card-title class="pb-0"><h2>Type left rule</h2></v-card-title>
      </v-flex>
      <v-flex xs6>
        <v-card-title class="pb-0"><h2>Type right rule</h2></v-card-title>
      </v-flex>

      <v-flex xs6 class="border px-2">
        <v-text-field
          outline
          single-line
          placeholder="Rule"
          v-model="ruleLeft"
          @input="checkValid"
        ></v-text-field>
      </v-flex>
      <v-flex xs6  class="px-2">
        <v-text-field
          outline
          single-line
          placeholder="Rule"
          v-model="ruleRight"
          @input="checkValid"
        ></v-text-field>
      </v-flex>
    </v-layout>
  </div>
</template>

<script>
  import dictSelector from '@/components/selector/dictionarySelector'
  export default {
    name: "combinator",
    props: {
      value: {
        type: [Boolean, Object],
        default: null
      },
    },
    watch:{
      value: function(){
        console.log('value changed')
        if (this.value.leftDictionaries)
          this.leftDictionaries = this.value.leftDictionaries
        if (this.value.rightDictionaries)
          this.rightDictionaries = this.value.rightDictionaries
        if (this.value.ruleLeft)
          this.ruleLeft = this.value.ruleLeft
        if (this.value.ruleRight)
          this.ruleRight = this.value.ruleRight
      }
    },
    components: {
      'dict-selector': dictSelector
    },
    methods: {
      checkValid: function () {
        if (this.leftDictionaries.length > 0 && this.rightDictionaries.length > 0) {
          this.$emit('input', {
            'attack_mode': this.attackId,
            'attack_name': this.attackName,
            'rule_left': this.ruleLeft,
            'rule_right': this.ruleRight,
            'left_dictionaries': this.leftDictionaries,
            'right_dictionaries': this.rightDictionaries
          })
          return true
        }
        return false
      }
    },
    data: function () {
      return {
        attackId: 1,
        attackName: 'combinator',
        valid: false,
        leftDictionaries: [],
        rightDictionaries: [],
        ruleLeft: '',
        ruleRight: ''
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
</style>

