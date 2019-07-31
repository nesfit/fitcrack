<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <div>
    <v-card-title class="pb-0"><h2>Select dictionary<span class="required primary--text"> *</span></h2></v-card-title>
    <dict-selector v-model="dictionaries" @input="checkValid"></dict-selector>

    <v-divider></v-divider>
    <v-card-title class="pb-0"><h2>Select pcfg file</h2></v-card-title>
    <pcfg-selector v-model="pcfg" @input="checkValid"></pcfg-selector>
  </div>
</template>

<script>
  import dictSelector from '@/components/selector/dictionarySelector'
  import pcfgSelector from '@/components/selector/pcfgSelector'
  export default {
    name: "dictionary",
    props: {
      value: {
        type: [Boolean, Object],
        default: null
      },
    },
    watch:{
      value: function(){
        if (this.value.left_dictionaries)
          this.dictionaries = this.value.left_dictionaries
        if (this.value.pcfg)
          this.pcfg = this.value.pcfg
      }
    },
    components: {
      'dict-selector': dictSelector,
      'pcfg-selector': pcfgSelector
    },
    methods: {
      checkValid: function () {
        if (this.dictionaries.length > 0) {
          this.$emit('input', {
            'attack_mode': this.attackId,
            'attack_name': this.attackName,
            'rules': this.pcfg,
            'left_dictionaries': this.dictionaries
          })
          return true
        }
        return false
      }
    },
    data: function () {
      return {
        attackId: 0,
        attackName: 'pcfg',
        valid: false,

        dictionaries: [],
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
