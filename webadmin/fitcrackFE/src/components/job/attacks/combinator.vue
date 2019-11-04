<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <div>
    <v-row>
      <v-col
        cols="6"
        class="border"
      >
        <v-card-title class="pb-0">
          <span>Select left dictionary<span class="required primary--text"> *</span></span>
        </v-card-title>
      </v-col>
      <v-col cols="6">
        <v-card-title class="pb-0">
          <span>Select right dictionary<span class="required primary--text"> *</span></span>
        </v-card-title>
      </v-col>
      <v-col
        cols="6"
        class="border"
      >
        <dict-selector
          v-model="leftDictionaries"
          @input="checkValid"
        />
      </v-col>
      <v-col cols="6">
        <dict-selector
          v-model="rightDictionaries"
          @input="checkValid"
        />
      </v-col>

      <v-col
        cols="6"
        class="border"
      >
        <v-card-title class="pb-0">
          <span>Type left rule</span>
        </v-card-title>
      </v-col>
      <v-col cols="6">
        <v-card-title class="pb-0">
          <span>Type right rule</span>
        </v-card-title>
      </v-col>

      <v-col
        cols="6"
        class="border px-2"
      >
        <v-text-field
          v-model="ruleLeft"
          outlined
          single-line
          placeholder="Rule"
          @input="checkValid"
        />
      </v-col>
      <v-col
        cols="6"
        class="px-2"
      >
        <v-text-field
          v-model="ruleRight"
          outlined
          single-line
          placeholder="Rule"
          @input="checkValid"
        />
      </v-col>
    </v-row>
  </div>
</template>

<script>
  import dictSelector from '@/components/selector/dictionarySelector'
  export default {
    name: "Combinator",
    components: {
      'dict-selector': dictSelector
    },
    props: {
      value: {
        type: [Boolean, Object],
        default: null
      },
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

