<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <div>
    <v-card-title class="pb-0"><h2>Select PCFG grammar<span class="required primary--text"> *</span></h2></v-card-title>
    <pcfg-selector v-model="pcfg" @input="checkValid"></pcfg-selector>

    <v-divider></v-divider>
    <v-card-title class="pb-0"><h2>Select rule file</h2></v-card-title>
    <rules-selector v-model="rules" @input="checkValid"></rules-selector>
  </div>
</template>

<script>
  import pcfgSelector from '@/components/selector/pcfgSelector'
  import ruleSelector from '@/components/selector/rulesSelector'
  export default {
    name: "pcfg",
    props: {
      value: {
        type: [Boolean, Object],
        default: null
      },
    },
    watch:{
      value: function(){
        if (this.value.pcfg_grammar)
          this.pcfg = this.value.pcfg_grammar
        if (this.value.rules)
          this.rules = this.value.rules
      }
    },
    components: {
      'pcfg-selector': pcfgSelector,
      'rules-selector': ruleSelector
    },
    methods: {
      checkValid: function () {

      /*  if (this.pcfg_grammmar.length > 0) {
        alert('motýle')*/
          this.$emit('input', {
            'attack_mode': this.attackId,
            'attack_name': this.attackName,
            'rules': this.rules,
            'pcfg_grammar': this.pcfg
          })
          return true
      //  }
      //  return false
      }
    },
    data: function () {
      return {
        attackId: 9,
        attackName: 'pcfg',
        valid: false,
        pcfg: [],
        rules: null,
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
