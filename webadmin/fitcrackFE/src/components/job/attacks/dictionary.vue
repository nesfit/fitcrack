<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <div>
    <v-card-title class="pb-0">
      <span>Select dictionary<span class="required primary--text"> *</span></span>
    </v-card-title>
    <dict-selector
      v-model="dictionaries"
      @input="checkValid"
    />

    <v-divider />
    <v-card-title class="pb-0">
      <span>Select rule file</span>
    </v-card-title>
    <rules-selector
      v-model="rules"
      @input="checkValid"
    />
  </div>
</template>

<script>
  import dictSelector from '@/components/selector/dictionarySelector'
  import ruleSelector from '@/components/selector/rulesSelector'
  export default {
    name: "Dictionary",
    components: {
      'dict-selector': dictSelector,
      'rules-selector': ruleSelector
    },
    props: {
      value: {
        type: [Boolean, Object],
        default: null
      },
    },
    data: function () {
      return {
        attackId: 0,
        attackName: 'dict',
        valid: false,

        dictionaries: [],
        rules: null,
      }
    },
    watch:{
      value: function(){
        if (this.value.left_dictionaries)
          this.dictionaries = this.value.left_dictionaries
        if (this.value.rules)
          this.rules = this.value.rules
      }
    },
    methods: {
      checkValid: function () {
        if (this.dictionaries.length > 0) {
          this.$emit('input', {
            'attack_mode': this.attackId,
            'attack_name': this.attackName,
            'rules': this.rules,
            'left_dictionaries': this.dictionaries
          })
          return true
        }
        return false
      }
    }
  }
</script>

<style scoped>
  .required {
    font-weight: bold;
    color: #000;
  }
</style>
