<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <div>
    <v-card-title class="pb-0">
      <span>Select PCFG grammar<span class="required primary--text"> *</span></span>
    </v-card-title>
    <pcfg-selector
      v-model="pcfg"
      @input="checkValidInit"
    />

    <v-divider />
    <v-card-title class="pb-0"><span>Select rule file</span></v-card-title>
    <rules-selector v-model="rules" @input="checkValidInit"></rules-selector>
    <v-card-title class="pb-0">
      <span>Edit keyspace limit</span>
    </v-card-title>
    <v-card-text>
      <v-text-field
        v-model.number="keyspaceLimit"
        text
        single-line
        :value="this.keyspaceLimit"
        required
        type="number"
        suffix="passwords"
        :max="this.pcfg.keyspace"
        @input="checkValidEdit"
      />
    </v-card-text>
  </div>
</template>

<script>
  import pcfgSelector from '@/components/selector/pcfgSelector'
  import ruleSelector from '@/components/selector/rulesSelector'
  export default {
    name: "Pcfg",
    components: {
      'pcfg-selector': pcfgSelector,
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
        attackId: 9,
        attackName: 'pcfg',
        valid: false,
        pcfg: [],
        rules: null,
        keyspaceLimit: 0,


      }
    },
    watch:{
      value: function(){
        if (this.value.pcfg_grammar)
          this.pcfg = this.value.pcfg_grammar
        if (this.value.rules)
          this.rules = this.value.rules
        if (this.pcfg)
          this.keyspaceLimit = this.pcfg.keyspace
        //  this.pcfg.time = "2019-04-16T15:28:52"

      }

    },
    methods: {
      checkValidInit: function () {
        if(this.keyspaceLimit !== this.pcfg.keyspace)
          this.keyspaceLimit = this.pcfg.keyspace
        if(this.keyspaceLimit > this.pcfg.keyspace)
          this.keyspaceLimit = this.pcfg.keyspace

      /*  if (this.pcfg_grammmar.length > 0) {
        alert('motýle')*/
          this.$emit('input', {
            'attack_mode': this.attackId,
            'attack_name': this.attackName,
            'rules': this.rules,
            'pcfg_grammar': this.pcfg,
            'keyspace_limit': this.keyspaceLimit
          })
          return true
      //  }
      //  return false
    },
    checkValidEdit: function () {
      if(this.keyspaceLimit > this.pcfg.keyspace)
        this.keyspaceLimit = this.pcfg.keyspace

    /*  if (this.pcfg_grammmar.length > 0) {
      alert('motýle')*/
        this.$emit('input', {
          'attack_mode': this.attackId,
          'attack_name': this.attackName,
          'rules': this.rules,
          'pcfg_grammar': this.pcfg,
          'keyspace_limit': this.keyspaceLimit
        })
        return true
    //  }
    //  return false
  },
    myFunction: function() {
      let context = this
      setTimeout(() => {
        this.keyspaceLimit = this.pcfg.keyspace
      }, 0)
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
